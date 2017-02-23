#include "RenderWidget.h"
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

class DiscFactory : public ShapeFactory
{
private:

  enum State
  {
    None,
    SettingRadius,
  };

  State _state = None;
  ggo::disc_float * _disc = nullptr;

  void OnMouseDown(Qt::MouseButton, int, int, int, int, ggo::canvas &) override
  {
    // Do nothing.
  }

  void OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas) override
  {
    if (button == Qt::LeftButton)
    {
      switch (_state)
      {
      case None:
      {
        _disc = canvas.create_disc();
        _disc->center() = ggo::canvas::from_render_pixel_to_canvas({ x, y }, ggo::canvas::main_direction::vertical, width, height);

        _state = SettingRadius;
        break;
      }
      case SettingRadius:
        if (_disc->radius() <= 0)
        {
          canvas.remove_shape(_disc);
        }
        _state = None;
        break;
      }
    }
  }

  bool OnMouseMove(int x, int y, int width, int height, ggo::canvas &) override
  {
    switch (_state)
    {
    case None:
      break;
    case SettingRadius:
      const ggo::pos2f p = ggo::canvas::from_render_pixel_to_canvas({ x, y }, ggo::canvas::main_direction::vertical, width, height);
      _disc->set_radius(ggo::distance(_disc->center(), p));
      return true;
    }

    return false;
  }
};

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