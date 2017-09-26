#ifndef __GGO_SHAPE_LIGHT__
#define __GGO_SHAPE_LIGHT__

namespace ggo
{
  template <uint32_t flags, typename shape_t>
  class shape_light : public reflection_object3d_abc<flags, shape_t>
  {
  public:

    shape_light(const shape_t & shape, const ggo::color_32f & color);

  private:

    ggo::color_32f get_emissive_color() const override { return _color; }
    ggo::color_32f process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;

  private:

    ggo::color_32f  _color;
  };
}

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
}

#endif
