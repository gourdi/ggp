#ifndef GGO_SHAPE_FACTORY
#define GGO_SHAPE_FACTORY

#include <QtWidgets/qwidget.h>
#include <ggo_canvas.h>

class ShapeHandler;

class ShapeFactory
{
public:

  ShapeFactory(const ggo::color_8u & c) : _color(c) {}

  // Return a non-null pointer once the shape has been creaed.
  virtual ShapeHandler * OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0;

  // Return a non-null pointer once the shape has been creaed.
  virtual ShapeHandler * OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0;

  virtual void OnMouseMove(int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0;

protected:

  const ggo::color_8u _color;
};

#endif
