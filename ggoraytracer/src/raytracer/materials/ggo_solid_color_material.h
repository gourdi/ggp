#ifndef __GGO_SOLID_COLOR_MATERIAL__
#define __GGO_SOLID_COLOR_MATERIAL__

#include <raytracer/materials/ggo_material_abc.h>

namespace ggo
{
  class solid_color_material : public material_abc
  {
  public:
    
                  solid_color_material();
                  solid_color_material(const ggo::rgb_32f & color);
    
    ggo::rgb_32f  get_color(const ggo::pos3f & pos) const override;
    
    void		      set_color(const ggo::rgb_32f & color);
    
  private:
    
    ggo::rgb_32f	_color;
  };
}

namespace ggo
{
  solid_color_material  white_material();
  solid_color_material  red_material();
  solid_color_material  green_material();
  solid_color_material  blue_material();
  solid_color_material  yellow_material();
}

#endif