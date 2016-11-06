#include "ggo_solid_color_material.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material()
  :
  _color(ggo::black<ggo::color_32f>())
  {
  }

  //////////////////////////////////////////////////////////////
  solid_color_material::solid_color_material(const ggo::color_32f & color)
  :
  _color(color)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f solid_color_material::get_color(const ggo::pos3f & pos) const
  {
    return _color;
  }

  //////////////////////////////////////////////////////////////
  void solid_color_material::set_color(const ggo::color_32f & color)
  {
    _color = color;
  }
}
