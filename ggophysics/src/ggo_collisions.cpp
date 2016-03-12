#include "ggo_collisions.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  bool test_collision(const ggo::half_plane<float> & half_plane, const ggo::oriented_box<float> & box,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal, ggo::vector2d_float & correction)
  {
    // Check if points of the current box are inside the half-plane.
    std::vector<ggo::point2d_float> inside_points;
    for (int i = 0; i < 4; ++i)
    {
      float dist = ggo::dot(half_plane.normal(), box[i]) - half_plane.dist_to_origin();
      if (dist < 0)
      {
        inside_points.push_back(box[i]);
      }
    }

    // If at least one point is inside => collision.
    if (inside_points.empty() == true)
    {
      return false;
    }

    normal = half_plane.normal();

    // Average inside points in case there is more than one.
    pos = { 0.f, 0.f };
    for (const auto & point : inside_points)
    {
      pos += point;
    }
    pos /= static_cast<float>(inside_points.size());

    // Position correction so that the box do not penetrate the half-plane.
    float penetration = ggo::dot(half_plane.normal(), pos) - half_plane.dist_to_origin();
    GGO_ASSERT_LT(penetration, 0.001f);
    correction = -half_plane.normal() * penetration;
    pos += correction;
    GGO_ASSERT_FLOAT_EQ(ggo::dot(pos, half_plane.normal()), half_plane.dist_to_origin());

    return true;
  }
}
