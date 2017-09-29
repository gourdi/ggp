#ifndef __GGO_RAYTRACER__
#define __GGO_RAYTRACER__

// Height is the "reference" dimension, ie. if width and height are not the same,
// computations are made according to the height value.

#include <ggo_shapes3d.h>
#include <ggo_color.h>
#include <ggo_raytracer_global.h>

namespace ggo
{
  class scene;
  class object3d_abc;

  class raytracer
  {
  public:

    raytracer(const ggo::scene & scene, const ggo::raycaster_abc & raycaster);

    ggo::color_32f process(const ggo::ray3d_float & ray, int depth, float random_variable1, float random_variable2, const ggo::object3d_abc * exclude_object = nullptr) const;

    std::vector<light_sample> filter_lights(const ggo::ray3d_float & world_normal, const ggo::object3d_abc * hit_object, float random_variable1, float random_variable2) const;

  private:

    const ggo::scene &          _scene;
    const ggo::raycaster_abc &  _raycaster;


#if 0
    //////////////////////////////////////////////////////////////
    // Transmission/reflection.

    // Returns 'false' in case the ray is below the incidence angle and gets reflected.
    static bool transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float current_density, float next_density);

    static float compute_reflection_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density);

    //////////////////////////////////////////////////////////////
    // Shadings.
    static ggo::color_32f diffuse_shading(const ggo::color_32f & object_color,
                                          const ggo::color_32f & light_color,
                                          const ggo::ray3d_float & world_normal,
                                          const ggo::ray3d_float & ray_to_light);

    static ggo::color_32f specular_shading(float phong_factor,
                                           float phong_shininess,
                                           const ggo::color_32f & light_color,
                                           const ggo::ray3d_float & ray,
                                           const ggo::ray3d_float & world_normal,
                                           const ggo::ray3d_float & ray_to_light);
#endif
  };
}

#endif

