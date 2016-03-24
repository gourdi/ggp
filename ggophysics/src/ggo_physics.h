#ifndef __GGO_PHYSICS__
#define __GGO_PHYSICS__

#include <vector>
#include <ggo_shapes2d.h>

namespace ggo
{
  inline float ortho_dot(const ggo::vector2d_float & v1, const ggo::vector2d_float & v2) { return v1.y() * v2.x() - v1.x() * v2.y(); }

  struct oriented_box_body
  {
    oriented_box_body(const ggo::oriented_box_float & box) : _box(box) {}

    float moment_of_intertia() const { return _mass * (ggo::square(_box.size1()) + ggo::square(_box.size2())) / 3.f; }

    void apply_impulse(float impulse, const ggo::point2d_float & pos, const ggo::vector2d_float & normal)
    {
      const ggo::vector2d_float diff = _box.get_center() - pos;

      _linear_velocity += impulse * normal / _mass;
      _angular_velocity += impulse * ggo::ortho_dot(diff, normal) / moment_of_intertia();
    }

    float                   _mass = 1.f; // 1 kg.
    float                   _restitution = 0.9f;
    ggo::oriented_box_float _box;
    ggo::vector2d_float     _linear_velocity = { 0.f, 0.f };
    float                   _angular_velocity = 0.f;
  };

  void update_physics(std::vector<oriented_box_body> & bodies, const std::vector<ggo::half_plane_float> & half_planes, float dt, float attenuation);
}

#endif
