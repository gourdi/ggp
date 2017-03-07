#include "PolygonFactory.h"
#include "PolygonHandler.h"

//////////////////////////////////////////////////////////////
ShapeHandler * PolygonFactory::OnMouseDown(Qt::MouseButton, int, int, int, int, ggo::canvas &, const ggo::canvas::view &)
{
  return nullptr;
}

//////////////////////////////////////////////////////////////
ShapeHandler * PolygonFactory::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view)
{
  switch (button)
  {
  case Qt::LeftButton:
    if (_polygon == nullptr)
    {
      _polygon = canvas.create_polygon();
      _polygon->get_polygon().add_point(ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height, true));
    }

    _polygon->get_polygon().add_point(ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height, true));
    break;

  case Qt::RightButton:
    if (_polygon->get_polygon().empty()  == false)
    {
      _polygon->get_polygon().pop_back();
    }
    return new PolygonHandler(_polygon);
  }

  GGO_FAIL();
  return nullptr;
}

//////////////////////////////////////////////////////////////
void PolygonFactory::OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view)
{
  if (_polygon != nullptr && _polygon->get_polygon().empty() == false)
  {
    _polygon->get_polygon().back() = ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height, true);
  }
}

