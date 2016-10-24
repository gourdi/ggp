#ifndef __GGO_SOLID_COLOR_MATERIAL__
#define __GGO_SOLID_COLOR_MATERIAL__

#include <ggo_material_abc.h>

namespace ggo
{
  class solid_color_material : public material_abc
  {
  public:
    
                    solid_color_material();
                    solid_color_material(const ggo::color_32f & color);
    
    ggo::color_32f  get_color(const ggo::pos3f & pos) const override;
    
    void		        set_color(const ggo::color_32f & color);
    
  private:
    
    ggo::color_32f	_color;
  };
}

#endif