#include "ggo_checker_material.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  checker_material_abc::checker_material_abc(const ggo::color & color1, const ggo::color & color2, float tile_size)
  :
  _color1(color1),
  _color2(color2),
  _tile_size(tile_size)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color checker_3d_material::get_color(const ggo::point3d_float & pos) const
  {
    int x = ggo::to<int>(pos.x() / _tile_size);
    int y = ggo::to<int>(pos.y() / _tile_size);
    int z = ggo::to<int>(pos.z() / _tile_size);
    int i = x + y + z;

    return i % 2 ? _color1 : _color2;
  }

  //////////////////////////////////////////////////////////////
  ggo::color checker_xy_material::get_color(const ggo::point3d_float & pos) const
  {
    int x = ggo::to<int>(pos.x() / _tile_size);
    int y = ggo::to<int>(pos.y() / _tile_size);
    int i = x + y;

    return i % 2 ? _color1 : _color2;
  }
}
