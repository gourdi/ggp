#include <physics/ggo_physics.h>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/lcp/lemke.h>

struct contact
{
  const ggo::rigid_body * _body1;
  const ggo::rigid_body * _body2;
  ggo::pos2_f _pos;
  ggo::vec2_f _normal; // From body2 to body1.
  float _relative_velocity;
};

ggo::array<float, 2> build_lcp_matrix(const std::vector<contact> & contacts)
{
  auto compute_velocity = [](const ggo::pos2_f & p, const ggo::pos2_f & center_of_mass, const ggo::vec2_f & linear_velocity, float angular_velocity)
  {
    const ggo::vec2_f diff = p - center_of_mass;
    const ggo::vec2_f ortho(-diff.y(), diff.x());
    return linear_velocity + angular_velocity * ortho;
  };

  ggo::array<float, 2> lcp_matrix(int(contacts.size()), int(contacts.size()));

  for (int i = 0; i < int(contacts.size()); ++i)
  {
    auto impulse1 = contacts[i]._body1->compute_impulse(contacts[i]._pos, contacts[i]._normal);
    auto impulse2 = contacts[i]._body2->compute_impulse(contacts[i]._pos, -contacts[i]._normal);

    ggo::vec2_f linear_velocity1 = contacts[i]._body1->_linear_velocity + impulse1._linear_velocity;
    ggo::vec2_f linear_velocity2 = contacts[i]._body2->_linear_velocity + impulse2._linear_velocity;
    float angular_velocity1 = contacts[i]._body1->_angular_velocity + impulse1._angular_velocity;
    float angular_velocity2 = contacts[i]._body2->_angular_velocity + impulse2._angular_velocity;

    for (int j = i; j < int(contacts.size()); ++j)
    {
      ggo::vec2_f v1 = compute_velocity(contacts[j]._pos, contacts[j]._body1->get_center_of_mass(), linear_velocity1, angular_velocity1);
      ggo::vec2_f v2 = compute_velocity(contacts[j]._pos, contacts[j]._body2->get_center_of_mass(), linear_velocity2, angular_velocity2);

      float relative_velocity = ggo::dot(v1 - v2, contacts[j]._normal);

      lcp_matrix(i, j) = lcp_matrix(j, i) = relative_velocity - contacts[j]._relative_velocity;
    }
  }

  return lcp_matrix;
}

GGO_TEST(contact, test1)
{
  ggo::oriented_box_body body1({ { 0.f, 1.f }, { 0.f, 1.f }, 1.f, 1.f });
  ggo::half_plane_body body2({ 0.f, 1.f }, 0.f);

  contact contact1;
  contact1._body1 = &body1;
  contact1._body2 = &body2;
  contact1._normal = { 0.f, 1.f };
  contact1._pos = { -1.f, 0.f };
  contact1._relative_velocity = -1.f;

  contact contact2;
  contact2._body1 = &body1;
  contact2._body2 = &body2;
  contact2._normal = { 0.f, 1.f };
  contact2._pos = { 1.f, 0.f };
  contact2._relative_velocity = -1.f;

  auto lcp_matrix = build_lcp_matrix({ contact1, contact2 });

  ggo::array<float, 2> previous_vel(1, 2);
  previous_vel(0, 0) = -1.f;
  previous_vel(0, 1) = -1.f;
  ggo::array<float, 2> after_vel(1, 2);
  ggo::array<float, 2> z(1, 2);

  ggo::lemke::solve(lcp_matrix, previous_vel, after_vel, z);
}

