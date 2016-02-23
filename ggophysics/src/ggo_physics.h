#ifndef __GGO_PHYSICS__
#define __GGO_PHYSICS__

#include <vector>
#include <ggo_shapes2d.h>

namespace ggo
{
  struct body
  {
    float                                             _mass = 1.f; // 1 kg.
    std::shared_ptr<ggo::physics_shape2d_abc<float>>  _shape;
    ggo::vector2d_float                               _linear_velocity = { 0.f, 0.f };
    float                                             _angular_velocity = 0.f;
  };

  void update_physics(std::vector<body> & bodies, const float dt);
}

#endif
