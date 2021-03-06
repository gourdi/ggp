#ifndef __GGO_MATERIAL_ABC__
#define __GGO_MATERIAL_ABC__

#include <kernel/ggo_vec3.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class material_abc
  {
  public:
    
    virtual              ~material_abc() {}
      
    virtual	ggo::rgb_32f  get_color(const ggo::pos3_f & pos) const = 0;
  };
}

#endif
