#ifndef __GGO_POINT_LIGHT__
#define __GGO_POINT_LIGHT__

#include <ggo_object3d_abc.h>
#include <ggo_shape_sampling.h>

namespace ggo
{
  class point_light : public object3d_abc
  {
  public:

    point_light(const ggo::pos3f & pos, const ggo::color_32f & color) : _pos(pos), _color(color) {}

  private:

    ggo::color_32f                    get_color(const ggo::pos3f & pos) const override;
    ggo::color_32f                    get_emissive_color() const override;
    const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray) const override;
    ggo::pos3f                        sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const  override;
    ggo::ray3d_float                  sample_ray(float random_variable1, float random_variable2) const override;
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const  override;
    ggo::color_32f                    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;

  private:

    ggo::pos3f      _pos;
    ggo::color_32f  _color;
  };
}

// Implementation.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::color_32f point_light::get_color(const ggo::pos3f & pos) const
  {
    GGO_FAIL();
    return _color;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f point_light::get_emissive_color() const
  {
    return _color;
  }

  //////////////////////////////////////////////////////////////
  const object3d_abc * point_light::handle_self_intersection(ggo::ray3d_float & ray) const
  {
    GGO_FAIL();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  ggo::pos3f point_light::sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const
  {
    return _pos;
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float point_light::sample_ray(float random_variable1, float random_variable2) const
  {
    return ggo::ray3d_float(_pos, sphere_uniform_sampling<float>(), false);
  }

  //////////////////////////////////////////////////////////////
  std::optional<intersection_data> point_light::intersect_ray(const ggo::ray3d_float & ray) const
  {
    GGO_FAIL();
    return {};
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f point_light::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    GGO_FAIL();
    return ggo::black_32f();
  }
}

#endif
