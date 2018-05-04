#ifndef __GGO_SHAPE_OBJECT3D__
#define __GGO_SHAPE_OBJECT3D__

#include <raytracer/objects3d/ggo_object3d_abc.h>

// Basis + Shape.

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class solid_object3d_abc : public object3d_abc
  {
  public:

                          solid_object3d_abc(const shape_t & shape) : _shape(shape) {}

    ggo::basis3d_float &  basis() { return _basis; }

  protected:

    const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray) const override;
    std::optional<box3d_data_float>   get_bounding_box() const;

    // Solid object.
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const override;
    bool                              intersect_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length) const override;

    // Lights.
    ggo::pos3f                        sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const override;
    ggo::ray3d_float                  sample_ray(float random_variable1, float random_variable2) const override;

    // Volumetric object.
    ggo::color_32f                    process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const;
    ggo::color_32f                    process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const;

  protected:

    shape_t             _shape;
    ggo::basis3d_float  _basis;
  };
}

// Implementation.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  std::optional<intersection_data> solid_object3d_abc<flags, shape_t>::intersect_ray(const ggo::ray3d_float & ray) const
  {
    if constexpr(flags & discard_basis)
    {
      float dist = 0.f;
      ggo::ray3d_float normal;
      if (_shape.intersect_ray(ray, dist, normal) == true)
      {
        return intersection_data(dist, normal, normal);
      }
    }
    else
    {
      float dist = 0.f;
      ggo::ray3d_float local_normal;
      if (_shape.intersect_ray(_basis.ray_from_world_to_local(ray), dist, local_normal) == true)
      {
        return intersection_data(dist, local_normal, _basis.ray_from_local_to_world(local_normal));
      }
    }

    return {};
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  bool solid_object3d_abc<flags, shape_t>::intersect_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length) const
  {
    if constexpr(flags & discard_basis)
    {
      return _shape.intersect_segment(pos, dir, length);
    }
    else
    {
      return _shape.intersect_segment(_basis.point_from_world_to_local(pos), _basis.vector_from_world_to_local(dir), length);
    }
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  const object3d_abc * solid_object3d_abc<flags, shape_t>::handle_self_intersection(ggo::ray3d_float & ray) const
  {
    if (_shape.is_convex() == true)
    {
      return this;
    }

    ray.pos() += 0.001f * ray.dir();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::pos3f solid_object3d_abc<flags, shape_t>::sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const
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

  //////////////////////////////////////////////////////////////.
  template <uint32_t flags, typename shape_t>
  ggo::ray3d_float solid_object3d_abc<flags, shape_t>::sample_ray(float random_variable1, float random_variable2) const
  {
    ggo::ray3d_float ray = _shape.sample_ray(random_variable1, random_variable2);

    if constexpr(!(flags & discard_basis))
    {
      _basis.ray_from_local_to_world(ray);
    }

    return ray;
  }

  //////////////////////////////////////////////////////////////.
  template <uint32_t flags, typename shape_t>
  std::optional<box3d_data_float> solid_object3d_abc<flags, shape_t>::get_bounding_box() const
  {
    if constexpr(flags & discard_basis)
    {
      return _shape.get_bounding_box(basis3d_float());
    }
    else
    {
      return _shape.get_bounding_box(_basis);
    }
  }

  //////////////////////////////////////////////////////////////.
  template <uint32_t flags, typename shape_t>
  ggo::color_32f solid_object3d_abc<flags, shape_t>::process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const
  {
    GGO_FAIL();
    return color;
  }

  //////////////////////////////////////////////////////////////.
  template <uint32_t flags, typename shape_t>
  ggo::color_32f solid_object3d_abc<flags, shape_t>::process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const
  {
    GGO_FAIL();
    return color;
  }
}

#endif
