#include "DiscFactory.h"

//////////////////////////////////////////////////////////////
void DiscFactory::OnMouseDown(Qt::MouseButton, int, int, int, int, ggo::canvas &)
{
  // Do nothing.
}

//////////////////////////////////////////////////////////////
void DiscFactory::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas)
{
  if (button == Qt::LeftButton)
  {
    switch (_state)
    {
    case None:
    {
      _disc = canvas.create_disc();
      _disc->center() = ggo::canvas::from_render_pixel_to_canvas({ x, y }, ggo::canvas::main_direction::vertical, width, height);

      _state = SettingRadius;
      break;
    }
    case SettingRadius:
      if (_disc->radius() <= 0)
      {
        canvas.remove_shape(_disc);
      }
      _state = None;
      break;
    }
  }
}

//////////////////////////////////////////////////////////////
bool DiscFactory::OnMouseMove(int x, int y, int width, int height, ggo::canvas &)
{
  switch (_state)
  {
  case None:
    break;
  case SettingRadius:
    const ggo::pos2f p = ggo::canvas::from_render_pixel_to_canvas({ x, y }, ggo::canvas::main_direction::vertical, width, height);
    _disc->set_radius(ggo::distance(_disc->center(), p));
    return true;
  }

  return false;
}

