#include "ggo_collisions.h"
#include <vector>
#include <map>
#include <numeric>

namespace ggo
{
  std::optional<ggo::separation_data> find_minimum_penetration_separating_axis(const ggo::oriented_box<float> & box1, const ggo::oriented_box<float> & box2)
  {
    auto check_separation = [](box_id reference_box_id,
      const ggo::oriented_box_f & reference_box,
      const ggo::oriented_box_f::edge & reference_edge,
      const ggo::oriented_box_f & incident_box,
      std::optional<separation_data> separation) -> std::optional<ggo::separation_data>
    {
      float reference_box_size_normal = 0.f;
      float reference_box_size_side = 0.f;
      switch (reference_edge._id)
      {
      case ggo::oriented_box_edge_id::left:
      case ggo::oriented_box_edge_id::right:
        reference_box_size_normal = reference_box.half_size_x();
        reference_box_size_side = reference_box.half_size_y();
        break;
      case ggo::oriented_box_edge_id::bottom:
      case ggo::oriented_box_edge_id::top:
        reference_box_size_normal = reference_box.half_size_y();
        reference_box_size_side = reference_box.half_size_x();
        break;
      }

      float proj_incident = project(incident_box, reference_edge._normal)._inf;
      float proj_reference = ggo::dot(reference_edge._normal, reference_box.pos()) + reference_box_size_normal;
      float penetration = proj_reference - proj_incident;
      if (penetration < 0)
      {
        return {};
      }
      if (separation && penetration > separation->_penetration)
      {
        return separation;
      }

      return { { penetration, reference_edge, reference_box_size_normal, reference_box_size_side, reference_box_id, &reference_box, &incident_box } };
    };

    const auto box1_edges = box1.get_edges();
    const auto box2_edges = box2.get_edges();

    std::optional<separation_data> separation;

    for (const auto & edge : box1_edges)
    {
      separation = check_separation(box_id::box1, box1, edge, box2, separation);
      if (!separation)
      {
        return {};
      }
    }

    for (const auto & edge : box2_edges)
    {
      separation = check_separation(box_id::box2, box2, edge, box1, separation);
      if (!separation)
      {
        return {};
      }
    }

    return separation;
  }

  std::array<ggo::oriented_box_f::vertex, 2> find_incident_edge(ggo::vec2_f reference_normal, const ggo::oriented_box<float> & incident_box)
  {
    auto incident_edges = incident_box.get_edges();

    std::array<ggo::oriented_box_f::vertex, 2> result;

    float proj_min = std::numeric_limits<float>::max();
    for (const auto & edge : incident_edges)
    {
      float proj_cur = ggo::dot(reference_normal, edge._normal);

      if (proj_cur < proj_min)
      {
        proj_min = proj_cur;
        result = edge._vertices;
      }
    }

    return result;
  }

  std::array<ggo::oriented_box_f::vertex, 2> clip_incident_edge(ggo::vec2_f reference_normal,
    ggo::pos2_f reference_box_center,
    float reference_dist_side,
    std::array<ggo::oriented_box_f::vertex, 2> incident_edge)
  {
    auto clip = [&](ggo::pos2_f p, ggo::pos2_f p2, ggo::vec2_f normal)
    {
      ggo::orthonormal_basis2d<float> basis(reference_box_center, reference_normal);

      ggo::pos2_f p_local = basis.point_from_world_to_local(p);
      ggo::pos2_f p2_local = basis.point_from_world_to_local(p2);
      ggo::vec2_f diff = p2_local - p_local;

      if (p_local.y() < -reference_dist_side)
      {
        float x = p_local.x() - (p_local.y() + reference_dist_side) * diff.x() / diff.y();
        ggo::pos2_f p_clipped_local = { x, -reference_dist_side };
        return basis.point_from_local_to_world(p_clipped_local);
      }
      else if (p_local.y() > reference_dist_side)
      {
        float x = p_local.x() - (p_local.y() - reference_dist_side) * diff.x() / diff.y();
        ggo::pos2_f p_clipped_local = { x, reference_dist_side };
        return basis.point_from_local_to_world(p_clipped_local);
      }
      else
      {
        return p;
      }
    };

    std::array<ggo::oriented_box_f::vertex, 2> result = incident_edge;

    result[0]._pos = clip(incident_edge[0]._pos, incident_edge[1]._pos, reference_normal);
    result[1]._pos = clip(incident_edge[1]._pos, incident_edge[0]._pos, reference_normal);

    return result;
  }

  incident_vertices discard_outside_contacts(ggo::pos2_f box_center,
    ggo::vec2_f normal, float dist_normal,
    const std::array<ggo::oriented_box_f::vertex, 2> & incident_edge)
  {
    incident_vertices result;
    result._count = 0;

    result._vertices[result._count]._penetration = dist_normal - ggo::dot(incident_edge[0]._pos - box_center, normal);
    if (result._vertices[result._count]._penetration > 0.f)
    {
      result._vertices[result._count]._vertex = incident_edge[0];
      result._count++;
    }

    result._vertices[result._count]._penetration = dist_normal - ggo::dot(incident_edge[1]._pos - box_center, normal);
    if (result._vertices[result._count]._penetration > 0.f)
    {
      result._vertices[result._count]._vertex = incident_edge[1];
      result._count++;
    }

    return result;
  }

  collision_data collide(const ggo::oriented_box<float> & box1, const ggo::oriented_box<float> & box2)
  {
    auto separating_axis_data = find_minimum_penetration_separating_axis(box1, box2);
    if (!separating_axis_data)
    {
      return {};
    }

    auto incident_edge = find_incident_edge(separating_axis_data->_reference_edge._normal,
      *separating_axis_data->_incident_box);

    incident_edge = clip_incident_edge(separating_axis_data->_reference_edge._normal,
      separating_axis_data->_reference_box->pos(),
      separating_axis_data->_reference_box_size_side,
      incident_edge);

    collision_data collision;
    collision._reference_box_id = separating_axis_data->_reference_box_id;
    collision._reference_edge = separating_axis_data->_reference_edge;
    collision._incident_vertices = discard_outside_contacts(separating_axis_data->_reference_box->pos(),
      separating_axis_data->_reference_edge._normal,
      separating_axis_data->_reference_box_size_normal,
      incident_edge);

    return collision;
  }
}
