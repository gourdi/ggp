#include <kernel/ggo_kernel.h>
#include <kernel/ggo_reduce.h>
#include <kernel/ggo_ease.h>
#include <kernel/ggo_string_helpers.h>
#include <kernel/nonreg/ggo_nonreg.h>

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
GGO_TEST(base, remove_duplicates)
{
  {
    std::vector<int> v;
    ggo::remove_duplicates(v);
    GGO_CHECK(v.empty() == true);
  }

  {
    std::vector<int> v{ 1, 2, 3 };
    ggo::remove_duplicates(v);
    GGO_CHECK_EQ(v[0], 1);
    GGO_CHECK_EQ(v[1], 2);
    GGO_CHECK_EQ(v[2], 3);
  }

  {
    std::vector<int> v{ 1, 2, 3, 3, 1, 2, 3, 2, 3, 3, 2, 1, 1 ,2, 2, 1, 3, 2 };
    ggo::remove_duplicates(v);
    GGO_CHECK_EQ(v[0], 1);
    GGO_CHECK_EQ(v[1], 2);
    GGO_CHECK_EQ(v[2], 3);
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
  static_assert(ggo::fixed_point_div<4>(117) == 7);
  static_assert(ggo::fixed_point_div<4>(118) == 7); 
  static_assert(ggo::fixed_point_div<4>(119) == 7); 
  static_assert(ggo::fixed_point_div<4>(120) == 8); 
  static_assert(ggo::fixed_point_div<4>(121) == 8); 
  static_assert(ggo::fixed_point_div<4>(122) == 8); 
  static_assert(ggo::fixed_point_div<4>(123) == 8); 
  static_assert(ggo::fixed_point_div<4>(124) == 8);
  static_assert(ggo::fixed_point_div<4>(125) == 8);
  static_assert(ggo::fixed_point_div<4>(126) == 8);
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

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, adaptor)
{
  std::vector<int> v{ 1, 2, 3, 4 };

  auto adaptor = ggo::make_adaptor(v, [](int c) { return 2 * c; });

  std::vector<int> v2;
  for (const auto & k : adaptor)
  {
    v2.push_back(k);
  }

  GGO_CHECK_EQ(v2[0], 2);
  GGO_CHECK_EQ(v2[1], 4);
  GGO_CHECK_EQ(v2[2], 6);
  GGO_CHECK_EQ(v2[3], 8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, average)
{
  static_assert(ggo::average(150_u8, 200_u8, 250_u8) == 200_u8);
  static_assert(ggo::average(1, -5, -2) == -2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, minmax)
{
  static_assert(ggo::minmax(3, 1, 5, 4).first == 1);
  static_assert(ggo::minmax(3, 1, 5, 4).second == 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, range_intersection)
{
  GGO_CHECK(ggo::get_range_intersection<int>({ 1, 2 }, { 3, 4 }).has_value() == false);
  GGO_CHECK(ggo::get_range_intersection<int>({ 3, 4 }, { 1, 2 }).has_value() == false);

  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 1, 3 }, { 2, 4 })->_inf, 2);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 1, 3 }, { 2, 4 })->_sup, 3);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 2, 4 }, { 1, 3 })->_inf, 2);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 2, 4 }, { 1, 3 })->_sup, 3);

  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 1, 4 }, { 2, 3 })->_inf, 2);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 1, 4 }, { 2, 3 })->_sup, 3);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 2, 3 }, { 1, 4 })->_inf, 2);
  GGO_CHECK_EQ(ggo::get_range_intersection<int>({ 2, 3 }, { 1, 4 })->_sup, 3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, ease_inout)
{
  static_assert(ggo::ease_inout_to<float>(0, 2) == 0.0f);
  static_assert(ggo::ease_inout_to<float>(1, 2) == 1.0f);
  static_assert(ggo::ease_inout_to<float>(0, 3) == 0.0f);
  static_assert(ggo::ease_inout_to<float>(1, 3) == 0.5f);
  static_assert(ggo::ease_inout_to<float>(2, 3) == 1.0f);
}






