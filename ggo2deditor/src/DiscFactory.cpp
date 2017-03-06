#include "DiscFactory.h"
#include "DiscHandler.h"

//////////////////////////////////////////////////////////////
ShapeHandler * DiscFactory::OnMouseDown(Qt::MouseButton, int, int, int, int, ggo::canvas &, const ggo::canvas::view &)
{
  // Do nothing.
  return nullptr;
}

//////////////////////////////////////////////////////////////
ShapeHandler * DiscFactory::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view)
{
  if (button == Qt::LeftButton)
  {
    switch (_state)
    {
    case None:
    {
      _disc = canvas.create_disc();
      _disc->get_disc().center() = ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height);

      _state = SettingRadius;
      return nullptr;
    }
    case SettingRadius:
      _state = None;
      return new DiscHandler(_disc);
    }
  }

  return nullptr;
}

//////////////////////////////////////////////////////////////
void DiscFactory::OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view)
{
  switch (_state)
  {
  case None:
    break;
  case SettingRadius:
    const ggo::pos2f p = ggo::canvas::from_render_pixel_to_view({ x, y }, view, width, height);
    _disc->get_disc().set_radius(ggo::distance(_disc->get_disc().center(), p));
    break;
  }
}

