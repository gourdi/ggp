#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, copy)
{
  {
    std::array<float, 3> src{ 1.f, 2.f, 3.f };
    std::array<float, 3> dst{ 0.f, 0.f, 0.f };

    ggo::copy<3>(dst.data(), src.data());

    GGO_CHECK_FABS(dst[0], 1.f);
    GGO_CHECK_FABS(dst[1], 2.f);
    GGO_CHECK_FABS(dst[2], 3.f);
  }

  {
    std::array<int, 4> src{ 1, 2, 3, 4 };
    std::array<int, 4> dst{ 0, 0, 0, 0 };

    ggo::copy<3>(dst.data(), src.data()); // Do not copy last coef.

    GGO_CHECK(dst[0] == 1);
    GGO_CHECK(dst[1] == 2);
    GGO_CHECK(dst[2] == 3);
    GGO_CHECK(dst[3] == 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, set)
{
  {
    std::array<float, 3> a{ 0.f, 0.f, 0.f };

    ggo::set(a.data(), 1.f, 2.f, 3.f);

    GGO_CHECK_FABS(a[0], 1.f);
    GGO_CHECK_FABS(a[1], 2.f);
    GGO_CHECK_FABS(a[2], 3.f);
  }

  {
    std::array<int, 4> a{ 0, 0, 0, 0 };

    ggo::set(a.data(), 1, 2, 3); // Do not set the last coef.

    GGO_CHECK(a[0] == 1);
    GGO_CHECK(a[1] == 2);
    GGO_CHECK(a[2] == 3);
    GGO_CHECK(a[3] == 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, gor_each)
{
  {
    std::array<int, 3> a{ 1, 2, 3 };
    ggo::for_each<3>(a.data(), [](int & v) { v *= 2; });
    GGO_CHECK_EQ(2, a[0]);
    GGO_CHECK_EQ(4, a[1]);
    GGO_CHECK_EQ(6, a[2]);
  }

  {
    std::array<float, 3> a{ 1.f, 2.f, 3.f };
    ggo::for_each<3>(a.data(), [](float & v) { v = -v; });
    GGO_CHECK_FABS(-1.f, a[0]);
    GGO_CHECK_FABS(-2.f, a[1]);
    GGO_CHECK_FABS(-3.f, a[2]);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, unary_operation)
{
  {
    std::array<int, 4> a{ 1, 2, 3, 4 };
    std::array<int, 4> b{ 0, 0, 0, 0 };

    ggo::unary_operation<4>(b.data(), a.data(), [](int & dst, int src) { dst = -src; });

    GGO_CHECK(b[0] == -1);
    GGO_CHECK(b[1] == -2);
    GGO_CHECK(b[2] == -3);
    GGO_CHECK(b[3] == -4);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, binary_operation)
{
  {
    std::array<int, 4> a{ 1, 2, 3, 4 };
    std::array<int, 4> b{ 2, 4, 1, 3 };
    std::array<int, 4> c{ 0, 0, 0, 0 };

    ggo::binary_operation<4>(c.data(), a.data(), b.data(), [](int & dst, int src1, int src2) { dst = src1 + src2; });

    GGO_CHECK(c[0] == 3);
    GGO_CHECK(c[1] == 6);
    GGO_CHECK(c[2] == 4);
    GGO_CHECK(c[3] == 7);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, compare)
{
  std::array<int, 3> a{ 0, 1, 2 };
  std::array<int, 3> b{ 0, 1, 2 };
  std::array<int, 3> c{ 0, 1, 3 };

  GGO_CHECK(ggo::compare<3>(a.data(), b.data()) == true);
  GGO_CHECK(ggo::compare<3>(a.data(), c.data()) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, dot)
{
  std::array<int, 3> a{ 1, 2, 3 };
  std::array<int, 3> b{ 1, 1, 2 };

  GGO_CHECK_EQ(9, ggo::dot<3>(a.data(), b.data()));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, multiply_all)
{
  std::array<int, 3> a{ 2, 3, 5 };
  int v = ggo::multiply_all<3>(a.data());

  GGO_CHECK_EQ(v, 30);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_pointer, fill)
{
  std::array<int, 3> a{ 0, 0, 0 };
  ggo::fill<3>(a.data(), 1);

  GGO_CHECK_EQ(a[0], 1);
  GGO_CHECK_EQ(a[1], 1);
  GGO_CHECK_EQ(a[2], 1);
}

