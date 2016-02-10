#ifndef __GGO_RAYTRACER__
#define __GGO_RAYTRACER__

#include <ggo_raytracer_global.h>
#include <ggo_object3d.h>
#include <ggo_scene.h>
#include <memory>

// Height is the "reference" dimension, ie. if width and height are not the same,
// computations are made according to the height value.

namespace ggo
{
  class raytracer
  {
  public:

    static ggo::color shade(const ggo::ray3d_float & ray,
                            const ggo::object3d * hit_object,
                            const ggo::color & hit_object_color,
                            const ggo::ray3d_float & world_normal,
                            const ggo::object3d * light,
                            const ggo::point3d_float & light_point,
                            const ggo::scene & scene,
                            const ggo::raycaster_abc * raycaster);

    static ggo::color mono_sampling_raytrace(const ggo::ray3d_float & ray,
                                             const ggo::scene & scene,
                                             const ggo::raytrace_params & raytrace_params);

    // Returns 'false' in case the ray is below the incidence angle and gets reflected.
    static bool transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float current_density, float next_density);

    static float compute_reflexion_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density);

  private:

    static ggo::color mono_sampling_raytrace_recursive(const ggo::ray3d_float & ray,
                                                       const ggo::scene & scene,
                                                       const ggo::raycaster_abc * raycaster,
                                                       int depth,
                                                       const ggo::object3d * previous_hit_object);
  };
}

#endif

