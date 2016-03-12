#include <ggo_kernel.h>
#include <ggo_link.h>
#include "ggo_physics.h"
#include "ggo_collisions.h"
#include "ggo_impulse.h"
#include <fstream>

const float GRAVITY = 9.8f;

std::vector<ggo::link<const void *>> contacts;
std::vector<ggo::link<const void *>> prv_contacts;
std::vector<ggo::link<const void *>> cur_contacts;

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void update_physics(oriented_box_body & body, const std::vector<ggo::half_plane_float> & half_planes, float dt)
  {
    cur_contacts.clear();

    if (dt <= 0.f)
    {
      return;
    }

    // Subdivide current time step to safely process collisions.
    const float dl_max = std::min(body._box.size1(), body._box.size2()) / 64.f;
    GGO_ASSERT_GT(dl_max, 0.f);
    int time_steps = 1;

    while (true)
    {
      float dt_inc = dt / time_steps;
      float dl1 = dt_inc * body._linear_velocity.get_length();
      float dl2 = dt_inc * std::max(body._box.size1(), body._box.size2()) * body._angular_velocity;

      if (dl1 < dl_max && dl2 < dl_max)
      {
        break;
      }

      time_steps *= 2;
    }
    float dt_sub = dt / time_steps;

    //std::ofstream log("d:/log.txt", std::ios::app);
    auto& log = std::cout;
    log << "-----------------------" << std::endl << "dt (ms)= " << 1000.f * dt << "; time_steps=" << time_steps << std::endl ;

    for (int i = 0; i < time_steps; ++i)
    {
      // Process gravity.
      //body._linear_velocity += dt_sub * ggo::vector2d_float(0.f, -GRAVITY) / body._mass;

      // Apply velocities.
      body._box.move(dt_sub * body._linear_velocity);
      body._box.rotate(dt_sub * body._angular_velocity, body._box.get_center());
      log << "applied velocities:" << std::endl;
      log << "linear vel.=" << body._linear_velocity << "; angular vel.=" << body._angular_velocity << "; box = " << body._box << std::endl;

      for (const auto & half_plane : half_planes)
      {
        ggo::point2d_float pos{ 0.f, 0.f }, normal{ 0.f, 0.f }, correction{ 0.f, 0.f };
        if (test_collision(half_plane, body._box, pos, normal, correction) == true)
        {
          // Collision: first move the body to the collision position.
          body._box.move(correction);

          // Handle bouncing: update velocities by computing impulses.
          ggo::vector2d_float diff = body._box.get_center() - pos;
          float impulse = compute_static_impulse(body, pos, normal);

          body._linear_velocity += impulse * normal / body._mass;
          body._angular_velocity += impulse * ggo::ortho_dot(diff, normal) / body.moment_of_intertia();

          log << "collision => applied impulse=" << impulse << std::endl;
          log << "linear vel.=" << body._linear_velocity << "; angular vel.=" << body._angular_velocity << "; box = " << body._box << std::endl;
        }
      }
    }
  }
}

