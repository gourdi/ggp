#ifndef __GGO_REFLECTION_OBJECT3D__
#define __GGO_REFLECTION_OBJECT3D__

#include <ggo_shape_object3d.h>

// Basis + Shape + Reflection.

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class reflection_object3d_abc : public shape_object3d_abc<flags, shape_t>
  {
  public:

    void set_reflection_factor(float reflection_factor) { _reflection_factor = reflection_factor; }
    void set_roughness(float roughness) { _roughness = roughness;  }

    void set_phong(float phong_factor, float phong_shininess) { _phong_factor = phong_factor; _phong_shininess = phong_shininess; }

  protected:

    reflection_object3d_abc(const shape_t & shape) : shape_object3d_abc<flags, shape_t>(shape) {};

    ggo::ray3d_float  get_reflected_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float random_variable1, float random_variable2) const;

    ggo::color_32f    compute_reflection_color(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const;
    ggo::color_32f    compute_phong_color(const ggo::ray3d_float & ray, const std::vector<ggo::light_sample> & light_samples, const intersection_data & intersection) const;

  private:

    float _reflection_factor = 0;
    float _roughness = 0;
    float _phong_factor = 0;
    float _phong_shininess = 0;
  };
}

// Implementation.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::ray3d_float reflection_object3d_abc<flags, shape_t>::get_reflected_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, float random_variable1, float random_variable2) const
  {
    ggo::pos3f reflected_dir(ray.dir() - 2 * ggo::dot(normal.dir(), ray.dir()) * normal.dir());
    GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
    GGO_ASSERT(ggo::dot(reflected_dir, normal.dir()) >= -0.001f); // Because of rounding errors, the dot product can be a little bit negative.

    if (flags & discard_roughness)
    {
      return ggo::ray3d_float(normal.pos(), reflected_dir, false);
    }
    else
    {
      // Glossy materials.
      if (_roughness > 0)
      {
        reflected_dir = ggo::hemisphere_sampling(reflected_dir, random_variable1, random_variable2, _roughness * ggo::pi<float>() / 2);

        // Handle the case when the reflected ray goes inside the hit object.
        float dot = ggo::dot(normal.dir(), reflected_dir);
        if (dot < 0)
        {
          reflected_dir -= 2.f * dot * normal.dir();
        }
      }

      GGO_ASSERT(reflected_dir.is_normalized(0.001f) == true);
      GGO_ASSERT_GE(ggo::dot(reflected_dir, normal.dir()), -0.001f);

      return ggo::ray3d_float(normal.pos(), reflected_dir, false);
    }
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f reflection_object3d_abc<flags, shape_t>::compute_reflection_color(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    ggo::ray3d_float reflection_ray = get_reflected_ray(ray, normal, random_variable1, random_variable2);
    const object3d_abc * exclude_object = handle_self_intersection(reflection_ray);

    ggo::color_32f reflection_color = raytracer.process(reflection_ray, depth - 1, random_variable1, random_variable2, exclude_object);

    return _reflection_factor * reflection_color;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f reflection_object3d_abc<flags, shape_t>::compute_phong_color(const ggo::ray3d_float & ray, const std::vector<ggo::light_sample> & light_samples, const intersection_data & intersection) const
  {
    ggo::color_32f output_color = ggo::black_32f();

    for (const auto & light_sample : light_samples)
    {
      ggo::vec3f reflection_dir(2 * ggo::dot(intersection._world_normal.dir(), light_sample._ray_to_light.dir()) * intersection._world_normal.dir() - light_sample._ray_to_light.dir());
      GGO_ASSERT(reflection_dir.is_normalized(0.0001f));

      ggo::vec3f viewer_dir(ray.pos() - intersection._world_normal.pos());
      viewer_dir.normalize();

      float phong = ggo::dot(reflection_dir, viewer_dir);
      if (phong > 0)
      {
        float specular = _phong_factor * std::pow(phong, _phong_shininess);
        output_color += light_sample._color * specular;
      }
    }

    return output_color;
  }
}

#endif
