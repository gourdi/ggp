#ifndef GGO_POLYGON_FACTORY
#define GGO_POLYGON_FACTORY

#include "ShapeFactory.h"

class PolygonFactory : public ShapeFactory
{
private:

  ShapeHandler * OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  ShapeHandler * OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  void OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view) override;

private:

  ggo::canvas::polygon * _polygon = nullptr;
};

#endif

