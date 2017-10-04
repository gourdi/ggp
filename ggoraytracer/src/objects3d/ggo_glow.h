#ifndef __GGO_GLOW__
#define __GGO_GLOW__

#include <ggo_object3d_abc.h>
#include <ggo_ease.h>

namespace ggo
{
  class glow : public object3d_abc
  {
  public:

    glow(const ggo::pos3f & pos, float opacity, const ggo::color_32f & color, float inner_radius, float outter_radius);

  private:

    ggo::color_32f                    get_color(const ggo::pos3f & pos) const override { GGO_FAIL();  return ggo::black_32f(); }
    ggo::color_32f                    get_emissive_color() const override { return ggo::black_32f(); }
    const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray) const override { return this; }
    ggo::pos3f                        sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const override { GGO_FAIL(); return _pos; }
    ggo::ray3d_float                  sample_ray(float random_variable1, float random_variable2) const override { GGO_FAIL(); return ray3d_float::O_Z(); }
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const override;
    ggo::color_32f                    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;
    transmission_data                 compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const override { GGO_FAIL(); return transmission_data(transmission_type::internal_error); }

  private:

    ggo::pos3f      _pos;
    float           _opacity;
    ggo::color_32f  _color;
    float           _inner_radius;
    float           _outter_radius;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  inline glow::glow(const ggo::pos3f & pos, float opacity, const ggo::color_32f & color, float inner_radius, float outter_radius)
  :
  _pos(pos),
  _opacity(opacity),
  _color(color),
  _inner_radius(inner_radius),
  _outter_radius(outter_radius)
  {
  }

  //////////////////////////////////////////////////////////////
  inline std::optional<intersection_data> glow::intersect_ray(const ggo::ray3d_float & ray) const
  {
    ray3d_float normal(_pos, -ray.dir(), false);
    disc3d_float disc(normal, _outter_radius);

    float dist = -1.f;
    if (disc.intersect_ray(ray, dist, normal) == true)
    {
      return intersection_data(dist, normal, normal);
    }

    return {};
  }

  //////////////////////////////////////////////////////////////
  inline ggo::color_32f glow::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    const float dist = ggo::distance(intersection._world_normal.pos(), _pos);
    GGO_ASSERT(dist <= _outter_radius + 0.001f);

    const float opacity = dist < _inner_radius ? _opacity : _opacity * ggo::ease_inout((_outter_radius - dist) / (_outter_radius - _inner_radius));
    const ggo::ray3d_float output_ray(intersection._world_normal.pos(), ray.dir(), false);

    // Glow does not consume raytracing recursion.  
    return opacity * _color + (1 - opacity) * raytracer.process(output_ray, depth, random_variable1, random_variable2, this);
  }
}

#endif
