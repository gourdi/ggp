#include "ggo_checker_material.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  checker_material_abc::checker_material_abc(const ggo::rgb_32f & color1, const ggo::rgb_32f & color2, float tile_size)
  :
  _color1(color1),
  _color2(color2),
  _tile_size(tile_size)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f checker_3d_material::get_color(const ggo::pos3f & pos) const
  {
    int x = ggo::round_to<int>(pos.get<0>() / _tile_size);
    int y = ggo::round_to<int>(pos.get<1>() / _tile_size);
    int z = ggo::round_to<int>(pos.get<2>() / _tile_size);
    int i = x + y + z;

    return i % 2 ? _color1 : _color2;
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f checker_xy_material::get_color(const ggo::pos3f & pos) const
  {
    int x = ggo::round_to<int>(pos.get<0>() / _tile_size);
    int y = ggo::round_to<int>(pos.get<1>() / _tile_size);
    int i = x + y;

    return i % 2 ? _color1 : _color2;
  }
}
