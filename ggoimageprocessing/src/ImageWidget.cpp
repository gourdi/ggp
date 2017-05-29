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
  _image_processor.reset(new ggo::image_processor(filename.toUtf8()));

  _view_basis = ggo::compute_fit_view_basis(_image_processor->width(), _image_processor->height(), _image.width(), _image.height());

  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoom(float zoomFactor)
{
  // Get the  position of the center of the view in the original image.
  ggo::pos2f center = _view_basis.pos() + (0.5f * _image.width() - 0.5f) * _view_basis.x() + (0.5f * _image.height() - 0.5f) * _view_basis.y();

  zoom(zoomFactor, center);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoom(float zoomFactor, const ggo::pos2f & center)
{
  _view_basis.x() *= zoomFactor;
  _view_basis.y() *= zoomFactor;

  // Make it so that that the current view still have the same center than before zooming.
  _view_basis.pos() = center - (0.5f * _image.width() - 0.5f) * _view_basis.x() - (0.5f * _image.height() - 0.5f) * _view_basis.y();

  _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(), _image.width(), _image.height(), _view_basis);

  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomIn()
{
  zoom(0.5f);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomOut()
{
  zoom(2.f);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomFit()
{
  _view_basis = ggo::compute_fit_view_basis(_image_processor->width(), _image_processor->height(), _image.width(), _image.height());

  update();
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
  if (_image_processor)
  {
    ggo::pos2f center = _view_basis.pos() + (0.5f * _image.width() - 0.5f) * _view_basis.x() + (0.5f * _image.height() - 0.5f) * _view_basis.y();

    _image = QImage(size(), QImage::Format_RGB32);

    _view_basis.pos() = center - (0.5f * _image.width() - 0.5f) * _view_basis.x() - (0.5f * _image.height() - 0.5f) * _view_basis.y();

    _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(), _image.width(), _image.height(), _view_basis);
  }
  else
  {
    _image = QImage(size(), QImage::Format_RGB32);
  }

  QWidget::resizeEvent(event);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mousePressEvent(QMouseEvent * eventPress)
{
  _is_mouse_down = true;
  _mouse_down_pos = ggo::pos2i(eventPress->pos().x(), height() - eventPress->pos().y() - 1);
  _mouse_down_view_basis = _view_basis;
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
  _is_mouse_down = false;
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mouseMoveEvent(QMouseEvent * eventMove)
{
  if (_is_mouse_down == true)
  {
    float dx = static_cast<float>(eventMove->pos().x() - _mouse_down_pos.x());
    float dy = static_cast<float>(eventMove->pos().y() - (height() - _mouse_down_pos.y() - 1));
    _view_basis.pos() = _mouse_down_view_basis.pos() - dx * _view_basis.x() + dy * _view_basis.y();

    _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(),
      _image.width(), _image.height(), _view_basis);

    update();
  }
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::wheelEvent(QWheelEvent *event)
{
  // Get the  position of the center of the view in the original image.
  float x = event->pos().x();
  float y = _image.height() - event->pos().y() - 1.f;
  ggo::pos2f center = _view_basis.pos() + x * _view_basis.x() + y * _view_basis.y();

  if (event->angleDelta().ry() > 0)
  {
    zoom(0.5f, center);
  }

  if (event->angleDelta().ry() < 0)
  {
    zoom(2.0f, center);
  }
}

