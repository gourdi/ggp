#ifndef __GGO_POINT_LIGHT__
#define __GGO_POINT_LIGHT__

#include <kernel/math/sampling/shape_sampling/ggo_shape_sampling.h>
#include <raytracer/objects3d/ggo_object3d_abc.h>

namespace ggo
{
  class point_light : public object3d_abc
  {
  public:

    point_light(const ggo::pos3_f & pos, const ggo::rgb_32f & color) : _pos(pos), _color(color) {}

  private:

    std::optional<box3d_data_float>   get_bounding_box() const override;
    const object3d_abc *              handle_self_intersection(ggo::ray3d_f & ray) const override;

    // Solid object.
    ggo::rgb_32f                      get_color(const ggo::pos3_f & pos) const override;
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_f & ray) const  override;
    bool                              intersect_segment(const ggo::pos3_f & pos, const ggo::vec3_f & dir, float length) const override;
    ggo::rgb_32f                      process_ray(const ggo::ray3d_f & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const override;
    transmission_data                 compute_transmission(const ggo::ray3d_f & ray, const ggo::ray3d_f & normal, int & depth) const override;

    // Light.
    ggo::rgb_32f                      get_emissive_color() const override;
    ggo::pos3_f                        sample_point(const ggo::pos3_f & target_pos, float random_variable1, float random_variable2) const  override;
    ggo::ray3d_f                  sample_ray(float random_variable1, float random_variable2) const override;
      
    // Volumetric object.
    ggo::rgb_32f                      process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const override;
    ggo::rgb_32f                      process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const override;

  private:

    ggo::pos3_f    _pos;
    ggo::rgb_32f  _color;
  };
}

// Implementation.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  inline ggo::rgb_32f point_light::get_color(const ggo::pos3_f & pos) const
  {
    GGO_FAIL();
    return _color;
  }

  //////////////////////////////////////////////////////////////
  inline ggo::rgb_32f point_light::get_emissive_color() const
  {
    return _color;
  }

  //////////////////////////////////////////////////////////////
  inline const object3d_abc * point_light::handle_self_intersection(ggo::ray3d_f & ray) const
  {
    GGO_FAIL();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  inline ggo::pos3_f point_light::sample_point(const ggo::pos3_f & target_pos, float random_variable1, float random_variable2) const
  {
    return _pos;
  }

  //////////////////////////////////////////////////////////////
  inline ggo::ray3d_f point_light::sample_ray(float random_variable1, float random_variable2) const
  {
    return ggo::ray3d_f(_pos, sphere_uniform_sampling<float>(), false);
  }

  //////////////////////////////////////////////////////////////
  inline std::optional<intersection_data> point_light::intersect_ray(const ggo::ray3d_f & ray) const
  {
    GGO_FAIL();
    return {};
  }

  //////////////////////////////////////////////////////////////
  inline bool point_light::intersect_segment(const ggo::pos3_f & pos, const ggo::vec3_f & dir, float length) const
  {
    GGO_FAIL();
    return false;
  }

  //////////////////////////////////////////////////////////////
  inline ggo::rgb_32f point_light::process_ray(const ggo::ray3d_f & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const
  {
    GGO_FAIL();
    return ggo::black_32f();
  }

  //////////////////////////////////////////////////////////////
  inline transmission_data point_light::compute_transmission(const ggo::ray3d_f & ray, const ggo::ray3d_f & normal, int & depth) const
  {
    GGO_FAIL();
    return transmission_data(transmission_type::internal_error);
  }

  //////////////////////////////////////////////////////////////
  inline std::optional<box3d_data_float> point_light::get_bounding_box() const
  {
    GGO_FAIL();
    return {};
  }

  //////////////////////////////////////////////////////////////
  inline ggo::rgb_32f point_light::process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const
  {
    GGO_FAIL();
    return color;
  }

  //////////////////////////////////////////////////////////////
  inline ggo::rgb_32f point_light::process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const
  {
    GGO_FAIL();
    return color;
  }
}

#endif
