#include "ggo_collisions.h"
#include <vector>
#include <numeric>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  bool test_collision(const ggo::half_plane<float> & half_plane, const ggo::oriented_box<float> & box,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal)
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
    pos = half_plane.normal() * half_plane.dist_to_origin();
    for (const auto & point : inside_points)
    {
      pos += point - half_plane.normal() * ggo::dot(half_plane.normal(), point);
    }
    pos /= inside_points.size();

    return true;
  }

  namespace
  {
    void find_collision_info(const ggo::oriented_box_float & box, const std::vector<ggo::point2d_float> & inside_points, ggo::point2d_float & pos, ggo::vector2d_float & normal)
    {
      std::vector<std::pair<float, float>> dots;
      for (const auto & point : inside_points)
      {
        GGO_ASSERT(box.is_point_inside(point) == true);

        ggo::vector2d_float diff(point - box.get_center());
        dots.push_back(std::make_pair(ggo::dot(box.dir(), diff), ggo::dot(box.dir2(), diff)));
      }

      float front_dist1 = 0.f;
      float back_dist1  = 0.f;
      float front_dist2 = 0.f;
      float back_dist2  = 0.f;
      for (const auto & dot : dots)
      {
        GGO_ASSERT(box.size1() - dot.first >= -0.001f);
        GGO_ASSERT(box.size1() + dot.first >= -0.001f);
        GGO_ASSERT(box.size2() - dot.second >= -0.001f);
        GGO_ASSERT(box.size2() + dot.second >= -0.001f);
        front_dist1 += box.size1() - dot.first;
        back_dist1  += box.size1() + dot.first;
        front_dist2 += box.size2() - dot.second;
        back_dist1  += box.size2() + dot.second;
      }

      auto offset_dir1 = [&]() { return std::accumulate(dots.begin(), dots.end(), 0.f, [](float acc, const std::pair<float, float> dots_pair) { return acc + dots_pair.first; }); };
      auto offset_dir2 = [&]() { return std::accumulate(dots.begin(), dots.end(), 0.f, [](float acc, const std::pair<float, float> dots_pair) { return acc + dots_pair.second; }); };

      if (front_dist1 <= back_dist1  && front_dist1 <= front_dist2 && front_dist1 <= back_dist2)
      {
        pos = box.size1() * box.dir() + offset_dir2() * box.dir2();
        normal = box.dir();
      }
      else if (back_dist1 <= front_dist1  && back_dist1 <= front_dist2 && back_dist1 <= back_dist2)
      {
        pos = -box.size1() * box.dir() + offset_dir2() * box.dir2();
        normal = -box.dir();
      }
      else if (front_dist2 <= front_dist1  && front_dist2 <= back_dist1 && front_dist2 <= back_dist2)
      {
        pos = box.size2() * box.dir2() + offset_dir1() * box.dir();
        normal = box.dir2();
      }
      else
      {
        GGO_ASSERT(back_dist2 <= front_dist1  && back_dist2 <= back_dist1 && back_dist2 <= front_dist2);
        pos = -box.size2() * box.dir2() + offset_dir1() * box.dir();
        normal = -box.dir2();
      }
    }
  }

  // This collision detection algorithm expect boxes to move slowly enough so that 
  // you just need to check for points inside one or another box to detect collision.
  // This is a simplification, since in the general case, this test is not enough.
  bool test_collision(const ggo::oriented_box<float> & box1, const ggo::oriented_box<float> & box2,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal)
  {
    std::vector<ggo::point2d_float> inside_box1(4);
    std::vector<ggo::point2d_float> inside_box2(4);

    for (const auto & point : box2.get_points())
    {
      if (box1.is_point_inside(point) == true)
      {
        inside_box1.push_back(point);
      }
    }

    for (const auto & point : box1.get_points())
    {
      if (box2.is_point_inside(point) == true)
      {
        inside_box2.push_back(point);
      }
    }

    if (inside_box1.empty() == true && inside_box2.empty() == true)
    {
      return false;
    }

    if (inside_box1.empty() == true)
    {
      find_collision_info(box1, inside_box1, pos, normal);
      return true;
    }

    if (inside_box2.empty() == true)
    {
      find_collision_info(box2, inside_box2, pos, normal);
      return true;
    }

    return false;
  }
}
