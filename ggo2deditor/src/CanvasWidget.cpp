#include "CanvasWidget.h"
#include "MainWindow.h"
#include <QtGui/qguiapplication.h>
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qcolordialog.h>

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

#include "DiscFactory.h"
#include "PolygonFactory.h"
#include "DiscHandler.h"
#include "PolygonHandler.h"

namespace
{
  const std::array<float, 13> zoomFactors = { 0.25f, 0.35f, 0.5f, 0.65f, 0.75f, 0.9f, 1.f, 1.1f, 1.35f, 1.50f, 2.f, 2.f, 4.f };
}

/////////////////////////////////////////////////////////////////////
CanvasWidget::CanvasWidget(QWidget * parent)
:
QWidget(parent),
_zoomIndex(int(zoomFactors.size() / 2 + 1))
{
  setMouseTracking(true);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::on_create_disc(ggo::canvas::disc * disc)
{
  _shapeHandlers.push_back(std::make_unique<DiscHandler>(disc));
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::on_create_polygon(ggo::canvas::polygon * polygon)
{
  _shapeHandlers.push_back(std::make_unique<PolygonHandler>(polygon));
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::loadCanvas(const QString & filename)
{
  _shapeHandlers.clear();
  _canvas.load(filename.toUtf8(), this);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::saveCanvas(const QString & filename) const
{
  _canvas.save(filename.toUtf8());
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::paintEvent(QPaintEvent * event)
{
  QPainter painter(this);

  // First render canvas.
  _canvas.render(_image.bits(), getCanvasView(), size().width(), size().height(), _image.bytesPerLine(), ggo::bgra_8u_yd);

  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);

  // Let the selected and edited shape handlers paint themselves.
  for (const auto * shapeHandler : _selection)
  {
    shapeHandler->Draw(painter, size().width(), size().height(), getCanvasView());
  }

  // Paint the reference frame [-1, 1] x [-1, 1].
  const auto p1 = _canvas.from_view_to_render_pixel({ -1.f, -1.f }, getCanvasView(), size().width(), size().height(), true);
  const auto p2 = _canvas.from_view_to_render_pixel({  1.f,  1.f }, getCanvasView(), size().width(), size().height(), true);

  painter.setPen(QPen(Qt::black, 1, Qt::DotLine));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(p1.x(), p1.y(), p2.x() - p1.x() + 1, p2.y() - p1.y() + 1);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::resizeEvent(QResizeEvent *event)
{
  _image = QImage(size(), QImage::Format_RGB32);

  QWidget::resizeEvent(event);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::mousePressEvent(QMouseEvent *eventPress)
{
  _scrollCanvas = false;

  // Check if a shape is currently built.
  if (_shapeFactory != nullptr)
  {
    auto shapeHandler = _shapeFactory->OnMouseDown(eventPress->button(), eventPress->x(), eventPress->y(), size().width(), size().height(), _canvas, getCanvasView());

    if (shapeHandler != nullptr)
    {
      _selection = { shapeHandler };
      _shapeHandlers.emplace_back(shapeHandler);
      _shapeFactory.reset();
    }

    update();
    return;
  }

  // Scroll view.
  if (_spacePressed == true)
  {
    _scrollCanvas = true;
    return;
  }

  // Check for edition.
  for (auto * shapeHandler : _selection)
  {
    if (shapeHandler->OnMouseDown(eventPress->button(), eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView()) == true)
    {
      update();
      return;
    }
  }

  // Hit shape.
  _mouseDownShape = hitTest(eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView());

  // Set anchor position.
  for (auto * shapeHandler : _selection)
  {
    shapeHandler->SetAnchor(eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView());
  }
  if (_mouseDownShape != nullptr)
  {
    _mouseDownShape->SetAnchor(eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView());
  }

  _hasMouveMoved = false;
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
  // Shape factory.
  if (_shapeFactory)
  {
    auto shapeHandler = _shapeFactory->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), _canvas, getCanvasView());

    if (shapeHandler != nullptr)
    {
      shapeHandler->GetShape()->_color = ggo::color_8u(_color);
      _selection = { shapeHandler };
      _shapeHandlers.emplace_back(shapeHandler);
      _shapeFactory.reset();
    }

    update();
    return;
  }

  // Edition.
  for (auto * shapeHandler : _selection)
  {
    if (shapeHandler->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), getCanvasView()) == true)
    {
      update();
      return;
    }
  }

  if (_hasMouveMoved == false)
  {
    // The mouse has not moved and the CTRL key is down: update selection.
    if (qGuiApp->keyboardModifiers() & Qt::ControlModifier)
    {
      if (_mouseDownShape != nullptr)
      {
        if (ggo::find(_selection, _mouseDownShape) == true)
        {
          ggo::remove(_selection, _mouseDownShape);
        }
        else
        {
          _selection.push_back(_mouseDownShape);
        }
      }
    }
    // The mouse has not moved and the CTRL key is not down: select clicked shape in any.
    else
    {
      _selection.clear();
      if (_mouseDownShape != nullptr)
      {
        _selection.push_back(_mouseDownShape);
      }
    }

    update();
    return;
  }
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
  _hasMouveMoved = true;

  // The space bar is pressed, just return.
  if (_spacePressed == true)
  {
    return;
  }

  // A shape is built.
  if (_shapeFactory != nullptr)
  {
    _shapeFactory->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), _canvas, getCanvasView());
    update();
    return;
  }

  // Let selected shapes handle the mouse move.
  for (auto * shapeHandler : _selection)
  {
    auto mouveMoveData = shapeHandler->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView());
    
    if (mouveMoveData._update_widget == true)
    {
      update();
    }
    if (mouveMoveData._consume_event)
    {
      setCursor(mouveMoveData._cursor);
      return;
    }
  }

  // Selection is moved.
  if (eventMove->buttons() == Qt::LeftButton)
  {
    if (_mouseDownShape != nullptr)
    {
      ggo::push_once(_selection, _mouseDownShape);
    }
    for (auto * shapeHandler : _selection)
    {
      shapeHandler->SetPosition(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView());
    }
    update();
    return;
  }

  // Check if the mouse is on a shape, and if so, set cursor to let the user know he can move the shape.
  setCursor(hitTest(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView()) != nullptr ? Qt::SizeAllCursor : Qt::ArrowCursor);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::wheelEvent(QWheelEvent *event)
{
  if (event->angleDelta().ry() > 0 && _zoomIndex > 0)
  {
    --_zoomIndex;
    update();
  }

  if (event->angleDelta().ry() < 0 && _zoomIndex < zoomFactors.size() - 1)
  {
    ++_zoomIndex;
    update();
  }
}

