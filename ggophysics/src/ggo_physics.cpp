#include "ggo_physics.h"

const float GRAVITY = 9.8f;

namespace ggo
{
  struct applied_force
  {
    ggo::point2d_float  _pos;
    ggo::vector2d_float _force;
  };

  /////////////////////////////////////////////////////////////////////
  void update_physics(std::vector<body> & bodies, const float dt)
  {
    if (dt <= 0.f)
    {
      return;
    }
     

    for (auto & body1 : bodies)
    {
      std::vector<applied_force> applied_forces;

      // Gravity (which applies on the center of mass).
      applied_force gravity_force;
      gravity_force._pos = body1._shape->get_center();
      gravity_force._force = { 0.f, -GRAVITY };
      applied_forces.emplace_back(gravity_force);

      // Update the linear velocity.
      ggo::vector2d_float velocity_delta{ 0.f, 0.f };
      for (const auto & applied_force : applied_forces)
      {
        velocity_delta += applied_force._force;
      }
      body1._linear_velocity += velocity_delta / body1._mass;

      // Update position.
      body1._shape->move(dt * body1._linear_velocity);
    }
  }
}
