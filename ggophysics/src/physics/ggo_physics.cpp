#include <kernel/ggo_kernel.h>
#include <kernel/ggo_link.h>
#include "ggo_physics.h"
#include "ggo_collisions.h"
#include "ggo_impulse.h"
#include <fstream>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void update_physics(std::vector<oriented_box_body> & bodies, const std::vector<ggo::half_plane_f> & half_planes, float dt, float attenuation)
  {
    if (dt <= 0.f)
    {
      return;
    }

    // Subdivide current time step to safely process collisions.
    float size_min = std::numeric_limits<float>::max();
    float disp_max = 0.f;
    for (const auto & body : bodies)
    {
      size_min = std::min(size_min, std::min(body._box.size1(), body._box.size2()));
      disp_max = std::max(disp_max, ggo::length(body._linear_velocity));
      disp_max = std::max(disp_max, std::max(body._box.size1(), body._box.size2()) * body._angular_velocity);
    }

    int time_steps = 1;
    if (disp_max > 0.f && size_min > 0.f)
    {
      time_steps = static_cast<int>(64 * dt * disp_max / size_min + 1); // For robustness, don't round but cast to the upper ineger.
    }
    float dt_sub = dt / time_steps;

    //std::ofstream log("d:/log.txt", std::ios::app);
    auto& log = std::cout;
    log << "-----------------------" << std::endl << "dt (ms)= " << 1000.f * dt << "; time_steps=" << time_steps << std::endl ;

    for (int i = 0; i < time_steps; ++i)
    {
      for (auto it_body = bodies.begin(); it_body != bodies.end(); ++it_body)
      {
        // Apply attenuation.
        it_body->_angular_velocity *= attenuation;
        it_body->_linear_velocity *= attenuation;

        // Apply velocities.
        it_body->_box.move(dt_sub * it_body->_linear_velocity);
        it_body->_box.rotate(dt_sub * it_body->_angular_velocity, it_body->_box.get_center());
        log << "applied velocities:" << std::endl;
        log << "linear vel.=" << it_body->_linear_velocity << "; angular vel.=" << it_body->_angular_velocity << "; box = " << it_body->_box << std::endl;

        // Collisions with half-planes.
        for (const auto & half_plane : half_planes)
        {
          ggo::pos2_f pos{ 0.f, 0.f }, normal{ 0.f, 0.f };
          if (test_collision(half_plane, it_body->_box, pos, normal) == true)
          {
            // Handle bouncing: update velocities by computing impulse.
            ggo::vec2_f diff = it_body->_box.get_center() - pos;
            float impulse = compute_impulse(*it_body, pos, normal);

            it_body->apply_impulse(impulse, pos, normal);

            log << "collision => applied impulse=" << impulse << std::endl;
            log << "linear vel.=" << it_body->_linear_velocity << "; angular vel.=" << it_body->_angular_velocity << "; box = " << it_body->_box << std::endl;
          }
        }

        // Collisions with other oriented boxes.
        for (auto it_body2 = it_body + 1; it_body2 != bodies.end(); ++it_body2)
        {
          GGO_ASSERT(it_body != it_body2);

          ggo::pos2_f pos{ 0.f, 0.f }, normal{ 0.f, 0.f };
          if (test_collision(it_body->_box, it_body2->_box, pos, normal) == true)
          {
            // Handle bouncing: update velocities by computing impulse.
            float impulse = compute_impulse(*it_body, *it_body2, pos, normal);

            it_body->apply_impulse(impulse, pos, normal);
            it_body2->apply_impulse(-impulse, pos, normal);
          }
        }
      }
    }
  }
}

