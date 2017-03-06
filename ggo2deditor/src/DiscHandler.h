#ifndef GGO_DISC_HANDLER
#define GGO_DISC_HANDLER

#include "ShapeHandler.h"
#include <ggo_canvas.h>

class DiscHandler : public ShapeHandler
{
public:

  enum class Modifier
  {
    none,
    bottom,
    top,
    left,
    right,
    bottom_left,
    bottom_right,
    top_left,
    top_right
  };

  DiscHandler(ggo::canvas::disc * disc) : _disc(disc) {}

  void Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const override;

  bool OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) override;
  bool OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) override;
  MouseMoveData OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view) override;

private:

  Modifier HitTest(int x, int y, int width, int height, const ggo::canvas::view & view) const;

  ggo::canvas::disc * _disc;
  ggo::pos2f _mouse_down_center;
  float _mouse_down_radius;
  Modifier _modifier = Modifier::none;
};

#endif
