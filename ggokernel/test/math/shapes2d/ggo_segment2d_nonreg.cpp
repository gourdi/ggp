#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(segment, intersect_horizontal_segment)
{
  {
    ggo::segment_f segment({ 1, 1 }, { 5, 3 });

    GGO_CHECK(segment.intersect_horizontal_segment(2, 4, 2) == true);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 2, 2) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(4, 6, 2) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 0) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 4) == false);
  }

  {
    ggo::segment_f segment({ 5, 3 }, { 1, 1 });

    GGO_CHECK(segment.intersect_horizontal_segment(2, 4, 2) == true);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 2, 2) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(4, 6, 2) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 0) == false);
    GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 4) == false);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(segment, intersect_vertical_segment)
{
  {
    ggo::segment_f segment({ 1, 1 }, { 5, 3 });

    GGO_CHECK(segment.intersect_vertical_segment(3, 1, 3) == true);
    GGO_CHECK(segment.intersect_vertical_segment(3, 0, 1) == false);
    GGO_CHECK(segment.intersect_vertical_segment(3, 3, 4) == false);
    GGO_CHECK(segment.intersect_vertical_segment(0, 0, 3) == false);
    GGO_CHECK(segment.intersect_vertical_segment(6, 0, 4) == false);
  }

  {
    ggo::segment_f segment({ 5, 3 }, { 1, 1 });

    GGO_CHECK(segment.intersect_vertical_segment(3, 1, 3) == true);
    GGO_CHECK(segment.intersect_vertical_segment(3, 0, 1) == false);
    GGO_CHECK(segment.intersect_vertical_segment(3, 3, 4) == false);
    GGO_CHECK(segment.intersect_vertical_segment(0, 0, 3) == false);
    GGO_CHECK(segment.intersect_vertical_segment(6, 0, 4) == false);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(segment, intersect_segment)
{
  ggo::segment_f segment({ 3, 1 }, { 6, 4 });

  GGO_CHECK(segment.intersect({ { 3, 2 },{ 4, 3 } }).has_value() == false);
  GGO_CHECK(segment.intersect({ { 0, -1 },{ 2, -1 } }).has_value() == false);
  GGO_CHECK(segment.intersect({ { 7, 3 },{ 6, 7 } }).has_value() == false);

  auto intersection = segment.intersect({ { 3, 2 },{ 5, 2 } });
  GGO_CHECK(intersection.has_value() == true);
  GGO_CHECK_FLOAT_EQ(intersection->x(), 4);
  GGO_CHECK_FLOAT_EQ(intersection->y(), 2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(segment, dist_hypot)
{
  ggo::segment_f segment({ 2.f, 1.f }, { 4.f, 3.f });
  GGO_CHECK_FLOAT_EQ(segment.dist_to_point({ 1.f, 1.f }), 1.f);
  GGO_CHECK_FLOAT_EQ(segment.dist_to_point({ 4.f, 5.f }), 2.f);
  GGO_CHECK_FLOAT_EQ(segment.hypot_to_point({ 1.f, 4.f }), 8.f);
}
