#ifndef __GGO_INDIRECT_LIGHTING_ABC__
#define __GGO_INDIRECT_LIGHTING_ABC__

#include <memory>
#include <raytracer/ggo_scene.h>

namespace ggo
{
  class indirect_lighting_abc
  {
  public:

    virtual ggo::rgb_32f  process(const ggo::ray3d_float & ray,
                                  const ggo::ray3d_float & world_normal,
                                  const ggo::object3d_abc & hit_object,
                                  const ggo::rgb_32f & hit_color,
                                  float random_variable1,
                                  float random_variable2) const = 0;
  };
}

#endif
