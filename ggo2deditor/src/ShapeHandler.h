#ifndef GGO_SHAPE_HANDLER
#define GGO_SHAPE_HANDLER

#include <ggo_canvas.h>
#include <ggo_shapes2d.h>
#include <QtGui/qpainter.h>
#include <QtGui/qcursor.h>

class ShapeHandler
{
public:

  struct MouseMoveData
  {
    QCursor _cursor;
    bool _update_widget;
  };

  virtual void Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const = 0;

  virtual bool OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) = 0;
  virtual bool OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) = 0;
  virtual MouseMoveData OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view) = 0;

protected:

  static QRect GetHandleRect(int x, int y);

  static  bool HitTestHandle(int handle_x, int handle_y, int test_x, int test_y);

  static  void PaintHandle(QPainter & painter, int x, int y);
  static  void PaintHandle(QPainter & painter, const ggo::pos2f & p);

  static constexpr int half_tolerance = 3;
  static constexpr int tolerance = 2 * half_tolerance + 1;
};

#endif
