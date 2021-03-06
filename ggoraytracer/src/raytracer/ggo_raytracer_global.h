#ifndef __GGO_RAYTRACER_GLOBAL__
#define __GGO_RAYTRACER_GLOBAL__

#include <2d/ggo_pixel_type.h>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

namespace ggo
{
  class indirect_lighting_abc;
  class raycaster_abc;
  class object3d_abc;

  //////////////////////////////////////////////////////////////
  struct raytrace_params
  {
    int						                _depth = 3;
    int                           _threads_count = 1;
    const raycaster_abc *         _raycaster = nullptr;
    const indirect_lighting_abc * _indirect_lighting = nullptr;
    ggo::pixel_type               _output_pixel_type = ggo::pixel_type::rgb_8u;
    ggo::lines_order              _output_memory_lines_order = ggo::lines_order::up;
  };

  //////////////////////////////////////////////////////////////
  struct light_sample
  {
    light_sample(const ggo::ray3d_f &ray_to_light, const ggo::rgb_32f & color) : _ray_to_light(ray_to_light), _color(color) {}

    ggo::ray3d_f  _ray_to_light;
    ggo::rgb_32f      _color;
  };

  //////////////////////////////////////////////////////////////
  struct intersection_data
  {
    intersection_data(float dist, const ggo::ray3d_f & local_normal, const ggo::ray3d_f & world_normal)
      :
      _dist(dist), _local_normal(local_normal), _world_normal(world_normal)
    {
    }

    float _dist;
    ggo::ray3d_f _local_normal;
    ggo::ray3d_f _world_normal;
  };

  //////////////////////////////////////////////////////////////
  struct hit_data
  {
    hit_data(const object3d_abc * object, float dist, const ggo::ray3d_f & local_normal, const ggo::ray3d_f & world_normal)
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
    transmission_data(const ggo::ray3d_f & transmitted_ray) : _type(transmission_type::partial_transmission), _ray(transmitted_ray) {}

    transmission_type _type;
    ggo::ray3d_f  _ray;
  };
}

#endif
