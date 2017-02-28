#include "PolygonFactory.h"

//////////////////////////////////////////////////////////////
void PolygonFactory::OnMouseDown(Qt::MouseButton, int, int, int, int, ggo::canvas &, const ggo::canvas::view &)
{
}

//////////////////////////////////////////////////////////////
void PolygonFactory::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view)
{
  switch (button)
  {
  case Qt::LeftButton:
    if (_polygon == nullptr)
    {
      _polygon = canvas.create_polygon();
      _polygon->add_point(ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height));
    }

    _polygon->add_point(ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height));
    break;

  case Qt::RightButton:
    _polygon = nullptr;
    break;
  }
}

//////////////////////////////////////////////////////////////
bool PolygonFactory::OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view)
{
  if (_polygon != nullptr)
  {
    _polygon->back() = ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height);
    return true;
  }

  return false;
}

