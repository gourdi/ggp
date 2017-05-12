#include <QGuiApplication>
#include <QPainter>
#include <QPaintEvent>

#include "ImageWidget.h"

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

/////////////////////////////////////////////////////////////////////
ImageWidget::ImageWidget(QWidget * parent)
:
QWidget(parent)
{
  setMouseTracking(true);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::loadImage(const QString & filename)
{
  _view_basis = ggo::basis2d<float>({ 0.f, 0.f }, { 1.f, 0.f }, { 0.f, 1.f });

  _image_processor.reset(new ggo::image_processor(filename.toUtf8()));
  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomIn()
{
  _view_basis.x() *= 2.f;
  _view_basis.y() *= 2.f;

  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomOut()
{
  _view_basis.x() /= 2.f;
  _view_basis.y() /= 2.f;

  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomFit()
{

}

/////////////////////////////////////////////////////////////////////
void ImageWidget::paintEvent(QPaintEvent * event)
{
  QPainter painter(this);

  if (_image_processor && _image.bits() != nullptr)
  {
    _image_processor->render(_image.bits(), _image.width(), _image.height(), _image.bytesPerLine(), ggo::bgra_8u_yd, _view_basis);
  }

  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::resizeEvent(QResizeEvent *event)
{
  _image = QImage(size(), QImage::Format_RGB32);

  QWidget::resizeEvent(event);
}

