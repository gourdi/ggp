#ifndef __GGO_COLLISIONS__
#define __GGO_COLLISIONS__

#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  enum class box_id
  {
    box1,
    box2,
  };

  inline std::ostream & operator<<(std::ostream & os, box_id id)
  {
    switch (id)
    {
    case box_id::box1:
      os << "box1";
      break;
    case box_id::box2:
      os << "box2";
      break;
    }
    return os;
  }

  struct separation_data
  {
    float _penetration;
    ggo::oriented_box_f::edge _reference_edge;
    float _reference_box_size_normal;
    float _reference_box_size_side;
    box_id _reference_box_id;
    const ggo::oriented_box<float> * _reference_box;
    const ggo::oriented_box<float> * _incident_box;
  };

  struct incident_vertex
  {
    float _penetration;
    ggo::oriented_box_f::vertex _vertex;
  };

  struct incident_vertices
  {
    size_t _count = 0;
    incident_vertex _vertices[2];
  };

  struct collision_data
  {
    ggo::box_id _reference_box_id;
    ggo::oriented_box_f::edge _reference_edge;
    incident_vertices _incident_vertices;
  };

  std::optional<ggo::separation_data> find_minimum_penetration_separating_axis(const ggo::oriented_box<float> & box1,
    const ggo::oriented_box<float> & box2);

  std::array<ggo::oriented_box_f::vertex, 2> find_incident_edge(ggo::vec2_f reference_normal,
    const ggo::oriented_box<float> & incident_box);

  std::array<ggo::oriented_box_f::vertex, 2> clip_incident_edge(ggo::vec2_f reference_normal,
    ggo::pos2_f reference_box_center,
    float reference_dist_side,
    std::array<ggo::oriented_box_f::vertex, 2> incident_edge);

  incident_vertices discard_outside_contacts(ggo::pos2_f box_center,
    ggo::vec2_f normal,
    float dist_normal,
    const std::array<ggo::oriented_box_f::vertex, 2> & incident_edge);

  collision_data collide(const ggo::oriented_box<float> & box1, const ggo::oriented_box<float> & box2);
}

#endif
