#ifndef GGO_SHAPE_FACTORY
#define GGO_SHAPE_FACTORY

#include <QtWidgets/qwidget.h>
#include <ggo_canvas.h>

class ShapeFactory
{
public:

  virtual void OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0;
  virtual void OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0;
  virtual bool OnMouseMove(int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) = 0; // Return false not to update the RenderWidget.
};

#endif
