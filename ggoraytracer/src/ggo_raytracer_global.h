#ifndef __GGO_RAYTRACER_GLOBAL__
#define __GGO_RAYTRACER_GLOBAL__

#include <ggo_raycaster_abc.h>
#include <ggo_color.h>
#include <ggo_shapes3d.h>

namespace ggo
{
  class indirect_lighting_abc;

  //////////////////////////////////////////////////////////////
  struct raytrace_params
  {
    int						                      _depth = 3;
    int                                 _threads_count = 1;
    const ggo::raycaster_abc *          _raycaster = nullptr;
    const ggo::indirect_lighting_abc *  _indirect_lighting = nullptr;
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
    float _dist;
    ggo::ray3d_float _local_normal;
    ggo::ray3d_float _world_normal;
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
