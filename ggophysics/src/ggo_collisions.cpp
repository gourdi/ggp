#include <ggo_intersections.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  bool test_collision(const ggo::half_plane<float> & half_plane,
                      const ggo::oriented_box<float> & box, const ggo::vector2d_float & linear_velocity, const float angular_velocity,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal, float & dt)
  {
    // Subdive the time step.
    int sub_steps = 1;
    const float dangle_max = ggo::PI<float>() / 64.f;
    const float dl_max = std::min(box.size1(), box.size2()) / 64.f;

    float dangle = std::abs(angular_velocity) * dt;
    float dx = std::abs(linear_velocity.x()) * dt;
    float dy = std::abs(linear_velocity.y()) * dt;
    while (dangle > dangle_max || dx > dl_max || dy > dl_max)
    {
      ++sub_steps;
      dangle = std::abs(angular_velocity) * dt / sub_steps;
      dx = std::abs(linear_velocity.x()) * dt / sub_steps;
      dy = std::abs(linear_velocity.y()) * dt / sub_steps;
    }

    for (int sub_step = 0; sub_step <= sub_steps; ++sub_step) // Large inequality here: we need to process the end of the time interval.
    {
      // Move and rotate the box to the current time.
      float dt_cur = dt * sub_step / static_cast<float>(sub_steps);
      ggo::oriented_box<float> box_cur(box);
      box_cur.move(dt_cur * linear_velocity);
      box_cur.rotate(dt_cur * angular_velocity, box_cur.get_center());

      // Check if points of the current box are inside the half-plane.
      std::vector<ggo::point2d_float> inside_points;
      for (int i = 0; i < 4; ++i)
      {
        float dist = ggo::dot(half_plane.normal(), box_cur[i]) - half_plane.dist_to_origin();
        if (dist < 0)
        {
          inside_points.push_back(box_cur[i]);
        }
      }

      // If at least one point is inside => collision.
      if (inside_points.empty() == false)
      {
        // Average inside points in case there is more than one.
        pos = { 0.f, 0.f };
        for (const auto & point : inside_points)
        {
          // Offset inside point to the half-plane border.
          float offset = half_plane.dist_to_origin() - ggo::dot(half_plane.normal(), point);
          GGO_ASSERT_GT(offset, -0.001f);
          pos += point + offset * half_plane.normal();
        }
        pos /= static_cast<float>(inside_points.size());
        GGO_ASSERT_FLOAT_EQ(ggo::dot(pos, half_plane.normal()), half_plane.dist_to_origin());

        normal = half_plane.normal();
        dt = dt_cur;

        return true;
      }
    }

    return false;
  }
}
