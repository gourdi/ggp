#ifndef GGO_POLYGON_FACTORY
#define GGO_POLYGON_FACTORY

#include "ShapeFactory.h"

class PolygonFactory : public ShapeFactory
{
private:

  void OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  void OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  bool OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view) override;

private:

  ggo::polygon2d_float * _polygon = nullptr;
};

#endif