/////////////////////////////////////////////////////////////////////
QSize CanvasWidget::minimumSizeHint() const
{
  return QSize(200, 100);
}

/////////////////////////////////////////////////////////////////////
QSize CanvasWidget::sizeHint() const
{
  return QSize(400, 400);
}

/////////////////////////////////////////////////////////////////////
ggo::canvas::view CanvasWidget::getCanvasView() const
{
  return ggo::canvas::view({ 0.f, 0.f }, zoomFactors[_zoomIndex], ggo::canvas::main_direction::vertical);
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::createDisc()
{
  _selection.clear();
  _shapeFactory.reset(new DiscFactory(_color));
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::createPolygon()
{
  _selection.clear();
  _shapeFactory.reset(new PolygonFactory(_color));
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::setShapeColor()
{
  QColor qcolor(_color.r(), _color.g(), _color.b());
  qcolor = QColorDialog::getColor(qcolor, this);
  _color = ggo::color_8u(qcolor.red(), qcolor.green(), qcolor.blue());

  for (auto * shape : _selection)
  {
    shape->SetColor(_color);
  }

  update();
}

/////////////////////////////////////////////////////////////////////
ShapeHandler * CanvasWidget::hitTest(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  // Reverse loop.
  for (auto it = _shapeHandlers.rbegin(); it != _shapeHandlers.rend(); ++it)
  {
    auto shape = it->get();
    if (shape->IsPointInside(static_cast<float>(x), static_cast<float>(y), width, height, view) == true)
    {
      return shape;
    }
  }

  return nullptr;
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    setCursor(Qt::OpenHandCursor);
    _spacePressed = true;
  }
}

/////////////////////////////////////////////////////////////////////
void CanvasWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    setCursor(Qt::ArrowCursor);
    _spacePressed = false;;
  }
}


