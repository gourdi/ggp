#ifndef __GGO_RIGIB_BODY__
#define __GGO_RIGIB_BODY__

#include <vector>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  inline float compute_inertia_moment(float mass, float half_size_x, float half_size_y)
  {
    float size_x = 2 * half_size_x;
    float size_y = 2 * half_size_y;
    return mass * (size_x * size_x + size_y * size_y) / 12.0f;
  }

  struct rigid_body
  {
    rigid_body(float half_size_x, float half_size_y, float mass, float friction)
      :
      rigid_body(half_size_x, half_size_y, mass, compute_inertia_moment(mass, half_size_x, half_size_y), friction)
    {
    }

    ggo::oriented_box_f box() const { return { _position, _rotation, _half_size_x, _half_size_y }; }

    ggo::pos2_f _position{ 0.f, 0.f };
    float _rotation = 0.f;

    ggo::vec2_f _linear_velocity{ 0.f, 0.f };
    float _angular_velocity = 0.f;

    const float _half_size_x;
    const float _half_size_y;
    const float _mass;
    const float _inv_mass;
    const float _inertia_moment;
    const float _inv_inertia_moment;
    const float _friction;

  private:

    rigid_body(float half_size_x, float half_size_y, float mass, float inertia_moment, float friction)
      :
      _half_size_x(half_size_x),
      _half_size_y(half_size_y),
      _mass(mass),
      _inv_mass(mass <= 0.f ? 0.f : 1.f / mass),
      _inertia_moment(inertia_moment),
      _inv_inertia_moment(inertia_moment <= 0.f ? 0.f : 1.f / inertia_moment),
      _friction(friction)
    {

    }
  };
}

#endif
