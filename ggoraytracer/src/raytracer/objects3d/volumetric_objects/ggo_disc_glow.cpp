#include <kernel/ggo_ease.h>
#include "ggo_disc_glow.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  disc_glow::disc_glow(const ggo::pos3_f & pos, float opacity, const ggo::rgb_32f & color, float inner_radius, float outter_radius)
  :
  _pos(pos),
  _opacity(opacity),
  _color(color),
  _inner_radius(inner_radius),
  _outter_radius(outter_radius)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f disc_glow::process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const
  {
    ray3d_f ray(p1, p2 - p1);
    ray3d_f normal(_pos, -ray.dir(), false);
    disc3d_f disc(normal, _outter_radius);

    float dist = -1.f;
    if (disc.intersect_ray(ray, dist, normal) == true && dist < distance(p1, p2))
    {
      dist = ggo::distance(normal.pos(), _pos);
      GGO_ASSERT(dist <= _outter_radius + 0.001f);

      const float opacity = dist < _inner_radius ? _opacity : _opacity * ggo::ease_inout((_outter_radius - dist) / (_outter_radius - _inner_radius));

      return opacity * _color + color;
    }

    return color;
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f disc_glow::process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const
  {
    ray3d_f normal(_pos, -ray.dir(), false);
    disc3d_f disc(normal, _outter_radius);

    float dist = -1.f;
    if (disc.intersect_ray(ray, dist, normal) == true)
    {
      dist = ggo::distance(normal.pos(), _pos);
      GGO_ASSERT(dist <= _outter_radius + 0.001f);

      const float opacity = dist < _inner_radius ? _opacity : _opacity * ggo::ease_inout((_outter_radius - dist) / (_outter_radius - _inner_radius));

      return opacity * _color + color;
    }

    return color;
  }

  //////////////////////////////////////////////////////////////
  std::optional<box3d_data_float> disc_glow::get_bounding_box() const
  {
    return sphere3d_f(_pos, _outter_radius).get_bounding_box(ggo::basis3d_f());
  }
}

