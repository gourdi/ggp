#include "ggo_kernel_nonreg.h"
#include <kernel/ggo_vec2.h>
#include <kernel/ggo_vec3.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, contruction_2d)
{
  constexpr ggo::vec2_i v1(2, 3);
  static_assert(v1.x() == 2);
  static_assert(v1.y() == 3);

  constexpr ggo::vec2_i v2(v1);
  static_assert(v2.x() == 2);
  static_assert(v2.y() == 3);

  constexpr ggo::vec2_i v3;
  static_assert(v3.x() == 0);
  static_assert(v3.y() == 0);

  constexpr ggo::vec2_i v4(1);
  static_assert(v4.x() == 1);
  static_assert(v4.y() == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, contruction_3d)
{
  constexpr ggo::vec3_i v1(2, 3, 4);
  static_assert(v1.x() == 2);
  static_assert(v1.y() == 3);
  static_assert(v1.z() == 4);

  constexpr ggo::vec3_i v2(v1);
  static_assert(v2.x() == 2);
  static_assert(v2.y() == 3);
  static_assert(v2.z() == 4);

  constexpr ggo::vec3_i v3;
  static_assert(v3.x() == 0);
  static_assert(v3.y() == 0);
  static_assert(v3.z() == 0);

  constexpr ggo::vec3_i v4(1);
  static_assert(v4.x() == 1);
  static_assert(v4.y() == 1);
  static_assert(v4.z() == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, equality)
{
  static_assert(ggo::vec2_i(1, 2) == ggo::vec2_i(1, 2));
  static_assert(ggo::vec2_i(1, 2) != ggo::vec2_i(1, 3));
  static_assert(ggo::vec2_i(1, 2) != ggo::vec2_i(3, 2));

  static_assert(ggo::vec3_i(1, 2, 3) == ggo::vec3_i(1, 2, 3));
  static_assert(ggo::vec3_i(1, 2, 3) != ggo::vec3_i(0, 2, 3));
  static_assert(ggo::vec3_i(1, 2, 3) != ggo::vec3_i(1, 0, 3));
  static_assert(ggo::vec3_i(1, 2, 3) != ggo::vec3_i(1, 2, 0));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, global_operators)
{
  // Vector operators.
  static_assert(ggo::vec2_i(1, 2) + ggo::vec2_i(2, 3) == ggo::vec2_i(3, 5));
  static_assert(ggo::vec2_i(6, 5) - ggo::vec2_i(4, 2) == ggo::vec2_i(2, 3));

  static_assert(ggo::vec3_i(1, 2, 1) + ggo::vec3_i(2, 3, 5) == ggo::vec3_i(3, 5, 6));
  static_assert(ggo::vec3_i(6, 7, 8) - ggo::vec3_i(2, 3, 5) == ggo::vec3_i(4, 4, 3));

  // Scalar operators.
  static_assert(ggo::vec2_i(1, 2) * 2 == ggo::vec2_i(2, 4));
  static_assert(2 * ggo::vec2_i(1, 2) == ggo::vec2_i(2, 4));
  static_assert(ggo::vec2_i(9, 6) / 3 == ggo::vec2_i(3, 2));

  static_assert(ggo::vec3_i(1, 2, 3) * 2 == ggo::vec3_i(2, 4, 6));
  static_assert(2 * ggo::vec3_i(1, 2, 3) == ggo::vec3_i(2, 4, 6));
  static_assert(ggo::vec3_i(9, 6, 3) / 3 == ggo::vec3_i(3, 2, 1));

  // Negate operator.
  static_assert(-ggo::vec2_i(6, 5) == ggo::vec2_i(-6, -5));
  static_assert(-ggo::vec3_i(6, 7, 8) == ggo::vec3_i(-6, -7, -8));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, in_place_operators)
{
  // 2D.
  ggo::vec2_i v2_1(1, 2);
  v2_1 *= { 3, 5 };
  GGO_CHECK_EQ(v2_1, ggo::vec2_i(3, 10));

  ggo::vec2_i v2_2(4, 6);
  v2_2 /= { 2, 6 };
  GGO_CHECK_EQ(v2_2, ggo::vec2_i(2, 1));

  ggo::vec2_i v2_3(1, 2);
  v2_3 += { 3, 5 };
  GGO_CHECK_EQ(v2_3, ggo::vec2_i(4, 7));

  ggo::vec2_i v2_4(4, 6);
  v2_4 -= { 1, 2 };
  GGO_CHECK_EQ(v2_4, ggo::vec2_i(3, 4));

  ggo::vec2_i v2_5(1, 2);
  v2_5 *= 3;
  GGO_CHECK_EQ(v2_5, ggo::vec2_i(3, 6));

  ggo::vec2_i v2_6(4, 6);
  v2_6 /= 2;
  GGO_CHECK_EQ(v2_6, ggo::vec2_i(2, 3));

  // 3D.
  ggo::vec3_i v3_1(1, 2, 3);
  v3_1 *= { 2, 3, 1 };
  GGO_CHECK_EQ(v3_1, ggo::vec3_i(2, 6, 3));

  ggo::vec3_i v3_2(4, 8, 6);
  v3_2 /= { 2, 1, 3 };
  GGO_CHECK_EQ(v3_2, ggo::vec3_i(2, 8, 2));

  ggo::vec3_i v3_3(1, 2, 3);
  v3_3 += { 3, 5, 2 };
  GGO_CHECK_EQ(v3_3, ggo::vec3_i(4, 7, 5));

  ggo::vec3_i v3_4(4, 6, 3);
  v3_4 -= { 1, 2, 3 };
  GGO_CHECK_EQ(v3_4, ggo::vec3_i(3, 4, 0));

  ggo::vec3_i v3_5(1, 2, 3);
  v3_5 *= 3;
  GGO_CHECK_EQ(v3_5, ggo::vec3_i(3, 6, 9));

  ggo::vec3_i v3_6(4, 8, 6);
  v3_6 /= 2;
  GGO_CHECK_EQ(v3_6, ggo::vec3_i(2, 4, 3));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, global_functions)
{
  static_assert(ggo::dot<int, 2>({ 1, 2 }, { 2, 3 }) == 8);
  static_assert(ggo::hypot<int, 2>({ 1, 2 }) == 5);

  static_assert(ggo::dot<int, 3>({ 1, 2, 2 }, { 3, 1, 2 }) == 9);
  static_assert(ggo::hypot<int, 3>({ 1, 2, 3 }) == 14);

  static_assert(ggo::ortho_dot<int>({ 1, 2 }, { 5, 4 }) == 6);

  static_assert(ggo::cross(ggo::vec3_i(1, 2, 3), ggo::vec3_i(2, 3, 4)) == ggo::vec3_i(-1, 2, -1));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, length_normalization)
{
  GGO_CHECK_FLOAT_EQ(ggo::length(ggo::vec2_f(1, 1)), std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(ggo::length(ggo::vec3_f(1, 2, 3)), std::sqrt(14.f));

  GGO_CHECK(ggo::is_normalized(ggo::vec2_f(1.f / std::sqrt(2.f), 1.f / std::sqrt(2.f))));
  GGO_CHECK(ggo::is_normalized(ggo::vec3_f(1.f, 0.f, 0.f)));

  auto v1 = ggo::normalize(ggo::vec2_f(1, 1));
  GGO_CHECK(ggo::is_normalized(v1));
  GGO_CHECK_FLOAT_EQ(v1.x(), 1.f / std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(v1.y(), 1.f / std::sqrt(2.f));

  auto v2 = ggo::normalize(ggo::vec3_f(1, 2, 3));
  GGO_CHECK(ggo::is_normalized(v2));
  GGO_CHECK_FLOAT_EQ(v2.x(), 1.f / std::sqrt(14.f));
  GGO_CHECK_FLOAT_EQ(v2.y(), 2.f / std::sqrt(14.f));
  GGO_CHECK_FLOAT_EQ(v2.z(), 3.f / std::sqrt(14.f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, distance)
{
  GGO_CHECK_FLOAT_EQ(ggo::distance(ggo::vec2_f(1, 1), ggo::vec2_f(0, 1)), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::distance(ggo::vec3_f(1, 2, 3), ggo::vec3_f(2, 3, 1)), std::sqrt(6.f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, angle_rotation)
{
  auto v1 = ggo::vec2_f::from_angle(ggo::pi<float>() / 4);
  GGO_CHECK_FLOAT_EQ(v1.x(), 1 / std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(v1.y(), 1 / std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(ggo::angle(v1), ggo::pi<float>() / 4);

  ggo::vec2_f v2(1, 0);
  v2 = ggo::rotate(v2, 3 * ggo::pi<float>() / 4);
  GGO_CHECK_FLOAT_EQ(v2.x(), -1 / std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(v2.y(), 1 / std::sqrt(2.f));
  GGO_CHECK_FLOAT_EQ(ggo::angle(v2), 3 * ggo::pi<float>() / 4);

  ggo::vec2_f v3(1, 0);
  v3 = ggo::rotate(v3, { 0, 1 }, ggo::pi<float>() / 2);
  GGO_CHECK_FLOAT_EQ(v3.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(v3.y(), 2.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, dump)
{
  ggo::vec2_i v(1, 2);

  std::ostringstream oss;
  oss << v;

  GGO_CHECK(oss.str() == "(1; 2)");
}
