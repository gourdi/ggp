#include <physics/ggo_contact.h>

namespace ggo
{
  namespace
  {
    contact_point create_contact_point(const incident_vertex & v)
    {
      return { v._penetration, 0.f, 0.f, v._vertex };
    };
  }

  /////////////////////////////////////////////////////////////////////
  contact create_contact(const ggo::collision_data & collision, rigid_body * body1, rigid_body * body2)
  {
    rigid_body * reference_body = collision._reference_box_id == box_id::box1 ? body1 : body2;
    rigid_body * incident_body  = collision._reference_box_id == box_id::box1 ? body2 : body1;

    contact new_contact({ collision._reference_edge, reference_body, incident_body });

    new_contact._points_count = collision._incident_vertices._count;
    new_contact._points[0] = create_contact_point(collision._incident_vertices._vertices[0]);
    if (collision._incident_vertices._count == 2)
    {
      new_contact._points[1] = create_contact_point(collision._incident_vertices._vertices[1]);
    }

    return new_contact;
  }

  /////////////////////////////////////////////////////////////////////
  contact update_contact(const contact & old_contact, const ggo::collision_data & collision, rigid_body * body1, rigid_body * body2)
  {
    contact new_contact = old_contact;

    new_contact._reference_edge = collision._reference_edge;
    new_contact._reference_body = collision._reference_box_id == box_id::box1 ? body1 : body2;
    new_contact._incident_body = collision._reference_box_id == box_id::box1 ? body2 : body1;
    new_contact._points_count = collision._incident_vertices._count;
    new_contact._points[0] = create_contact_point(collision._incident_vertices._vertices[0]);
    if (collision._incident_vertices._count == 2)
    {
      new_contact._points[1] = create_contact_point(collision._incident_vertices._vertices[1]);
    }

    // Try to match collision data.
    if (new_contact._reference_body == old_contact._reference_body && new_contact._reference_edge._id == old_contact._reference_edge._id)
    {
      for (int i = 0; i < collision._incident_vertices._count; ++i)
      {
        std::optional<int> match;

        // Try to match contact point.
        for (int j = 0; j < old_contact._points_count; ++j)
        {
          if (collision._incident_vertices._vertices[i]._vertex._id == old_contact._points[j]._vertex._id)
          {
            match = j;
            break;
          }
        }

        if (match)
        {
          new_contact._points[i]._normal_impulse = old_contact._points[*match]._normal_impulse;
          new_contact._points[i]._tangent_impulse = old_contact._points[*match]._tangent_impulse;
        }
      }
    }

    return new_contact;
  }
}

