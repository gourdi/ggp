#ifndef __GGO_CONTACT__
#define __GGO_CONTACT__

#include <physics/ggo_collisions.h>

namespace ggo
{
  struct rigid_body;

  struct contact_point
  {
    float _penetration;
    float _normal_impulse;
    float _tangent_impulse;
    ggo::oriented_box_f::vertex _vertex;
  };

  struct contact
  {
    ggo::oriented_box_f::edge _reference_edge;
    rigid_body * _reference_body;
    rigid_body * _incident_body;
    size_t _points_count = 0;
    contact_point _points[2];
  };

  contact create_contact(const ggo::collision_data & collision, rigid_body * body1, rigid_body * body2);
  contact update_contact(const contact & c, const ggo::collision_data & collision, rigid_body * body1, rigid_body * body2);
}

#endif