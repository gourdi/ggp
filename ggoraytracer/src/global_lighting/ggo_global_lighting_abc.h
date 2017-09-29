#ifndef __GGO_GLOBAL_LIGHTING_ABC__
#define __GGO_GLOBAL_LIGHTING_ABC__

#include <memory>
#include <ggo_scene.h>

namespace ggo
{
  class global_lighting_abc
  {
  public:

    virtual ggo::color_32f  process(const ggo::ray3d_float & ray,
                                    const ggo::ray3d_float & world_normal,
                                    const ggo::object3d_abc & hit_object,
                                    const ggo::color_32f & hit_color,
                                    float random_variable1,
                                    float random_variable2) const = 0;
  };
}

#endif