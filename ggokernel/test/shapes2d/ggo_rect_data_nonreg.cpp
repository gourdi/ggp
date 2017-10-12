#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_data, rect_data_union)
{
  auto rect = ggo::get_union(
    ggo::rect_data<float>{ {1.f, -1.f}, 2.f, 3.f },
    ggo::rect_data<float>{ {2.f, 1.f}, 2.f, 2.f });

  GGO_CHECK_FABS(rect._pos.x(), 1);
  GGO_CHECK_FABS(rect._pos.y(), -1);
  GGO_CHECK_FABS(rect._width, 3);
  GGO_CHECK_FABS(rect._height, 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_data, rect_data_intersection)
{
  {
    ggo::rect_data<float> rect_data1{ { 1.f, 2.f }, 2.f, 1.f };
    ggo::rect_data<float> rect_data2{ { 4.f, 1.f }, 2.f, 3.f };

    GGO_CHECK(ggo::get_intersection(rect_data1, rect_data2).has_value() == false);
    GGO_CHECK(ggo::get_intersection(rect_data2, rect_data1).has_value() == false);
  }

  {
    ggo::rect_data<float> rect_data1{ { 2.f, 1.f }, 2.f, 1.f };
    ggo::rect_data<float> rect_data2{ { 1.f, 3.f }, 5.f, 1.f };

    GGO_CHECK(ggo::get_intersection(rect_data1, rect_data2).has_value() == false);
    GGO_CHECK(ggo::get_intersection(rect_data2, rect_data1).has_value() == false);
  }

  {
    ggo::rect_data<float> rect_data1{ { 1.f, -1.f }, 2.f, 6.f };
    ggo::rect_data<float> rect_data2{ { 2.f, 1.f }, 3.f, 2.f };

    auto intersection1 = ggo::get_intersection(rect_data1, rect_data2);
    GGO_CHECK(intersection1.has_value() == true);
    GGO_CHECK_FABS(intersection1->_pos.x(), 2);
    GGO_CHECK_FABS(intersection1->_pos.y(), 1);
    GGO_CHECK_FABS(intersection1->_width, 1);
    GGO_CHECK_FABS(intersection1->_height, 2);

    auto intersection2 = ggo::get_intersection(rect_data2, rect_data1);
    GGO_CHECK(intersection2.has_value() == true);
    GGO_CHECK_FABS(intersection2->_pos.x(), 2);
    GGO_CHECK_FABS(intersection2->_pos.y(), 1);
    GGO_CHECK_FABS(intersection2->_width, 1);
    GGO_CHECK_FABS(intersection2->_height, 2);
  }

  {
    ggo::rect_data<float> rect_data1{ { 2.f, 2.f }, 5.f, 3.f };
    ggo::rect_data<float> rect_data2{ { 3.f, 1.f }, 2.f, 2.f };

    auto intersection1 = ggo::get_intersection(rect_data1, rect_data2);
    GGO_CHECK(intersection1.has_value() == true);
    GGO_CHECK_FABS(intersection1->_pos.x(), 3);
    GGO_CHECK_FABS(intersection1->_pos.y(), 2);
    GGO_CHECK_FABS(intersection1->_width, 2);
    GGO_CHECK_FABS(intersection1->_height, 1);

    auto intersection2 = ggo::get_intersection(rect_data2, rect_data1);
    GGO_CHECK(intersection2.has_value() == true);
    GGO_CHECK_FABS(intersection2->_pos.x(), 3);
    GGO_CHECK_FABS(intersection2->_pos.y(), 2);
    GGO_CHECK_FABS(intersection2->_width, 2);
    GGO_CHECK_FABS(intersection2->_height, 1);
  }
}

