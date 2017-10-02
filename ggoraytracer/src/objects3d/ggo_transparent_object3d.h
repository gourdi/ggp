#ifndef __GGO_TRANSPARENT_OBJECT3D__
#define __GGO_TRANSPARENT_OBJECT3D__

#include <ggo_reflection_object3d_abc.h>

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class transparent_object3d : public reflection_object3d_abc<flags, shape_t>
  {
  public:

    transparent_object3d(const shape_t & shape, const ggo::color_32f & color, float density) : reflection_object3d_abc<flags, shape_t>(shape), _color(color), _density(density) {}

  private:

    ggo::color_32f  get_emissive_color() const override { return ggo::black_32f(); }
    ggo::color_32f  process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;

    ggo::color_32f  compute_transmitted_color(ggo::ray3d_float ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const;

    static bool transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float current_density, float next_density);

  private:

    ggo::color_32f  _color;
    float _density;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  bool transparent_object3d<flags, shape_t>::transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float current_density, float next_density)
  {
    float cos_input = ggo::dot(ray.dir(), normal.dir());
    GGO_ASSERT(cos_input <= 0.001f); // Expected negative dot product.

                                     // Snell-Descartes's law.
    float sin_input = std::sqrt(1 - cos_input * cos_input);
    float sin_output = sin_input * current_density / next_density;

    // The input ray is below the incidence angle => full reflection.
    if (sin_output >= 1.f || sin_output <= -1.f)
    {
      return false;
    }

    // Transmission: the ray passes thgough.
    ggo::vec3f parallel_dir = ray.dir() - cos_input * normal.dir();
    parallel_dir.set_length(sin_output);

    float cos_output = std::sqrt(1 - ggo::square(sin_output));
    ggo::vec3f orthogonal_dir = -normal.dir();
    orthogonal_dir.set_length(cos_output);

    ray.set_dir(parallel_dir + orthogonal_dir);
    ray.pos() = normal.pos() + 0.001f * ray.dir();  // Avoid self-intersection.

    GGO_ASSERT(ray.is_normalized());
    GGO_ASSERT_LE(ggo::dot(normal.dir(), ray.dir()), 0.001f);

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f transparent_object3d<flags, shape_t>::compute_transmitted_color(ggo::ray3d_float ray,
                                                                                 const intersection_data & intersection,
                                                                                 const ggo::raytracer & raytracer,
                                                                                 int depth,
                                                                                 float random_variable1,
                                                                                 float random_variable2) const
  {
    // Transmit the ray into the current object.
    if (transmit_ray(ray, intersection._world_normal, 1.0f, _density) == false)
    {
      ray3d_float reflection_ray = get_reflected_ray(ray, intersection._world_normal, random_variable1, random_variable2);

      const object3d_abc * exclude_object = handle_self_intersection(reflection_ray);

      return raytracer.process(reflection_ray, depth - 1, random_variable1, random_variable2, exclude_object);
    }

    // Bounce if ray internally until it leaves the current object.
    while (true)
    {
      depth -= 1;
      if (depth <= 0)
      {
        return ggo::black_32f();
      }

      // Find intersection between the current ray and the current object.
      float dist = -1.f;
      ggo::ray3d_float normal;
      if (_shape.intersect_ray(ray, dist, normal) == false)
      {
        GGO_FAIL();
        return ggo::black_32f();
      }

      // Does the ray get out of the objects?
      if (transmit_ray(ray, normal, _density, 1.f) == true)
      {
        const object3d_abc * exclude_object = handle_self_intersection(ray);

        return raytracer.process(ray, depth - 1, random_variable1, random_variable2, exclude_object);
      }
    }

    GGO_FAIL();
    return ggo::black_32f();
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f transparent_object3d<flags, shape_t>::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    auto light_samples = raytracer.filter_lights(intersection._world_normal, this, random_variable1, random_variable2);

    ggo::color_32f output_color = compute_transmitted_color(ray, intersection, raytracer, depth, random_variable1, random_variable2);

    if constexpr(!(flags & discard_reflection))
    {
      output_color += compute_reflection_color(ray, intersection._world_normal, raytracer, depth, random_variable1, random_variable2);
    }

    if constexpr(!(flags & discard_phong))
    {
      output_color += compute_phong_color(ray, light_samples, intersection);
    }

    return output_color;
  }
}
#endif
