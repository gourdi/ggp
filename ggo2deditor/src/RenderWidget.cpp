#include "RenderWidget.h"
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

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
  if (width() > image.width() || height() > image.height())
  {
    int newWidth = qMax(width() + 128, image.width());
    int newHeight = qMax(height() + 128, image.height());
    resizeImage(&image, QSize(newWidth, newHeight));
    update();
  }

  QWidget::resizeEvent(event);
}

void RenderWidget::resizeImage(QImage * image, const QSize & newSize)
{
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));


  for (int i = 0; i < 480; ++i)
  {
    newImage.bits()[4 * i] = 0;
  }



  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

QSize RenderWidget::minimumSizeHint() const
{
  return QSize(200, 100);
}

QSize RenderWidget::sizeHint() const
{
  return QSize(400, 400);
}