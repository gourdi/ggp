#ifndef GGO_POLYGON_HANDLER
#define GGO_POLYGON_HANDLER

#include "ShapeHandler.h"

class PolygonHandler : public ShapeHandler
{
public:

  PolygonHandler(ggo::canvas::polygon * polygon) : _polygon(polygon) {}

  void Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const override;

  bool OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) override;
  bool OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view) override;
  MouseMoveData OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view) override;

private:

  ggo::canvas::polygon * _polygon;
};

#endif
