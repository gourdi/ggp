#ifndef __GGO_SHAPE_OBJECT3D__
#define __GGO_SHAPE_OBJECT3D__

#include <ggo_object3d_abc.h>

// Basis + Shape.

namespace ggo
{
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
}

// Implementation.

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

#endif
