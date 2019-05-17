#ifndef __GGO_PHYSICS_WORLD__
#define __GGO_PHYSICS_WORLD__

#include <physics/ggo_rigid_body.h>
#include <physics/ggo_collisions.h>
#include <physics/ggo_contact.h>

namespace ggo
{
  class physics_world
  {
  public:

    physics_world(vec2_f gravity, int iterations) : _gravity(gravity), _iterations(iterations) {}

    void add(std::unique_ptr<rigid_body> rb) { _rigid_bodies.emplace_back(std::move(rb)); }

    void update(float time_step);

    template <typename func>
    void for_each_body(func f) const { for (const auto& body : _rigid_bodies) { f(*body); } };

    template <typename func>
    void for_each_contact(func f) const { for (const auto& contact : _contacts) { f(contact); } };

  private:

    void update_contacts();
    void integrate_forces(float time_step);
    void integrate_velocities(float time_step);
    void apply_accumulated_impulses();
    void apply_contact_impulses(float time_step);

    std::vector<contact>::iterator find_contact(rigid_body * body1, rigid_body * body2);

    const ggo::vec2_f _gravity;
    const int _iterations;
    std::vector<std::unique_ptr<rigid_body>> _rigid_bodies;
    std::vector<contact> _contacts;
  };
}

#endif
