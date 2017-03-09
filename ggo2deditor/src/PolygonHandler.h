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

  void SetAnchor(int x, int y, int width, int height, const ggo::canvas::view & view) override;
  void SetPosition(int x, int y, int width, int height, const ggo::canvas::view & view) override;

  ggo::canvas::shape_abc *        GetShape() override { return _polygon; }
  const ggo::canvas::shape_abc *  GetShape() const override { return _polygon; }

private:

  int HitTest(int x, int y, int width, int height, const ggo::canvas::view & view) const;

  ggo::canvas::polygon * _polygon;

  // Edition.
  int _mouse_down_index = -1;

  // Move.
  struct PolygonAnchor
  {
    PolygonAnchor(const ggo::polygon2d_float & polygon, int x, int y) : _polygon(polygon), _x(x), _y(y) {}

    ggo::polygon2d_float _polygon;
    int _x;
    int _y;
  };
  std::unique_ptr<PolygonAnchor> _polygon_anchor;
};

#endif

