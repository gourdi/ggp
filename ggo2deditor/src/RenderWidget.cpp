#include "RenderWidget.h"
#include <QtGui/qguiapplication.h>
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

#include "DiscFactory.h"
#include "PolygonFactory.h"
#include "ShapeHandler.h"

namespace
{
  const std::array<float, 13> zoomFactors = { 0.25f, 0.35f, 0.5f, 0.65f, 0.75f, 0.9f, 1.f, 1.1f, 1.35f, 1.50f, 2.f, 2.f, 4.f };
}

/////////////////////////////////////////////////////////////////////
RenderWidget::RenderWidget(QWidget * parent)
:
QWidget(parent),
_zoomIndex(int(zoomFactors.size() / 2 + 1))
{
  setMouseTracking(true);
}

/////////////////////////////////////////////////////////////////////
void RenderWidget::paintEvent(QPaintEvent * event)
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
  if (_edition != nullptr)
  {
    _edition->Draw(painter, size().width(), size().height(), getCanvasView());
  }
}

/////////////////////////////////////////////////////////////////////
void RenderWidget::resizeEvent(QResizeEvent *event)
{
  _image = QImage(size(), QImage::Format_RGB32);

  QWidget::resizeEvent(event);
}

/////////////////////////////////////////////////////////////////////
void RenderWidget::mousePressEvent(QMouseEvent *eventPress)
{
  // First check if a shape is currently built.
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

  // Then check for edition.
  for (auto * shapeHandler : _selection)
  {
    if (shapeHandler->OnMouseDown(eventPress->button(), eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView()) == true)
    {
      _selection = { shapeHandler };
      _edition = shapeHandler;
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
void RenderWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
  // Shape factory.
  if (_shapeFactory)
  {
    auto shapeHandler = _shapeFactory->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), _canvas, getCanvasView());

    if (shapeHandler != nullptr)
    {
      _selection = { shapeHandler };
      _shapeHandlers.emplace_back(shapeHandler);
      _shapeFactory.reset();
    }

    update();
    return;
  }

  // Edition.
  if (_edition != nullptr &&
      _edition->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), getCanvasView()) == true)
  {
    update();
    return;
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
void RenderWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
  _hasMouveMoved = true;

  // A shape is built.
  if (_shapeFactory != nullptr)
  {
    _shapeFactory->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), _canvas, getCanvasView());
    update();
    return;
  }

  // A shape is edited.
  if (_edition != nullptr)
  {
    auto mouveMoveData = _edition->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView());
    setCursor(mouveMoveData._cursor);
    if (mouveMoveData._update_widget == true)
    {
      update();
    }
    return;
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
void RenderWidget::wheelEvent(QWheelEvent *event)
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
QSize RenderWidget::minimumSizeHint() const
{
  return QSize(200, 100);
}

/////////////////////////////////////////////////////////////////////
QSize RenderWidget::sizeHint() const
{
  return QSize(400, 400);
}

/////////////////////////////////////////////////////////////////////
ggo::canvas::view RenderWidget::getCanvasView() const
{
  return ggo::canvas::view({ 0.f, 0.f }, zoomFactors[_zoomIndex], ggo::canvas::main_direction::vertical);
}

/////////////////////////////////////////////////////////////////////
void RenderWidget::createDisc()
{
  _selection.clear();
  _shapeFactory.reset(new DiscFactory());
}

/////////////////////////////////////////////////////////////////////
void RenderWidget::createPolygon()
{
  _selection.clear();
  _shapeFactory.reset(new PolygonFactory());
}

/////////////////////////////////////////////////////////////////////
ShapeHandler * RenderWidget::hitTest(int x, int y, int width, int height, const ggo::canvas::view & view)
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

