#include "ggo_kernel_nonreg.h"
#include <ggo_vec.h>
#include <sstream>

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, contruction_copy)
{
  ggo::vec<int, 2> v1(2, 3);
  GGO_CHECK_EQ(v1.get<0>(), 2);
  GGO_CHECK_EQ(v1.get<1>(), 3);

  ggo::vec<int, 2> v2(v1);
  GGO_CHECK_EQ(v2.get<0>(), 2);
  GGO_CHECK_EQ(v2.get<1>(), 3);
  
  v2.set(8, 9);
  v1 = v2;
  GGO_CHECK_EQ(v1.get<0>(), 8);
  GGO_CHECK_EQ(v1.get<1>(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, set_get)
{
  ggo::vec<int, 2> v;
  v.set<0>(5);
  v.set<1>(6);
  GGO_CHECK_EQ(v.get<0>(), 5);
  GGO_CHECK_EQ(v.get<1>(), 6);

  v.set(8, 9);
  GGO_CHECK_EQ(v.get<0>(), 8);
  GGO_CHECK_EQ(v.get<1>(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, egality)
{
  ggo::vec<int, 3> v1(1, 2, 3);
  ggo::vec<int, 3> v2(1, 2, 3);
  ggo::vec<int, 3> v3(1, 2, 4);
  GGO_CHECK(v1 == v2);
  GGO_CHECK(v1 != v3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, dot)
{
  ggo::vec<int, 3> a(1, 2, 3);
  ggo::vec<int, 3> b(1, 1, 2);

  GGO_CHECK_EQ(9, ggo::dot(a, b));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, operators)
{
  {
    ggo::vec<int, 3> a(1, 2, 3);
    ggo::vec<int, 3> b(-a);

    GGO_CHECK_EQ(-1, b.get<0>());
    GGO_CHECK_EQ(-2, b.get<1>());
    GGO_CHECK_EQ(-3, b.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);
    ggo::vec<int, 3> b(1, 4, 2);

    a += b;

    GGO_CHECK_EQ(2, a.get<0>());
    GGO_CHECK_EQ(6, a.get<1>());
    GGO_CHECK_EQ(5, a.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);
    ggo::vec<int, 3> b(1, 1, 2);

    a -= b;

    GGO_CHECK_EQ(0, a.get<0>());
    GGO_CHECK_EQ(1, a.get<1>());
    GGO_CHECK_EQ(1, a.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);

    a *= 3;

    GGO_CHECK_EQ(3, a.get<0>());
    GGO_CHECK_EQ(6, a.get<1>());
    GGO_CHECK_EQ(9, a.get<2>());
  }

  {
    ggo::vec<float, 3> a(1.f, 2.f, 3.f);

    a /= 2.f;

    GGO_CHECK_EQ(0.5f, a.get<0>());
    GGO_CHECK_EQ(1.0f, a.get<1>());
    GGO_CHECK_EQ(1.5f, a.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);
    ggo::vec<int, 3> b(4, 5, 6);
    ggo::vec<int, 3> c(a + b);

    GGO_CHECK_EQ(5, c.get<0>());
    GGO_CHECK_EQ(7, c.get<1>());
    GGO_CHECK_EQ(9, c.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);
    ggo::vec<int, 3> b(4, 7, 9);
    ggo::vec<int, 3> c(b - a);

    GGO_CHECK_EQ(3, c.get<0>());
    GGO_CHECK_EQ(5, c.get<1>());
    GGO_CHECK_EQ(6, c.get<2>());
  }

  {
    ggo::vec<int, 3> a(1, 2, 3);
    
    ggo::vec<int, 3> b(2 * a);
    GGO_CHECK_EQ(2, b.get<0>());
    GGO_CHECK_EQ(4, b.get<1>());
    GGO_CHECK_EQ(6, b.get<2>());

    ggo::vec<int, 3> c(a * 3);
    GGO_CHECK_EQ(3, c.get<0>());
    GGO_CHECK_EQ(6, c.get<1>());
    GGO_CHECK_EQ(9, c.get<2>());
  }

  {
    ggo::vec<float, 3> a(1.f, 2.f, 3.f);
    ggo::vec<float, 3> b(a / 2.f);
    GGO_CHECK_FABS(0.5f, b.get<0>());
    GGO_CHECK_FABS(1.0f, b.get<1>());
    GGO_CHECK_FABS(1.5f, b.get<2>());
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, flip)
{
  ggo::vec<int, 2> v(1, 2);
  v.flip();
  GGO_CHECK_EQ(-1, v.get<0>());
  GGO_CHECK_EQ(-2, v.get<1>());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, length)
{
  ggo::vec<float, 2> v(1.f, 2.f);
  GGO_CHECK_FABS(5.f, v.get_hypot());
  GGO_CHECK_FABS(std::sqrt(5.f), v.get_length());

  v.normalize();
  GGO_CHECK(v.is_normalized());
  GGO_CHECK_FABS(1.f / std::sqrt(5.f), v.get<0>());
  GGO_CHECK_FABS(2.f / std::sqrt(5.f), v.get<1>());

  auto v2 = v.get_normalized();
  GGO_CHECK(v2.is_normalized());
  GGO_CHECK_FABS(1.f / std::sqrt(5.f), v2.get<0>());
  GGO_CHECK_FABS(2.f / std::sqrt(5.f), v2.get<1>());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, compare)
{
  ggo::vec<float, 2> v1(1.f, 2.f);
  ggo::vec<float, 2> v2(1.f, 2.001f);

  GGO_CHECK(ggo::compare(v1, v2, 0.00001f) == false);
  GGO_CHECK(ggo::compare(v1, v2, 0.01f) == true);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, dump)
{
  ggo::vec<int, 2> v(1, 2);

  std::ostringstream oss;
  oss << v;

  GGO_CHECK(oss.str() == "(1; 2)");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(vec, rotation)
{
  {
    ggo::vec<float, 2> v = ggo::rotate(ggo::vec<float, 2>(1.f, 0.f), ggo::PI<float>() / 2);
    GGO_CHECK_FABS(0.f, v.get<0>());
    GGO_CHECK_FABS(1.f, v.get<1>());
  }

  {
    const ggo::vec<float, 2> center(3.f, 1.f);
    ggo::vec<float, 2> v = ggo::rotate(ggo::vec<float, 2>(4.f, 1.f), center, ggo::PI<float>() / 2);
    GGO_CHECK_FABS(3.f, v.get<0>());
    GGO_CHECK_FABS(2.f, v.get<1>());
  }
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, ortho_dot)
{
  GGO_CHECK_FABS(ggo::ortho_dot(ggo::vec<float, 2>(1.f, 2.f), ggo::vec<float, 2>(3.f, 4.f)), 2.f);
  GGO_CHECK_FABS(ggo::ortho_dot(ggo::vec<float, 2>(2.f, 1.f), ggo::vec<float, 2>(5.f, 4.f)), -3.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, cross)
{
  auto c = cross(ggo::vec<int, 3>(1, 2, 3), ggo::vec<int, 3>(2, 3, 4));

  GGO_CHECK_EQ(-1, c.get<0>());
  GGO_CHECK_EQ( 2, c.get<1>());
  GGO_CHECK_EQ(-1, c.get<0>());
}

//////////////////////////////////////////////////////////////////
GGO_TEST(vec, basis)
{
  {
    ggo::vec<float, 3> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3> v3(0.f, 0.0f, 1.0f);
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == true);
  }

  {
    ggo::vec<float, 3> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3> v3(0.f, 0.0f, -1.0f);
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == false);
  }

  {
    ggo::vec<float, 3> v1(1.f, 0.0f, 0.0f);
    ggo::vec<float, 3> v2(0.f, 1.0f, 0.0f);
    ggo::vec<float, 3> v3(0.f, 1.0f / std::sqrt(2.f), 1.0f / std::sqrt(2.f));
    GGO_CHECK(ggo::is_basis(v1, v2, v3) == false);
  }

  {
    ggo::vec<float, 3> v(0.f, 0.f, 1.f);
    auto basis = ggo::build_basis(v);
    GGO_CHECK(ggo::is_basis(v, basis.first, basis.second));
    GGO_CHECK_FABS(0.f, basis.first.get<0>());
    GGO_CHECK_FABS(1.f, basis.first.get<1>());
    GGO_CHECK_FABS(0.f, basis.first.get<2>());
    GGO_CHECK_FABS(-1.f, basis.second.get<0>());
    GGO_CHECK_FABS(0.f, basis.second.get<1>());
    GGO_CHECK_FABS(0.f, basis.second.get<2>());
  }
}

