#ifndef __GGO_RAYTRACER_GLOBAL__
#define __GGO_RAYTRACER_GLOBAL__

#include <2d/ggo_color.h>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

namespace ggo
{
  class indirect_lighting_abc;
  class raycaster_abc;
  class object3d_abc;

  //////////////////////////////////////////////////////////////
  struct raytrace_params
  {
    int						              _depth = 3;
    int                         _threads_count = 1;
    const ggo::raycaster_abc *  _raycaster = nullptr;
  };

  //////////////////////////////////////////////////////////////
  struct light_sample
  {
    light_sample(const ggo::ray3d_float &ray_to_light, const ggo::color_32f & color) : _ray_to_light(ray_to_light), _color(color) {}

    ggo::ray3d_float  _ray_to_light;
    ggo::color_32f    _color;
  };

  //////////////////////////////////////////////////////////////
  struct intersection_data
  {
    intersection_data(float dist, const ggo::ray3d_float & local_normal, const ggo::ray3d_float & world_normal)
      :
      _dist(dist), _local_normal(local_normal), _world_normal(world_normal)
    {
    }

    float _dist;
    ggo::ray3d_float _local_normal;
    ggo::ray3d_float _world_normal;
  };

  //////////////////////////////////////////////////////////////
  struct hit_data
  {
    hit_data(const object3d_abc * object, float dist, const ggo::ray3d_float & local_normal, const ggo::ray3d_float & world_normal)
      :
      _object(object),
      _intersection(dist, local_normal, world_normal)
    {
    }

    hit_data(const object3d_abc * object, const intersection_data & intersection)
      :
      _object(object),
      _intersection(intersection)
    {
    }

    const object3d_abc *  _object;
    intersection_data     _intersection;
  };

  //////////////////////////////////////////////////////////////
  enum class transmission_type
  {
    internal_error,
    full_reflection,
    partial_transmission,
    out_of_recursion
  };

  //////////////////////////////////////////////////////////////
  struct transmission_data
  {
    transmission_data(transmission_type type) : _type(type) {}
    transmission_data(const ggo::ray3d_float & transmitted_ray) : _type(transmission_type::partial_transmission), _ray(transmitted_ray) {}

    transmission_type _type;
    ggo::ray3d_float  _ray;
  };
}

#endif
