#include <QGuiApplication>
#include <QPainter>
#include <QPaintEvent>

#include "ImageWidget.h"
#include "NavigationTool.h"

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
void ImageWidget::loadImage(const std::string & filename)
{
  _tool.reset(new NavigationTool(*this));

  _image_processor.reset(new ggo::image_processor(filename.c_str()));

  _view_basis = ggo::compute_fit_view_basis(_image_processor->width(), _image_processor->height(), _image.width(), _image.height());

  update();
}

/////////////////////////////////////////////////////////////////////
ggo::image_view ImageWidget::get_view() const
{
  ggo::image_view view;
  view._basis = _view_basis;
  view._width = _image.width();
  view._height = _image.height();
  return view;
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoom(float zoomFactor)
{
  // The center of the image remains fixed.
  zoom(zoomFactor, { _image.width() / 2, _image.height() / 2 });
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoom(float zoomFactor, const ggo::pos2i & fixed_view_point)
{
  // The fixed point in image coordinates.
  ggo::pos2f image_fixed_point = _view_basis.pos() + 
    static_cast<float>(fixed_view_point.x()) * _view_basis.x() +
    static_cast<float>(fixed_view_point.y()) * _view_basis.y();

  _view_basis.x() *= zoomFactor;
  _view_basis.y() *= zoomFactor;

  // Make it so that that the current view still have the same fixed than before zooming.
  _view_basis.pos() = image_fixed_point -
    static_cast<float>(fixed_view_point.x()) * _view_basis.x() -
    static_cast<float>(fixed_view_point.y()) * _view_basis.y();

  _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(), get_view());

  update();
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomIn()
{
  zoom(0.75f);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::zoomOut()
{
  zoom(1.5f);
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

    _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(), get_view());
  }
  else
  {
    _image = QImage(size(), QImage::Format_RGB32);
  }

  QWidget::resizeEvent(event);
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mousePressEvent(QMouseEvent * event)
{
  if (_tool)
  {
    _tool->mousePressEvent(event);
  }

  _is_mouse_down = true;
  _mouse_down_pos = ggo::pos2i(event->pos().x(), height() - event->pos().y() - 1);
  _mouse_down_view_basis = _view_basis;
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (_tool)
  {
    _tool->mousePressEvent(event);
  }

  _is_mouse_down = false;
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::mouseMoveEvent(QMouseEvent * event)
{
  if (_tool)
  {
    _tool->mouseMoveEvent(event);
  }


  if (_is_mouse_down == true)
  {
    float dx = static_cast<float>(event->pos().x() - _mouse_down_pos.x());
    float dy = static_cast<float>(event->pos().y() - (height() - _mouse_down_pos.y() - 1));
    _view_basis.pos() = _mouse_down_view_basis.pos() - dx * _view_basis.x() + dy * _view_basis.y();

    _view_basis = ggo::clamp_basis_view(_image_processor->width(), _image_processor->height(), get_view());

    update();
  }
}

/////////////////////////////////////////////////////////////////////
void ImageWidget::wheelEvent(QWheelEvent *event)
{
  if (_tool)
  {
    _tool->wheelEvent(event);
  }
}

