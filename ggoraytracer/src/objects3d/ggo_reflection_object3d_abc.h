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

    void set_phong(float phong_factor, float phong_shininess) { _phong_factor = phong_factor; _phong_shininess = phong_shininess; }

  protected:

    reflection_object3d_abc(const shape_t & shape) : shape_object3d_abc<flags, shape_t>(shape) {};

    ggo::color_32f  compute_reflection_color() const;
    ggo::color_32f  compute_phong_color(const ggo::ray3d_float & ray, const std::vector<ggo::light_sample> & light_samples, const intersection_data & intersection) const;

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
  ggo::color_32f reflection_object3d_abc<flags, shape_t>::compute_reflection_color() const
  {
#if 0
    // Handle self-intersection.
    ggo::ray3d_float safe_ray(ray);
    const ggo::object3d * exclude_object = nullptr;
    if (previous_hit_object != nullptr)
    {
      exclude_object = previous_hit_object->handle_self_intersection(safe_ray, false);
    }
#endif


    return ggo::black_32f();
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
