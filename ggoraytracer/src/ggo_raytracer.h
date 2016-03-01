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

    static ggo::color process(const ggo::ray3d_float & ray,
                              const ggo::scene & scene,
                              const ggo::raytrace_params & raytrace_params);

    static ggo::color process(const ggo::ray3d_float & ray,
                              const ggo::scene & scene,
                              const ggo::raytrace_params & raytrace_params,
                              float random_variable1,
                              float random_variable2);

    //////////////////////////////////////////////////////////////
    // Transmission/reflection.

    // Returns 'false' in case the ray is below the incidence angle and gets reflected.
    static bool transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float current_density, float next_density);

    static float compute_reflection_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density);

    //////////////////////////////////////////////////////////////
    // Shadings.
    static ggo::color diffuse_shading(const ggo::color & object_color,
                                      const ggo::color & light_color,
                                      const ggo::ray3d_float & world_normal,
                                      const ggo::ray3d_float & ray_to_light);

    static ggo::color specular_shading(float phong_factor,
                                       float phong_shininess,
                                       const ggo::color & light_color,
                                       const ggo::ray3d_float & ray,
                                       const ggo::ray3d_float & world_normal,
                                       const ggo::ray3d_float & ray_to_light);
  };
}

#endif

