#ifndef __GGO_SIMPLE_COLOR_OBJECT3D__
#define __GGO_SIMPLE_COLOR_OBJECT3D__

#include <ggo_shape_object3d.h>

namespace ggo
{
  template <uint32_t flags, typename shape_t, typename material_t>
  class simple_color_object3d : public shape_object3d_abc<discard_basis, shape_t>
  {
  public:

    simple_color_object3d(const shape_t & shape, const material_t & material) : shape_object3d_abc<flags, shape_t>(shape), _material(material) {}

  private:

    ggo::color_32f    get_color(const ggo::pos3f & pos) const;
    ggo::color_32f    get_emissive_color() const override;
    ggo::color_32f    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;
    transmission_data compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const override;

  private:

    material_t  _material;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  ggo::color_32f simple_color_object3d<flags, shape_t, material_t>::get_emissive_color() const
  {
    return ggo::black_32f();
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  ggo::color_32f  simple_color_object3d<flags, shape_t, material_t>::get_color(const ggo::pos3f & pos) const
  {
    GGO_FAIL();
    return ggo::black_32f();
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  ggo::color_32f simple_color_object3d<flags, shape_t, material_t>::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    return _material.get_color(ray.pos());
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  transmission_data simple_color_object3d<flags, shape_t, material_t>::compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const
  {
    GGO_FAIL();
    return transmission_data(transmission_type::internal_error);
  }
}

#endif