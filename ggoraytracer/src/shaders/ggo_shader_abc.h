#ifndef __GGO_SHADER_ABC__
#define __GGO_SHADER_ABC__

#include <ggo_color.h>
#include <ggo_shapes3d.h>

namespace ggo
{
  class shader_abc
  {
  public:

    virtual ggo::color shade(const ggo::color & object_color,
                             const ggo::color & light_color,
                             const ggo::ray3d_float & ray,
                             const ggo::ray3d_float & world_normal,
                             const ggo::ray3d_float & ray_to_light) const = 0;
  };
}

#endif

