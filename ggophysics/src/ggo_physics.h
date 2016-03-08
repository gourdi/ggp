#ifndef __GGO_PHYSICS__
#define __GGO_PHYSICS__

#include <vector>
#include <ggo_shapes2d.h>

namespace ggo
{
  struct oriented_box_body
  {
    oriented_box_body(const ggo::oriented_box_float & box) : _box(box) {}

    float moment_of_intertia() const { return _mass * (ggo::square(_box.size1()) + ggo::square(_box.size2())) / 3.f; }

    float                   _mass = 1.f; // 1 kg.
    float                   _restitution = 0.8f;
    ggo::oriented_box_float _box;
    ggo::vector2d_float     _linear_velocity = { 0.f, 0.f };
    float                   _angular_velocity = 0.f;
  };

  void update_physics(oriented_box_body & body, const ggo::half_plane_float & half_plane, float dt);
}

#endif