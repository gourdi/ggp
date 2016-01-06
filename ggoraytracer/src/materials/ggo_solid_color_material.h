#ifndef __GGO_SOLID_COLOR_MATERIAL__
#define __GGO_SOLID_COLOR_MATERIAL__

#include <ggo_material_abc.h>

namespace ggo
{
  class solid_color_material : public material_abc
  {
  public:
    
                solid_color_material();
                solid_color_material(const ggo::color & color);
    
    ggo::color	get_color(const ggo::point3d_float & pos) const override;
    
    void		    set_color(const ggo::color & color);
    
  private:
    
    ggo::color	_color;
  };
}

#endif