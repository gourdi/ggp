#ifndef __GGO_MATERIAL_ABC__
#define __GGO_MATERIAL_ABC__

#include <ggo_vec.h>
#include <ggo_color.h>

namespace ggo
{
  class material_abc
  {
  public:
    
    virtual                ~material_abc() {}
      
    virtual	ggo::color_32f  get_color(const ggo::pos3f & pos) const = 0;
  };
}

#endif
