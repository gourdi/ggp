#ifndef __GGO_CHECKER_MATERIAL__
#define __GGO_CHECKER_MATERIAL__

#include <ggo_material_abc.h>

namespace ggo
{
  class checker_material_abc : public material_abc
  {
  public:

          checker_material_abc(const ggo::color_32f & color1, const ggo::color_32f & color2, float tile_size);
          
    void	set_color1(const ggo::color_32f & color) { _color1 = color; };
    void	set_color2(const ggo::color_32f & color) { _color2 = color; };
    void	set_tile_size(float size) { _tile_size = size; };
    
  protected:
    
    ggo::color_32f	_color1;
    ggo::color_32f	_color2;
    float		        _tile_size;
  };

  class checker_3d_material : public checker_material_abc
  {
  public:

                    checker_3d_material(const ggo::color_32f & color1, const ggo::color_32f & color2, float tile_size) : checker_material_abc(color1, color2, tile_size) {}

    ggo::color_32f  get_color(const ggo::pos3f & pos) const override;
  };

  class checker_xy_material : public checker_material_abc
  {
  public:

                    checker_xy_material(const ggo::color_32f & color1, const ggo::color_32f & color2, float tile_size) : checker_material_abc(color1, color2, tile_size) {}

    ggo::color_32f	get_color(const ggo::pos3f & pos) const override;
  };
}

#endif