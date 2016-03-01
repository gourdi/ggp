#include <ggo_kernel.h>
#include <ggo_link.h>
#include "ggo_physics.h"
#include "ggo_collisions.h"
#include "ggo_intersections.h"
#include <fstream>

const float GRAVITY = 9.8f;

std::vector<ggo::link<const void *>> contacts;
std::vector<ggo::link<const void *>> prv_contacts;
std::vector<ggo::link<const void *>> cur_contacts;

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void update_physics(oriented_box_body & body, const ggo::half_plane_float & half_plane, float dt)
  {
    cur_contacts.clear();

    if (dt <= 0.f)
    {
      return;
    }

    std::ofstream log("d:/log.txt", std::ios::app);
    log << "dt: " << dt << std::endl;

    // Process collisions and contacts.
    //ggo::link<const void *> contact_link(&half_plane, &body);
    //if (ggo::find(contacts, contact_link) == false)
    //{
    //while (dt > 0.f)
    //{

      // Check for collisions.
    float dt_tmp = dt;
    ggo::point2d_float pos{ 0.f, 0.f };
    ggo::vector2d_float normal{ 0.f, 0.f };
    if (test_collision(half_plane, body._box, body._linear_velocity, body._angular_velocity, pos, normal, dt_tmp) == true)
    {
      log << "collision detected: dt_tmp=" << dt_tmp << std::endl;

      // Collision: first move the body to the collision position.
      body._box.move(dt_tmp * body._linear_velocity);
      body._box.rotate(dt_tmp * body._angular_velocity);
      log << "position correction: box=" << body._box << std::endl;

      // Handle bouncing: update velocities by computing impulses.
      ggo::vector2d_float diff = body._box.get_center() - pos;
      float impulse = compute_static_impulse(body, pos, normal);

      body._linear_velocity += impulse * normal / body._mass;
      body._angular_velocity += impulse * ggo::ortho_dot(diff, normal) / body.moment_of_intertia();

      // Remove elapsed time.
      dt -= dt_tmp;
    }

    // Process gravity.
    body._linear_velocity += dt * ggo::vector2d_float(0.f, -GRAVITY) / body._mass;
    

    // Apply velocities.
    body._box.move(dt * body._linear_velocity);
    body._box.rotate(dt * body._angular_velocity, body._box.get_center());

    log << "applied velocities: linear=" << body._linear_velocity << "; angular=" << body._angular_velocity << "; box=" << body._box << std::endl;

    float linear_kinetic_engery = 0.5f * body._mass * ggo::square(body._linear_velocity.get_length());
    float angular_kinetic_engery = 0.5f * body.moment_of_intertia() * ggo::square(body._angular_velocity);
    float total_kinetic_engery = linear_kinetic_engery + angular_kinetic_engery;
    log << "kinetic energies: linear=" << linear_kinetic_engery << "; angular=" << angular_kinetic_engery << "; sum=" << total_kinetic_engery << std::endl << std::endl;
    //}


      // The body is moving away => discard detected collision.
      //if (ggo::dot(body._linear_velocity, normal) > 0.001f)
      //{
      //  log << "moving away" << std::endl;
      //  break;
      //}



      // Contact if the objects were intersectiong at the previous frame already.
      //if (ggo::find(prv_contacts, contact_link) == true)
      //{
      //  log << "contact detected" << std::endl;
      //  push_once(contacts, contact_link);
      //  break;
      //}
      //ggo::push_once(cur_contacts, contact_link);

  }

  // Physics is not the same with or without contacts.
  //if (ggo::find(contacts, contact_link) == true)
  //{
  //  log << "contact:" << std::endl;
  //}
  //else
  //{
  //  // Process gravity.
  //  body._linear_velocity += dt * ggo::vector2d_float(0.f, -GRAVITY) / body._mass;
  //  log << "gravity: linear_velocity=" << body._linear_velocity << "; angle_velocity=" << body._angular_velocity << "; pos = " << body._box.get_center() << std::endl;

  //  // Apply velocities.
  //  body._box.move(dt * body._linear_velocity);
  //  body._box.rotate(dt * body._angular_velocity, body._box.get_center());
  //}

  //std::swap(prv_contacts, cur_contacts);
}

