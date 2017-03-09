#include "RenderWidget.h"
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

RenderWidget::RenderWidget(QWidget * parent)
:
QWidget(parent),
_zoomIndex(int(zoomFactors.size() / 2 + 1))
{
  setMouseTracking(true);
}

void RenderWidget::paintEvent(QPaintEvent * event)
{
  QPainter painter(this);

  // First render canvas.
  _canvas.render(_image.bits(), getCanvasView(), size().width(), size().height(), _image.bytesPerLine(), ggo::bgra_8u_yd);

  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);

  // Let the current shape handler paint itself.
  if (_editShapeHandler != nullptr)
  {
    _editShapeHandler->Draw(painter, size().width(), size().height(), getCanvasView());
  }
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
  _image = QImage(size(), QImage::Format_RGB32);

  QWidget::resizeEvent(event);
}

void RenderWidget::mousePressEvent(QMouseEvent *eventPress)
{
  // First check if a shape is currently built.
  if (_shapeFactory)
  {
    auto shapeHandler = _shapeFactory->OnMouseDown(eventPress->button(), eventPress->x(), eventPress->y(), size().width(), size().height(), _canvas, getCanvasView());

    if (shapeHandler != nullptr)
    {
      _editShapeHandler = shapeHandler;
      _shapeHandlers.emplace_back(shapeHandler);
      _shapeFactory.reset();
    }

    update();
    return;
  }

  // Then check if a shape is currently edited.
  if (_editShapeHandler != nullptr &&
      _editShapeHandler->OnMouseDown(eventPress->button(), eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView()) == true)
  {
    return; // Event is consumed.
  }
  
  // Then check if the user clicked on a shape. In any case, if the shape handler did not consume the mouse event, edition is stopped.
  _editShapeHandler = nullptr;
  _moveShapeHandler = hitTest(eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView());
  if (_moveShapeHandler != nullptr)
  {
    _moveShapeHandler->SetAnchor(eventPress->x(), eventPress->y(), size().width(), size().height(), getCanvasView());
  }
  update();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
  if (_shapeFactory)
  {
    auto shapeHandler = _shapeFactory->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), _canvas, getCanvasView());

    if (shapeHandler != nullptr)
    {
      _editShapeHandler = shapeHandler;
      _shapeHandlers.emplace_back(shapeHandler);
      _shapeFactory.reset();
    }

    update();
    return;
  }

  if (_editShapeHandler != nullptr &&
      _editShapeHandler->OnMouseUp(releaseEvent->button(), releaseEvent->x(), releaseEvent->y(), size().width(), size().height(), getCanvasView()) == true)
  {
    return; // Event consumed.
  }

  // Select the shape (if any) after it has been moved.
  _editShapeHandler = _moveShapeHandler;
  _moveShapeHandler = nullptr;
  update();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
  // A shape is built.
  if (_shapeFactory)
  {
    _shapeFactory->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), _canvas, getCanvasView());
    update();
    return;
  }

  // A shape is edited.
  if (_editShapeHandler != nullptr)
  {
    auto mouveMoveData = _editShapeHandler->OnMouseMove(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView());
    setCursor(mouveMoveData._cursor);
    if (mouveMoveData._update_widget == true)
    {
      update();
    }
    return;
  }

  // A shape is moved.
  if (_moveShapeHandler != nullptr)
  {
    _moveShapeHandler->SetPosition(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView());
    update();
    return;
  }

  // Check if the mouse is on a shape, and if so, set cursor to let the user know he can move the shape.
  setCursor(hitTest(eventMove->x(), eventMove->y(), size().width(), size().height(), getCanvasView()) != nullptr ? Qt::SizeAllCursor : Qt::ArrowCursor);
}

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

QSize RenderWidget::minimumSizeHint() const
{
  return QSize(200, 100);
}

QSize RenderWidget::sizeHint() const
{
  return QSize(400, 400);
}

ggo::canvas::view RenderWidget::getCanvasView() const
{
  return ggo::canvas::view({ 0.f, 0.f }, zoomFactors[_zoomIndex], ggo::canvas::main_direction::vertical);
}

void RenderWidget::createDisc()
{
  _shapeFactory.reset(new DiscFactory());
}

void RenderWidget::createPolygon()
{
  _shapeFactory.reset(new PolygonFactory());
}

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

