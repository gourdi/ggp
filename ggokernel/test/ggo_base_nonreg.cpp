#include <ggo_nonreg.h>
#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, pad)
{
  static_assert(ggo::pad(0, 4) == 4);
  static_assert(ggo::pad(1, 4) == 4);
  static_assert(ggo::pad(2, 4) == 4);
  static_assert(ggo::pad(3, 4) == 4);
  static_assert(ggo::pad(4, 4) == 4);
  static_assert(ggo::pad(5, 4) == 8);
  static_assert(ggo::pad(6, 4) == 8);
  static_assert(ggo::pad(7, 4) == 8);
  static_assert(ggo::pad(8, 4) == 8);
  static_assert(ggo::pad(9, 4) == 12);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, loop_index)
{
  static_assert(ggo::loop_index(-6, 4) == 2);
  static_assert(ggo::loop_index(-5, 4) == 3);
  static_assert(ggo::loop_index(-4, 4) == 0);
  static_assert(ggo::loop_index(-3, 4) == 1);
  static_assert(ggo::loop_index(-2, 4) == 2);
  static_assert(ggo::loop_index(-1, 4) == 3);
  static_assert(ggo::loop_index( 0, 4) == 0);
  static_assert(ggo::loop_index( 1, 4) == 1);
  static_assert(ggo::loop_index( 2, 4) == 2);
  static_assert(ggo::loop_index( 3, 4) == 3);
  static_assert(ggo::loop_index( 4, 4) == 0);
  static_assert(ggo::loop_index( 5, 4) == 1);
  static_assert(ggo::loop_index( 6, 4) == 2);
  static_assert(ggo::loop_index( 7, 4) == 3);
  static_assert(ggo::loop_index( 8, 4) == 0);
  static_assert(ggo::loop_index( 9, 4) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, mirror_index)
{
  // Edge duplicated.
  static_assert(ggo::mirror_index_edge_duplicated(-6, 4) == 2);
  static_assert(ggo::mirror_index_edge_duplicated(-5, 4) == 3);
  static_assert(ggo::mirror_index_edge_duplicated(-4, 4) == 3);
  static_assert(ggo::mirror_index_edge_duplicated(-3, 4) == 2);
  static_assert(ggo::mirror_index_edge_duplicated(-2, 4) == 1);
  static_assert(ggo::mirror_index_edge_duplicated(-1, 4) == 0);
  static_assert(ggo::mirror_index_edge_duplicated(0, 4) == 0);
  static_assert(ggo::mirror_index_edge_duplicated(1, 4) == 1);
  static_assert(ggo::mirror_index_edge_duplicated(2, 4) == 2);
  static_assert(ggo::mirror_index_edge_duplicated(3, 4) == 3);
  static_assert(ggo::mirror_index_edge_duplicated(4, 4) == 3);
  static_assert(ggo::mirror_index_edge_duplicated(5, 4) == 2);
  static_assert(ggo::mirror_index_edge_duplicated(6, 4) == 1);
  static_assert(ggo::mirror_index_edge_duplicated(7, 4) == 0);
  static_assert(ggo::mirror_index_edge_duplicated(8, 4) == 0);
  static_assert(ggo::mirror_index_edge_duplicated(9, 4) == 1);

  // Single edge.
  static_assert(ggo::mirror_index_single_edge(-6, 5) == 2);
  static_assert(ggo::mirror_index_single_edge(-5, 5) == 3);
  static_assert(ggo::mirror_index_single_edge(-4, 5) == 4);
  static_assert(ggo::mirror_index_single_edge(-3, 5) == 3);
  static_assert(ggo::mirror_index_single_edge(-2, 5) == 2);
  static_assert(ggo::mirror_index_single_edge(-1, 5) == 1);
  static_assert(ggo::mirror_index_single_edge(0, 5) == 0);
  static_assert(ggo::mirror_index_single_edge(1, 5) == 1);
  static_assert(ggo::mirror_index_single_edge(2, 5) == 2);
  static_assert(ggo::mirror_index_single_edge(3, 5) == 3);
  static_assert(ggo::mirror_index_single_edge(4, 5) == 4);
  static_assert(ggo::mirror_index_single_edge(5, 5) == 3);
  static_assert(ggo::mirror_index_single_edge(6, 5) == 2);
  static_assert(ggo::mirror_index_single_edge(7, 5) == 1);
  static_assert(ggo::mirror_index_single_edge(8, 5) == 0);
  static_assert(ggo::mirror_index_single_edge(9, 5) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, round_to)
{
  static_assert(ggo::round_to<int8_t>(1.1f) == 1);
  static_assert(ggo::round_to<int8_t>(1.4f) == 1);
  static_assert(ggo::round_to<int8_t>(1.6f) == 2);
  static_assert(ggo::round_to<int8_t>(1.9f) == 2);
  static_assert(ggo::round_to<int8_t>(2.1f) == 2);
  static_assert(ggo::round_to<int8_t>(-1.1f) == -1);
  static_assert(ggo::round_to<int8_t>(-1.4f) == -1);
  static_assert(ggo::round_to<int8_t>(-1.6f) == -2);
  static_assert(ggo::round_to<int8_t>(-1.9f) == -2);
  static_assert(ggo::round_to<int8_t>(-2.1f) == -2);

  static_assert(ggo::round_to<int32_t>(1.1) == 1);
  static_assert(ggo::round_to<int32_t>(1.4) == 1);
  static_assert(ggo::round_to<int32_t>(1.6) == 2);
  static_assert(ggo::round_to<int32_t>(1.9) == 2);
  static_assert(ggo::round_to<int32_t>(2.1) == 2);
  static_assert(ggo::round_to<int32_t>(-1.1) == -1);
  static_assert(ggo::round_to<int32_t>(-1.4) == -1);
  static_assert(ggo::round_to<int32_t>(-1.6) == -2);
  static_assert(ggo::round_to<int32_t>(-1.9) == -2);
  static_assert(ggo::round_to<int32_t>(-2.1) == -2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, clamp_and_round)
{
  static_assert(ggo::clamp_and_round_to<int8_t>(1.1f) == 1);
  static_assert(ggo::clamp_and_round_to<int8_t>(1.4f) == 1);
  static_assert(ggo::clamp_and_round_to<int8_t>(1.6f) == 2);
  static_assert(ggo::clamp_and_round_to<int8_t>(1.9f) == 2);
  static_assert(ggo::clamp_and_round_to<int8_t>(2.1f) == 2);
  static_assert(ggo::clamp_and_round_to<int8_t>(127.1f) == 127);
  static_assert(ggo::clamp_and_round_to<int8_t>(127.2f) == 127);
  static_assert(ggo::clamp_and_round_to<int8_t>(127.9f) == 127);
  static_assert(ggo::clamp_and_round_to<int8_t>(128.1f) == 127);
  static_assert(ggo::clamp_and_round_to<int8_t>(999.0f) == 127);
  static_assert(ggo::clamp_and_round_to<int8_t>(-1.1f) == -1);
  static_assert(ggo::clamp_and_round_to<int8_t>(-1.4f) == -1);
  static_assert(ggo::clamp_and_round_to<int8_t>(-1.6f) == -2);
  static_assert(ggo::clamp_and_round_to<int8_t>(-1.9f) == -2);
  static_assert(ggo::clamp_and_round_to<int8_t>(-2.1f) == -2);
  static_assert(ggo::clamp_and_round_to<int8_t>(-128.1f) == -128);
  static_assert(ggo::clamp_and_round_to<int8_t>(-128.2f) == -128);
  static_assert(ggo::clamp_and_round_to<int8_t>(-128.9f) == -128);
  static_assert(ggo::clamp_and_round_to<int8_t>(-129.1f) == -128);
  static_assert(ggo::clamp_and_round_to<int8_t>(-999.0f) == -128);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, remove_first_if)
{
  {
    std::vector<int> v{ 6, 2, 3, 4, 5 };
    ggo::remove_first_if(v, [](int i) { return i % 2 == 1; }); // Remove first odd number.
    GGO_CHECK_EQ(4, v.size());
    GGO_CHECK_EQ(6, v[0]);
    GGO_CHECK_EQ(2, v[1]);
    GGO_CHECK_EQ(4, v[2]);
    GGO_CHECK_EQ(5, v[3]);
  }

  {
    std::vector<int> v{ 5, 1, 3 };
    ggo::remove_first_if(v, [](int i) { return i % 2 == 0; }); // Remove first even number.
    GGO_CHECK_EQ(3, v.size());
    GGO_CHECK_EQ(5, v[0]);
    GGO_CHECK_EQ(1, v[1]);
    GGO_CHECK_EQ(3, v[2]);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, pow)
{
  GGO_CHECK_EQ(ggo::pow(4, 0), 1);
  GGO_CHECK_EQ(ggo::pow(4, 1), 4);
  GGO_CHECK_EQ(ggo::pow(4, 2), 16);
  GGO_CHECK_EQ(ggo::pow(-4, 1), -4);
  GGO_CHECK_EQ(ggo::pow(-4, 2), 16);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, fixed_point_div)
{
  static_assert(ggo::fixed_point_div<4>(127) == 8);
  static_assert(ggo::fixed_point_div<4>(128) == 8);
  static_assert(ggo::fixed_point_div<4>(129) == 8);
  static_assert(ggo::fixed_point_div<4>(-128) == -8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, round_div)
{
  static_assert(ggo::round_div(0, 5) == 0);
  static_assert(ggo::round_div(1, 5) == 0);
  static_assert(ggo::round_div(2, 5) == 0);
  static_assert(ggo::round_div(3, 5) == 1);
  static_assert(ggo::round_div(4, 5) == 1);
  static_assert(ggo::round_div(5, 5) == 1);
  static_assert(ggo::round_div(6, 5) == 1);
  static_assert(ggo::round_div(7, 5) == 1);
  static_assert(ggo::round_div(8, 5) == 2);
  static_assert(ggo::round_div(9, 5) == 2);
  static_assert(ggo::round_div(10, 5) == 2);
  static_assert(ggo::round_div(11, 5) == 2);
  static_assert(ggo::round_div(12, 5) == 2);
  static_assert(ggo::round_div(13, 5) == 3);
  static_assert(ggo::round_div(14, 5) == 3);

  static_assert(ggo::round_div(-9, 4) == -2);
  static_assert(ggo::round_div(-8, 4) == -2);
  static_assert(ggo::round_div(-7, 4) == -2);
  static_assert(ggo::round_div(-6, 4) == -2);
  static_assert(ggo::round_div(-5, 4) == -1);
  static_assert(ggo::round_div(-4, 4) == -1);
  static_assert(ggo::round_div(-3, 4) == -1);
  static_assert(ggo::round_div(-2, 4) == -1);
  static_assert(ggo::round_div(-1, 4) == 0);
  static_assert(ggo::round_div(0, 4) == 0);
  static_assert(ggo::round_div(1, 4) == 0);
  static_assert(ggo::round_div(2, 4) == 1);
  static_assert(ggo::round_div(3, 4) == 1);
  static_assert(ggo::round_div(4, 4) == 1);
  static_assert(ggo::round_div(5, 4) == 1);
  static_assert(ggo::round_div(6, 4) == 2);
  static_assert(ggo::round_div(7, 4) == 2);
  static_assert(ggo::round_div(9, 4) == 2);
  static_assert(ggo::round_div(9, 4) == 2);

  static_assert(ggo::round_div(-9, -4) == 2);
  static_assert(ggo::round_div(-8, -4) == 2);
  static_assert(ggo::round_div(-7, -4) == 2);
  static_assert(ggo::round_div(-6, -4) == 2);
  static_assert(ggo::round_div(-5, -4) == 1);
  static_assert(ggo::round_div(-4, -4) == 1);
  static_assert(ggo::round_div(-3, -4) == 1);
  static_assert(ggo::round_div(-2, -4) == 1);
  static_assert(ggo::round_div(-1, -4) == 0);
  static_assert(ggo::round_div(0, -4) == 0);
  static_assert(ggo::round_div(1, -4) == 0);
  static_assert(ggo::round_div(2, -4) == -1);
  static_assert(ggo::round_div(3, -4) == -1);
  static_assert(ggo::round_div(4, -4) == -1);
  static_assert(ggo::round_div(5, -4) == -1);
  static_assert(ggo::round_div(6, -4) == -2);
  static_assert(ggo::round_div(7, -4) == -2);
  static_assert(ggo::round_div(9, -4) == -2);
  static_assert(ggo::round_div(9, -4) == -2);
}

