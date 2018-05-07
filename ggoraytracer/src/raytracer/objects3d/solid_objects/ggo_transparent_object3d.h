#ifndef __GGO_TRANSPARENT_OBJECT3D__
#define __GGO_TRANSPARENT_OBJECT3D__

#include <raytracer/objects3d/solid_objects/ggo_reflection_object3d_abc.h>

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class transparent_object3d : public reflection_object3d_abc<flags, shape_t>
  {
  public:

    transparent_object3d(const shape_t & shape, const ggo::color_32f & color, float density) : reflection_object3d_abc<flags, shape_t>(shape), _color(color), _density(density) {}

    static float                            compute_reflection_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density);
    static std::optional<ggo::ray3d_float>  transmit_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float current_density, float next_density);

  private:

    // 'object3d_abc' interface.
    ggo::color_32f    get_color(const ggo::pos3f & pos) const override;
    ggo::color_32f    get_emissive_color() const override { return ggo::black_32f(); }
    ggo::color_32f    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const override;
    transmission_data compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const override;

    ggo::color_32f    compute_transmitted_color(ggo::ray3d_float ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const;

  private:

    ggo::color_32f  _color;
    float _density;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f transparent_object3d<flags, shape_t>::get_color(const ggo::pos3f & pos) const
  {
    GGO_FAIL();
    return ggo::black_32f();
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  float transparent_object3d<flags, shape_t>::compute_reflection_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density)
  {
    float cos_input = ggo::dot(ray.dir(), world_normal.dir());
    float num = current_density - next_density;
    float den = current_density + next_density;
    float parallel_reflection_factor = ggo::square(num / den);
    float reflection_factor = parallel_reflection_factor + (1 - parallel_reflection_factor) * std::pow(1.f + cos_input, 5.f);
    GGO_ASSERT_BTW(reflection_factor, -0.001f, 1.001f);

    return reflection_factor;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  std::optional<ggo::ray3d_float> transparent_object3d<flags, shape_t>::transmit_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float current_density, float next_density)
  {
    float cos_input = ggo::dot(ray.dir(), normal.dir());
    GGO_ASSERT(cos_input <= 0.001f); // Expected negative dot product.

                                     // Snell-Descartes's law.
    float sin_input = std::sqrt(1 - cos_input * cos_input);
    float sin_output = sin_input * current_density / next_density;

    // The input ray is below the incidence angle => full reflection.
    if (sin_output >= 1.f || sin_output <= -1.f)
    {
      return {};
    }

    // Transmission: the ray passes thgough.
    ggo::vec3f parallel_dir = ray.dir() - cos_input * normal.dir();
    parallel_dir.set_length(sin_output);

    float cos_output = std::sqrt(1 - ggo::square(sin_output));
    ggo::vec3f orthogonal_dir = -normal.dir();
    orthogonal_dir.set_length(cos_output);

    ggo::ray3d_float transmitted_ray;
    transmitted_ray.set_dir(parallel_dir + orthogonal_dir);
    transmitted_ray.pos() = normal.pos() + 0.001f * transmitted_ray.dir(); // Avoid self-intersection.

    GGO_ASSERT(transmitted_ray.is_normalized());
    GGO_ASSERT_LE(ggo::dot(normal.dir(), transmitted_ray.dir()), 0.001f);

    return transmitted_ray;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::transmission_data transparent_object3d<flags, shape_t>::compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const
  {
    // Try to transmit the ray into the current object.
    auto inner_ray = transmit_ray(ray, normal, 1.0f, _density);

    // No transmission (because the ray is reflected).
    if (!inner_ray)
    {
      return transmission_data(transmission_type::full_reflection);
    }

    // From world to local coordinates.
    if constexpr(!(flags & discard_basis))
    {
      inner_ray = _basis.ray_from_world_to_local(inner_ray);
    }

    // Bounce ray internally until it leaves the current object.
    while (true)
    {
      depth -= 1;
      if (depth <= 0)
      {
        return transmission_data(transmission_type::out_of_recursion);
      }

      // Find intersection between the current ray and the current object.
      float dist = -1.f;
      ggo::ray3d_float normal;

      if (_shape.intersect_ray(*inner_ray, dist, normal) == false)
      {
        GGO_FAIL();
        return transmission_data(transmission_type::internal_error);
      }

      // Does the ray get out of the object?
      auto outter_ray = transmit_ray(*inner_ray, normal, _density, 1.f);
      if (outter_ray)
      {
        // From local to world coordinates.
        if constexpr(!(flags & discard_basis))
        {
          outter_ray = _basis.ray_from_local_to_world(*outter_ray);
        }
        return transmission_data(*outter_ray);
      }

      // Internal reflection.
      ggo::vec3f reflected_dir(inner_ray->dir() - 2 * ggo::dot(normal.dir(), inner_ray->dir()) * normal.dir());
      GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
      GGO_ASSERT(ggo::dot(reflected_dir, normal.dir()) >= -0.001f); // Because of rounding errors, the dot product can be a little bit negative.

      inner_ray = ggo::ray3d_float(normal.pos(), reflected_dir, false);

      GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
      GGO_ASSERT_GE(ggo::dot(reflected_dir, normal.dir()), -0.001f);
    }

    GGO_FAIL();
    return transmission_data(transmission_type::internal_error);
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f transparent_object3d<flags, shape_t>::compute_transmitted_color(ggo::ray3d_float ray,
    const intersection_data & intersection,
    const ggo::raytracer & raytracer,
    int depth,
    const ggo::indirect_lighting_abc * indirect_lighting,
    float random_variable1,
    float random_variable2) const
  {
    auto transmission = compute_transmission(ray, intersection._world_normal, depth);

    switch (transmission._type)
    {
    default:
      GGO_FAIL();
      return ggo::black_32f();
    case transmission_type::internal_error:
    case transmission_type::out_of_recursion:
      return ggo::black_32f();

    case transmission_type::full_reflection:
    {
      ray3d_float reflection_ray = get_reflected_ray(ray, intersection._world_normal, random_variable1, random_variable2);

      const object3d_abc * exclude_object = handle_self_intersection(reflection_ray);

      return raytracer.process(reflection_ray, depth - 1, indirect_lighting, random_variable1, random_variable2, exclude_object);
    }

    case transmission_type::partial_transmission:
    {
      ggo::ray3d_float transmitted_ray(transmission._ray);
      const object3d_abc * exclude_object = handle_self_intersection(transmitted_ray);

      // Compute transmitted ray and color by bouncing ray internally until it leaves the current object.
      const ggo::color_32f transmitted_color = raytracer.process(transmitted_ray, depth - 1, indirect_lighting, random_variable1, random_variable2, exclude_object);

      // Compute reflected ray.
      const ggo::ray3d_float & reflected_ray = get_reflected_ray(ray, intersection._world_normal, random_variable1, random_variable2);
      const ggo::color_32f reflected_color = raytracer.process(reflected_ray, depth - 1, indirect_lighting, random_variable1, random_variable2, exclude_object);

      // Compute final color.
      const float reflection_factor = compute_reflection_factor(ray, intersection._world_normal, 1.f, _density);

      return reflection_factor * reflected_color + (1 - reflection_factor) * transmitted_color;
    }
    }
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f transparent_object3d<flags, shape_t>::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const
  {
    auto light_samples = raytracer.sample_lights(intersection._world_normal, this, random_variable1, random_variable2);

    ggo::color_32f output_color = compute_transmitted_color(ray, intersection, raytracer, depth, indirect_lighting, random_variable1, random_variable2);

    if constexpr(!(flags & discard_reflection))
    {
      output_color += compute_reflection_color(ray, intersection._world_normal, raytracer, depth, indirect_lighting, random_variable1, random_variable2);
    }

    if constexpr(!(flags & discard_phong))
    {
      output_color += compute_phong_color(ray, light_samples, intersection);
    }

    return output_color;
  }
}
#endif
