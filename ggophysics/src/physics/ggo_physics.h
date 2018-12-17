#ifndef __GGO_PHYSICS__
#define __GGO_PHYSICS__

#include <vector>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  struct impulse
  {
    ggo::vec2_f _linear_velocity;
    float _angular_velocity;
  };

  struct rigid_body
  {
    ggo::vec2_f  _linear_velocity = { 0.f, 0.f };
    float       _angular_velocity = 0.f;

    virtual impulse     compute_impulse(const ggo::pos2_f & pos, const ggo::vec2_f & normal) const = 0;
    virtual ggo::pos2_f  get_center_of_mass() const = 0;

    void apply_impulse(float impulse_intensity, const ggo::pos2_f & pos, const ggo::vec2_f & normal)
    {
      auto impulse = compute_impulse(pos, normal);

      _linear_velocity += impulse_intensity * impulse._linear_velocity;
      _angular_velocity += impulse_intensity * impulse._angular_velocity;
    }
  };

  struct half_plane_body : public rigid_body
  {
    half_plane_body(const ggo::vec2_f & normal, float dist_to_origin) : _half_plane(normal, dist_to_origin) {}

    impulse compute_impulse(const ggo::pos2_f & pos, const ggo::vec2_f & normal) const override
    {
      return { {0.f, 0.f}, 0.f };
    }

    ggo::pos2_f  get_center_of_mass() const override
    {
      return _half_plane.normal() * _half_plane.dist_to_origin();
    }

    ggo::half_plane_f _half_plane;
  };

  struct oriented_box_body : public rigid_body
  {
    oriented_box_body(const ggo::oriented_box_f & box) : _box(box) {}

    float moment_of_intertia() const { return _mass * (ggo::square(_box.size1()) + ggo::square(_box.size2())) / 3.f; }

    impulse compute_impulse(const ggo::pos2_f & pos, const ggo::vec2_f & normal) const
    {
      impulse impulse;
      const ggo::vec2_f diff = _box.get_center() - pos;

      impulse._linear_velocity = normal / _mass;
      impulse._angular_velocity = ggo::ortho_dot(diff, normal) / moment_of_intertia();

      return impulse;
    }

    ggo::pos2_f  get_center_of_mass() const override
    {
      return _box.pos();
    }

    float                   _mass = 1.f; // 1 kg.
    float                   _restitution = 0.9f;
    ggo::oriented_box_f _box;
  };

  void update_physics(std::vector<oriented_box_body> & bodies, const std::vector<ggo::half_plane_f> & half_planes, float dt, float attenuation);
}

#endif
