#include "ggo_physics_world.h"
#include <physics/ggo_collisions.h>

namespace ggo
{
  namespace
  {
    void apply_impulse(ggo::rigid_body * reference_body, ggo::rigid_body * incident_body, ggo::vec2_f r1, ggo::vec2_f r2, ggo::vec2_f impulse)
    {
      auto cross = [](ggo::vec2_f a, ggo::vec2_f b)
      {
        return a.x() * b.y() - a.y() * b.x();
      };

      reference_body->_linear_velocity -= reference_body->_inv_mass * impulse;
      reference_body->_angular_velocity -= reference_body->_inv_inertia_moment * cross(r1, impulse);

      incident_body->_linear_velocity += incident_body->_inv_mass * impulse;
      incident_body->_angular_velocity += incident_body->_inv_inertia_moment * cross(r2, impulse);
    };
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::update(float time_step)
  {
    update_contacts();
    integrate_forces(time_step);
    apply_accumulated_impulses();

    // Perform iterations
    for (int i = 0; i < _iterations; ++i)
    {
      apply_contact_impulses(time_step);
    }

    integrate_velocities(time_step);
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::update_contacts()
  {
    for (int i = 0; i < int(_rigid_bodies.size()); ++i)
    {
      const auto & rigid_body1 = _rigid_bodies[i];

      for (int j = i + 1; j < int(_rigid_bodies.size()); ++j)
      {
        const auto & rigid_body2 = _rigid_bodies[j];

        if (rigid_body1->_mass == 0.0f && rigid_body2->_mass == 0.0f)
          continue;

        auto collision = collide(rigid_body1->box(), rigid_body2->box());
        auto it = find_contact(rigid_body1.get(), rigid_body2.get());

        if (collision._incident_vertices._count > 0)
        {
          if (it != _contacts.end())
          {
            *it = update_contact(*it, collision, rigid_body1.get(), rigid_body2.get());
          }
          else
          {
            _contacts.push_back(create_contact(collision, rigid_body1.get(), rigid_body2.get()));
          }
        }
        else
        {
          if (it != _contacts.end())
          {
            _contacts.erase(it);
          }
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::integrate_velocities(float time_step)
  {
    for (auto & rigib_body : _rigid_bodies)
    {
      rigib_body->_position += time_step * rigib_body->_linear_velocity;
      rigib_body->_rotation += time_step * rigib_body->_angular_velocity;
    }
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::integrate_forces(float time_step)
  {
    for (auto & rigib_body : _rigid_bodies)
    {
      if (rigib_body->_inv_mass > 0.0f)
      {
        rigib_body->_linear_velocity  += time_step * _gravity;
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::apply_accumulated_impulses()
  {
    for (auto & contact : _contacts)
    {
      for (int i = 0; i < contact._points_count; ++i)
      {
        auto & contact_point = contact._points[i];

        ggo::vec2_f tangent = ggo::ortho_cw(contact._reference_edge._normal);
        ggo::vec2_f r1 = contact_point._vertex._pos - contact._reference_body->_position;
        ggo::vec2_f r2 = contact_point._vertex._pos - contact._incident_body->_position;

        // Apply normal + friction impulse
        ggo::vec2_f impulse = contact_point._normal_impulse * contact._reference_edge._normal + contact_point._tangent_impulse * tangent;
        apply_impulse(contact._reference_body, contact._incident_body, r1, r2, impulse);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  void physics_world::apply_contact_impulses(float time_step)
  {
    constexpr float allowed_penetration = 0.001f;
    constexpr float bias_factor = 0.2f;

    auto compute_mass = [](const ggo::rigid_body * body1, const ggo::rigid_body * body2, ggo::vec2_f r1, ggo::vec2_f r2, ggo::vec2_f dir)
    {
      float r1_proj = ggo::dot(r1, dir);
      float r2_proj = ggo::dot(r2, dir);

      float res = body1->_inv_mass + body2->_inv_mass;
      res += body1->_inv_inertia_moment * (ggo::dot(r1, r1) - r1_proj * r1_proj);
      res += body2->_inv_inertia_moment * (ggo::dot(r2, r2) - r2_proj * r2_proj);

      return 1.f / res;
    };

    for (auto & contact : _contacts)
    {
      for (int i = 0; i < contact._points_count; ++i)
      {
        auto & contact_point = contact._points[i];

        ggo::vec2_f r_reference = contact_point._vertex._pos - contact._reference_body->_position;
        ggo::vec2_f r_incident  = contact_point._vertex._pos - contact._incident_body->_position;

        // Relative velocity at contact point.
        ggo::vec2_f dv =
          contact._incident_body->_linear_velocity + contact._incident_body->_angular_velocity * ortho_ccw(r_incident) -
          contact._reference_body->_linear_velocity - contact._reference_body->_angular_velocity  * ortho_ccw(r_reference);

        // Normal impulse.
        {
          float mass_normal = compute_mass(contact._reference_body, contact._incident_body, r_reference, r_incident, contact._reference_edge._normal);
          float bias = -bias_factor * std::min(0.0f, -contact_point._penetration + allowed_penetration) / time_step; // Against penetration.

          float vn = ggo::dot(dv, contact._reference_edge._normal);
          float delta_impulse = mass_normal * (-vn + bias);

          // Clamp the accumulated impulse
          float old_impulse = contact_point._normal_impulse;
          contact_point._normal_impulse = std::max(old_impulse + delta_impulse, 0.0f);
          delta_impulse = contact_point._normal_impulse - old_impulse;

          // Apply contact impulse
          ggo::vec2_f normal_impulse = delta_impulse * contact._reference_edge._normal;
          apply_impulse(contact._reference_body, contact._incident_body, r_reference, r_incident, normal_impulse);
        }

        // Tangent normal.
        {
          ggo::vec2_f tangent = ggo::ortho_cw(contact._reference_edge._normal);
          float mass_tangent = compute_mass(contact._reference_body, contact._incident_body, r_reference, r_incident, tangent);

          float vt = ggo::dot(dv, tangent);
          float delta_impulse = -mass_tangent * vt;

          // Clamp the accumulated impulse
          float old_impulse = contact_point._tangent_impulse;
          float max_impulse = std::sqrt(contact._reference_body->_friction * contact._incident_body->_friction) * contact_point._normal_impulse;
          contact_point._tangent_impulse = ggo::clamp(old_impulse + delta_impulse, -max_impulse, max_impulse);
          delta_impulse = contact_point._tangent_impulse - old_impulse;

          // Apply contact impulse
          ggo::vec2_f tangent_impulse = delta_impulse * tangent;
          apply_impulse(contact._reference_body, contact._incident_body, r_reference, r_incident, tangent_impulse);
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::vector<contact>::iterator physics_world::find_contact(rigid_body * body1, rigid_body * body2)
  {
    for (auto it = _contacts.begin(); it != _contacts.end(); ++it)
    {
      if (it->_reference_body == body1 && it->_incident_body == body2)
      {
        return it;
      }

      if (it->_reference_body == body2 && it->_incident_body == body1)
      {
        return it;
      }
    }

    return _contacts.end();
  }
}
