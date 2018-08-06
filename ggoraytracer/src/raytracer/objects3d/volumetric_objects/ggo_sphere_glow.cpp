#include "ggo_sphere_glow.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  sphere_glow::sphere_glow(const ggo::pos3f & pos, float radius, float opacity, const ggo::rgb_32f & color)
    :_sphere(pos, radius), _opacity(opacity), _color(color)
  {

  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f sphere_glow::process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::rgb_32f & color) const
  {
    float dist = distance(p1, p2);
    ggo::vec3f dir = (p2 - p1) / dist;

    ggo::line3d<float> line(p1, dir, false);

    float dist_inf = 0.f;
    float dist_sup = 0.f;
    if (_sphere.intersect_line(line, dist_inf, dist_sup) == false)
    {
      return color;
    }

    if (dist_sup <= 0.f)
    {
      return color;
    }
    if (dist_inf > dist)
    {
      return color;
    }

    dist_inf = std::max(dist_inf, 0.f);
    dist_sup = std::min(dist_sup, dist);

    float weight = _opacity * (dist_sup - dist_inf) / (2.f * _sphere.radius());

    return weight * _color + (1.f - weight) * color;
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f sphere_glow::process_background_ray(const ggo::ray3d_float & ray, const ggo::rgb_32f & color) const
  {
    ggo::line3d<float> line(ray.pos(), ray.dir(), false);

    float dist_inf = 0.f;
    float dist_sup = 0.f;
    if (_sphere.intersect_line(line, dist_inf, dist_sup) == false)
    {
      return color;
    }

    if (dist_sup <= 0.f)
    {
      return color;
    }

    dist_inf = std::max(dist_inf, 0.f);

    float weight = _opacity * (dist_sup - dist_inf) / (2.f * _sphere.radius());

    return weight * _color + (1.f - weight) * color;
  }

  //////////////////////////////////////////////////////////////
  std::optional<box3d_data_float> sphere_glow::get_bounding_box() const
  {
    return _sphere.get_bounding_box();
  }
}
