#include "ggo_solid_color_material.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material()
  :
  _color(ggo::black_32f())
  {
  }

  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material(const ggo::rgb_32f & color)
  :
  _color(color)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f solid_color_material::get_color(const ggo::pos3_f & pos) const
  {
    return _color;
  }

  //////////////////////////////////////////////////////////////
  void solid_color_material::set_color(const ggo::rgb_32f & color)
  {
    _color = color;
  }
}

namespace ggo
{
  solid_color_material  white_material()
  {
    return solid_color_material(white_32f());
  }

  solid_color_material  red_material()
  {
    return solid_color_material(red_32f());
  }

  solid_color_material  green_material()
  {
    return solid_color_material(green_32f());
  }

  solid_color_material  blue_material()
  {
    return solid_color_material(blue_32f());
  }

  solid_color_material  yellow_material()
  {
    return solid_color_material(yellow_32f());
  }
}
