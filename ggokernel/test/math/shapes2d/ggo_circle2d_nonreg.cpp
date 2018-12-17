#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(circle, line_segment_intersection)
{
  {
    ggo::circle_f circle(ggo::pos2_f(3.f, 2.f), 1);

    {
      auto points = circle.get_line_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(1.f, 1.f), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2_f(2.f, 2.f)) == true);
      GGO_CHECK(find_point(points, ggo::pos2_f(3.f, 3.f)) == true);
    }

    {
      auto points = circle.get_segment_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(2.5f, 2.5f));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::pos2_f(2.f, 2.f)) == true);
    }

    {
      auto points = circle.get_segment_intersection(ggo::pos2_f(4.f, 4.f), ggo::pos2_f(2.5f, 2.5f));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::pos2_f(3.f, 3.f)) == true);
    }

    {
      auto points = circle.get_segment_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(1.f, 1.f));
      GGO_CHECK(points.empty() == true);
    }

    {
      auto points = circle.get_segment_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(10.f, 10.f));
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2_f(2.f, 2.f)) == true);
      GGO_CHECK(find_point(points, ggo::pos2_f(3.f, 3.f)) == true);
    }
  }

  {
    ggo::circle_f circle(ggo::pos2_f(3.f, 1.f), 1);

    {
      auto points = circle.get_line_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(2.f, 1.f), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2_f(2.f, 1.f)) == true);
    }

    {
      auto points = circle.get_segment_intersection(ggo::pos2_f(0.f, 0.f), ggo::pos2_f(1.f, 0.5f));
      GGO_CHECK(points.empty() == true);
    }
  }
}
