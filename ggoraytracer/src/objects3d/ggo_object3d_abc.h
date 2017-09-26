#ifndef __GGO_OBJECT3D_ABC__
#define __GGO_OBJECT3D_ABC__

#include <ggo_shapes3d.h>
#include <ggo_color.h>
#include <ggo_raytracer_global.h>
#include <ggo_raytracer.h>

// object3d_abc
//  |
//  +--> glow
//  |
//  +--> point_light
//  |
//  +--> shape_object3d_abc
//          |
//          +--> simple_color_object3d
//          |
//          +--> reflection_object3d_abc
//                  |
//                  +--> diffuse_object3d
//                  |
//                  +--> transparent_object3d
//                  |
//                  +--> shape_light

namespace ggo
{
  constexpr uint32_t no_flags = 0;
  constexpr uint32_t discard_basis = 1 << 0;
  constexpr uint32_t discard_roughness = 1 << 1;
  constexpr uint32_t discard_phong = 1 << 2;
  constexpr uint32_t discard_reflection = 1 << 3;

  //////////////////////////////////////////////////////////////
  // Abstract base class.
  class object3d_abc
  {
  public:

    virtual ggo::color_32f                    get_emissive_color() const = 0;
    virtual const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray, bool inside) const = 0;
    virtual ggo::pos3f                        sample_shape(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const = 0;
    virtual std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const = 0;
    virtual ggo::color_32f                    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const = 0;
  };

  //////////////////////////////////////////////////////////////
  // Basis + Shape.
  template <uint32_t flags, typename shape_t>
  class shape_object3d_abc : public object3d_abc
  {
  public:

                          shape_object3d_abc(const shape_t & shape) : _shape(shape) {}

    ggo::basis3d_float &  basis() { return _basis; }

  private:

    const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray, bool inside) const override;
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const override;
    ggo::pos3f                        sample_shape(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const override;

  private:

    shape_t             _shape;
    ggo::basis3d_float  _basis;
  };

  //////////////////////////////////////////////////////////////
  // Basis + Shape + Reflection.
  template <uint32_t flags, typename shape_t>
  class reflection_object3d_abc : public shape_object3d_abc<flags, shape_t>
  {
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

// shape_object3d_abc
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  std::optional<intersection_data> shape_object3d_abc<flags, shape_t>::intersect_ray(const ggo::ray3d_float & ray) const
  {
    intersection_data intersection;

    if constexpr(flags & discard_basis)
    {
      if (_shape.intersect_ray(ray, intersection._dist, intersection._local_normal) == false)
      {
        return {};
      }

      intersection._world_normal = intersection._local_normal;
    }
    else
    {
      if (_shape.intersect_ray(_basis.ray_from_world_to_local(ray), intersection._dist, intersection._local_normal) == false)
      {
        return {};
      }

      intersection._world_normal = _basis.ray_from_local_to_world(intersection._local_normal);
    }

    return intersection;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  const object3d_abc * shape_object3d_abc<flags, shape_t>::handle_self_intersection(ggo::ray3d_float & ray, bool inside) const
  {
    if (inside == false && _shape.is_convex() == true)
    {
      return this;
    }

    ray.pos() += 0.001f * ray.dir();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::pos3f shape_object3d_abc<flags, shape_t>::sample_shape(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const
  {
    if constexpr(flags & discard_basis)
    {
      return _shape.sample_point(target_pos, random_variable1, random_variable2);
    }
    else
    {
      ggo::pos3f point = _shape.sample_point(_basis.point_from_world_to_local(target_pos), random_variable1, random_variable2);

      return _basis.point_from_local_to_world(point);
    }
  }
}

// reflection_object3d_abc
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