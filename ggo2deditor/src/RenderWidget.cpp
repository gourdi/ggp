#include "RenderWidget.h"
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

#include "DiscFactory.h"

RenderWidget::RenderWidget(QWidget * parent) : QWidget(parent), _shapeFactory(new DiscFactory())
{
  setMouseTracking(true);
}

void RenderWidget::paintEvent(QPaintEvent * event)
{
  QPainter painter(this);

  _canvas.render(_image.bits(), ggo::canvas::main_direction::vertical, size().width(), size().height(), _image.bytesPerLine(), ggo::bgra_8u_yd);

  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
  _image = QImage(size(), QImage::Format_RGB32);

  QWidget::resizeEvent(event);
}

void RenderWidget::mousePressEvent(QMouseEvent *eventPress)
{
  _shapeFactory->OnMouseDown(eventPress->button(), eventPress->x(), size().height() - eventPress->y() - 1, size().width(), size().height(), _canvas);
  update();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
  _shapeFactory->OnMouseUp(releaseEvent->button(), releaseEvent->x(), size().height() - releaseEvent->y() - 1, size().width(), size().height(), _canvas);
  update();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *eventMove)
{
  if (_shapeFactory->OnMouseMove(eventMove->x(), size().height() - eventMove->y() - 1, size().width(), size().height(), _canvas) == true)
  {
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