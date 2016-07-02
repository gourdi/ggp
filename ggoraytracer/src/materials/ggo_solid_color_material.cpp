#include "ggo_solid_color_material.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material()
  :
  _color(ggo::color::BLACK)
  {
  }

  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material(const ggo::color & color)
  :
  _color(color)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color solid_color_material::get_color(const ggo::pos3f & pos) const
  {
    return _color;
  }

  //////////////////////////////////////////////////////////////
  void solid_color_material::set_color(const ggo::color & color)
  {
    _color = color;
  }
}
