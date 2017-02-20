#include "RenderWidget.h"
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

RenderWidget::RenderWidget(QWidget * parent) : QWidget(parent)
{
}

void RenderWidget::paintEvent(QPaintEvent * event)
{
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, image, dirtyRect);
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
  image = QImage(size(), QImage::Format_RGB32);

  ggo::fill_solid<ggo::bgra_8u_yd>(image.bits(), size().width(), size().height(), image.bytesPerLine(), ggo::green_8u());

  ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_2x2>(image.bits(), size().width(), size().height(), image.bytesPerLine(),
    ggo::disc_float(100.f, 100.f, 100.f), ggo::red_8u());

  QWidget::resizeEvent(event);
}

QSize RenderWidget::minimumSizeHint() const
{
  return QSize(200, 100);
}

QSize RenderWidget::sizeHint() const
{
  return QSize(400, 400);
}