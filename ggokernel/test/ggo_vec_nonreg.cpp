#include "ggo_kernel_nonreg.h"
#include <kernel/ggo_vec.h>
#include <sstream>

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, contruction_copy)
{
  ggo::vec<int, 2, ggo::geometry_t> v1(2, 3);
  GGO_CHECK_EQ(v1.x(), 2);
  GGO_CHECK_EQ(v1.y(), 3);

  ggo::vec<int, 2, ggo::geometry_t> v2(v1);
  GGO_CHECK_EQ(v2.x(), 2);
  GGO_CHECK_EQ(v2.y(), 3);
  
  v2.set(8, 9);
  v1 = v2;
  GGO_CHECK_EQ(v1.x(), 8);
  GGO_CHECK_EQ(v1.y(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, set_get)
{
  ggo::vec<int, 2, ggo::geometry_t> v;
  v.x() = 5;
  v.y() = 6;
  GGO_CHECK_EQ(v.x(), 5);
  GGO_CHECK_EQ(v.y(), 6);

  v.set(8, 9);
  GGO_CHECK_EQ(v.x(), 8);
  GGO_CHECK_EQ(v.y(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, egality)
{
  ggo::vec<int, 3, ggo::geometry_t> v1(1, 2, 3);
  ggo::vec<int, 3, ggo::geometry_t> v2(1, 2, 3);
  ggo::vec<int, 3, ggo::geometry_t> v3(1, 2, 4);
  GGO_CHECK(v1 == v2);
  GGO_CHECK(v1 != v3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, dot)
{
  ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
  ggo::vec<int, 3, ggo::geometry_t> b(1, 1, 2);

  GGO_CHECK_EQ(9, ggo::dot(a, b));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, operators)
{
  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    ggo::vec<int, 3, ggo::geometry_t> b(-a);

    GGO_CHECK_EQ(-1, b.x());
    GGO_CHECK_EQ(-2, b.y());
    GGO_CHECK_EQ(-3, b.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    ggo::vec<int, 3, ggo::geometry_t> b(1, 4, 2);

    a += b;

    GGO_CHECK_EQ(2, a.x());
    GGO_CHECK_EQ(6, a.y());
    GGO_CHECK_EQ(5, a.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    ggo::vec<int, 3, ggo::geometry_t> b(1, 1, 2);

    a -= b;

    GGO_CHECK_EQ(0, a.x());
    GGO_CHECK_EQ(1, a.y());
    GGO_CHECK_EQ(1, a.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);

    a *= 3;

    GGO_CHECK_EQ(3, a.x());
    GGO_CHECK_EQ(6, a.y());
    GGO_CHECK_EQ(9, a.z());
  }

  {
    ggo::vec<float, 3, ggo::geometry_t> a(1.f, 2.f, 3.f);

    a /= 2.f;

    GGO_CHECK_EQ(0.5f, a.x());
    GGO_CHECK_EQ(1.0f, a.y());
    GGO_CHECK_EQ(1.5f, a.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    ggo::vec<int, 3, ggo::geometry_t> b(4, 5, 6);
    ggo::vec<int, 3, ggo::geometry_t> c(a + b);

    GGO_CHECK_EQ(5, c.x());
    GGO_CHECK_EQ(7, c.y());
    GGO_CHECK_EQ(9, c.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    ggo::vec<int, 3, ggo::geometry_t> b(4, 7, 9);
    ggo::vec<int, 3, ggo::geometry_t> c(b - a);

    GGO_CHECK_EQ(3, c.x());
    GGO_CHECK_EQ(5, c.y());
    GGO_CHECK_EQ(6, c.z());
  }

  {
    ggo::vec<int, 3, ggo::geometry_t> a(1, 2, 3);
    
    ggo::vec<int, 3, ggo::geometry_t> b(2 * a);
    GGO_CHECK_EQ(2, b.x());
    GGO_CHECK_EQ(4, b.y());
    GGO_CHECK_EQ(6, b.z());

    ggo::vec<int, 3, ggo::geometry_t> c(a * 3);
    GGO_CHECK_EQ(3, c.x());
    GGO_CHECK_EQ(6, c.y());
    GGO_CHECK_EQ(9, c.z());
  }

  {
    ggo::vec<float, 3, ggo::geometry_t> a(1.f, 2.f, 3.f);
    ggo::vec<float, 3, ggo::geometry_t> b(a / 2.f);
    GGO_CHECK_FLOAT_EQ(0.5f, b.x());
    GGO_CHECK_FLOAT_EQ(1.0f, b.y());
    GGO_CHECK_FLOAT_EQ(1.5f, b.z());
  }
}

/////////////////////////////////////////////////////////////////////
/*GGO_TEST(vec, argb_sum)
{
  {
    ggo::alpha_color<float> c1{ 0.f, 0.5f, 1.f, 0.5f };
    ggo::alpha_color<float> c2{ 0.f, 0.5f, 1.f, 0.5f };
    auto c3 = c1 + c2;
    GGO_CHECK_FLOAT_EQ(c3.r(), 0.f);
    GGO_CHECK_FLOAT_EQ(c3.g(), 0.5f);
    GGO_CHECK_FLOAT_EQ(c3.b(), 1.f);
    GGO_CHECK_FLOAT_EQ(c3.a(), 0.75f);
  }

  {
    ggo::alpha_color<float> c1{ 0.f, 1.f, 0.5f, 0.1f };
    ggo::alpha_color<float> c2{ 1.f, 0.f, 0.5f, 0.2f };
    auto c3 = c1 + c2;
    GGO_CHECK_FLOAT_EQ(c3.r(), 2.f / 3.f);
    GGO_CHECK_FLOAT_EQ(c3.g(), 1.f / 3.f);
    GGO_CHECK_FLOAT_EQ(c3.b(), 0.5f);
    GGO_CHECK_FLOAT_EQ(c3.a(), 0.28f);
  }

  {
    ggo::alpha_color<float> c1{ 0.f, 1.f, 0.5f, 0.f };
    ggo::alpha_color<float> c2{ 1.f, 0.f, 0.5f, 0.f };
    auto c3 = c1 + c2;
    GGO_CHECK_FLOAT_EQ(c3.r(), 0.f);
    GGO_CHECK_FLOAT_EQ(c3.g(), 0.f);
    GGO_CHECK_FLOAT_EQ(c3.b(), 0.f);
    GGO_CHECK_FLOAT_EQ(c3.a(), 0.f);
  }
}*/

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, flip)
{
  ggo::vec<int, 2, ggo::geometry_t> v(1, 2);
  v.flip();
  GGO_CHECK_EQ(-1, v.x());
  GGO_CHECK_EQ(-2, v.y());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, length)
{
  ggo::vec<float, 2, ggo::geometry_t> v(1.f, 2.f);
  GGO_CHECK_FLOAT_EQ(5.f, v.get_hypot());
  GGO_CHECK_FLOAT_EQ(std::sqrt(5.f), v.get_length());

  v.normalize();
  GGO_CHECK(v.is_normalized());
  GGO_CHECK_FLOAT_EQ(1.f / std::sqrt(5.f), v.x());
  GGO_CHECK_FLOAT_EQ(2.f / std::sqrt(5.f), v.y());

  auto v2 = v.get_normalized();
  GGO_CHECK(v2.is_normalized());
  GGO_CHECK_FLOAT_EQ(1.f / std::sqrt(5.f), v2.x());
  GGO_CHECK_FLOAT_EQ(2.f / std::sqrt(5.f), v2.y());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, compare)
{
  ggo::vec<float, 2, ggo::geometry_t> v1(1.f, 2.f);
  ggo::vec<float, 2, ggo::geometry_t> v2(1.f, 2.001f);

  GGO_CHECK(ggo::compare(v1, v2, 0.00001f) == false);
  GGO_CHECK(ggo::compare(v1, v2, 0.01f) == true);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, dump)
{
  ggo::vec<int, 2, ggo::geometry_t> v(1, 2);

  std::ostringstream oss;
  oss << v;

  GGO_CHECK(oss.str() == "(1; 2)");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, rotation)
{
  {
    ggo::vec<float, 2, ggo::geometry_t> v = ggo::rotate(ggo::vec<float, 2, ggo::geometry_t>(1.f, 0.f), ggo::pi<float>() / 2);
    GGO_CHECK_FLOAT_EQ(0.f, v.x());
    GGO_CHECK_FLOAT_EQ(1.f, v.y());
  }

  {
    const ggo::vec<float, 2, ggo::geometry_t> center(3.f, 1.f);
    ggo::vec<float, 2, ggo::geometry_t> v = ggo::rotate(ggo::vec<float, 2, ggo::geometry_t>(4.f, 1.f), center, ggo::pi<float>() / 2);
    GGO_CHECK_FLOAT_EQ(3.f, v.x());
    GGO_CHECK_FLOAT_EQ(2.f, v.y());
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, move)
{
  ggo::vec<int, 2, ggo::geometry_t> v(1, 2);
  v.move(3, 4);
  GGO_CHECK_FLOAT_EQ(4, v.x());
  GGO_CHECK_FLOAT_EQ(6, v.y());
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, ortho_dot)
{
  GGO_CHECK_FLOAT_EQ(ggo::ortho_dot(ggo::vec<float, 2, ggo::geometry_t>(1.f, 2.f), ggo::vec<float, 2, ggo::geometry_t>(3.f, 4.f)), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::ortho_dot(ggo::vec<float, 2, ggo::geometry_t>(2.f, 1.f), ggo::vec<float, 2, ggo::geometry_t>(5.f, 4.f)), -3.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, cross)
{
  auto c = cross(ggo::vec<int, 3, ggo::geometry_t>(1, 2, 3), ggo::vec<int, 3, ggo::geometry_t>(2, 3, 4));

  GGO_CHECK_EQ(-1, c.x());
  GGO_CHECK_EQ( 2, c.y());
  GGO_CHECK_EQ(-1, c.x());
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, basis)
{
  {
    ggo::vec<float, 3, ggo::geometry_t> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v3(0.f, 0.0f, 1.0f);
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == true);
  }

  {
    ggo::vec<float, 3, ggo::geometry_t> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v3(0.f, 0.0f, -1.0f);
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == false);
  }

  {
    ggo::vec<float, 3, ggo::geometry_t> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3, ggo::geometry_t> v3(0.f, 1.0f / std::sqrt(2.f), 1.0f / std::sqrt(2.f));
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == false);
  }

  {
    ggo::vec<float, 3, ggo::geometry_t> v(0.f, 0.f, 1.f);
    auto basis = ggo::build_basis(v);
    GGO_CHECK(ggo::is_basis(v, basis.first, basis.second));
    GGO_CHECK_FLOAT_EQ(0.f, basis.first.x());
    GGO_CHECK_FLOAT_EQ(1.f, basis.first.y());
    GGO_CHECK_FLOAT_EQ(0.f, basis.first.z());
    GGO_CHECK_FLOAT_EQ(-1.f, basis.second.x());
    GGO_CHECK_FLOAT_EQ(0.f, basis.second.y());
    GGO_CHECK_FLOAT_EQ(0.f, basis.second.z());
  }
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, angle)
{
  GGO_CHECK_FLOAT_EQ(ggo::get_angle(ggo::vec2f(2.f, 0.f), ggo::vec2f(0.f, 2.f)), ggo::pi<float>()/ 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::get_angle(ggo::vec2f(2.f, 0.f), ggo::vec2f(0.f, -2.f)), ggo::pi<float>() / 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::get_angle(ggo::vec2f(-2.f, 0.f), ggo::vec2f(0.f, -2.f)), ggo::pi<float>() / 2.f);
}

