#ifndef __GGO_SHAPE_LIGHT__
#define __GGO_SHAPE_LIGHT__

#include <raytracer/objects3d/ggo_reflection_object3d_abc.h>

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class shape_light : public reflection_object3d_abc<flags, shape_t>
  {
  public:

    shape_light(const shape_t & shape, const ggo::color_32f & color);

  private:

    ggo::color_32f    get_color(const ggo::pos3f & pos) const override;
    ggo::color_32f    get_emissive_color() const override { return _color; }
    ggo::color_32f    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;
    transmission_data compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const override;

  private:

    ggo::color_32f  _color;
  };
}

// Implementation.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  shape_light<flags, shape_t>::shape_light(const shape_t & shape, const ggo::color_32f & color)
    :
    reflection_object3d_abc<flags, shape_t>(shape),
    _color(color)
  {

  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f shape_light<flags, shape_t>::get_color(const ggo::pos3f & pos) const
  {
    GGO_FAIL();
    return _color;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  ggo::color_32f shape_light<flags, shape_t>::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    ggo::color_32f output_color = _color;

    if constexpr(!(flags & discard_reflection))
    {
      output_color += compute_reflection_color();
    }

    if constexpr(!(flags & discard_phong))
    {
      auto light_samples = raytracer.filter_lights(intersection._world_normal, this, random_variable1, random_variable2);
      output_color += compute_phong_color(ray, light_samples, intersection);
    }

    return output_color;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t>
  transmission_data shape_light<flags, shape_t>::compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const
  {
    GGO_FAIL();
    return transmission_data(transmission_type::internal_error);
  }
}

#endif
