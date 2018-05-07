#ifndef __GGO_RAYTRACER__
#define __GGO_RAYTRACER__

// Height is the "reference" dimension, ie. if width and height are not the same,
// computations are made according to the height value.

#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>
#include <raytracer/ggo_raytracer_global.h>
#include <raytracer/raycasters/ggo_brute_force_raycaster.h>

namespace ggo
{
  class scene;
  class object3d_abc;
  class indirect_lighting_abc;
  class raycaster_abc;

  class raytracer
  {
  public:

                              raytracer(const ggo::scene & scene, const raycaster_abc & raycaster);

    ggo::color_32f            process(const ggo::ray3d_float & ray, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2, const ggo::object3d_abc * exclude_object = nullptr) const;

    std::vector<light_sample> sample_lights(const ggo::ray3d_float & world_normal, const ggo::object3d_abc * hit_object, float random_variable1, float random_variable2) const;

  private:

    const ggo::scene &          _scene;
    const ggo::raycaster_abc &  _raycaster;
  };
}

#endif

