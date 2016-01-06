#ifndef __GGO_MATERIAL_ABC__
#define __GGO_MATERIAL_ABC__

#include <ggo_set3.h>
#include <ggo_color.h>

namespace ggo
{
  class material_abc
  {
  public:
    
    virtual            ~material_abc() {}
      
    virtual	ggo::color	get_color(const ggo::point3d_float & pos) const = 0;
  };
}

#endif
