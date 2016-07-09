#include <ggo_nonreg.h>
#include <ggo_gauss_seidel.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gauss_seidel, solve2x2)
{
  // C array
  {
    const float a[2][2] = { { 16.f, 3.f },{ 7.f, -11.f } };
    const float b[2] = { 11.f, 13.f };
    float s[2] = { 1.f, 1.f };

    ggo::gauss_seidel(a, b, s, 20);

    GGO_CHECK_FABS(s[0], 0.8122f);
    GGO_CHECK_FABS(s[1], -0.6650f);
  }

  // ggo::matrix and ggo::vec (size is known at compile time).
  {
    ggo::square_matrix<float, 2> a;
    a(0, 0) = 16.f;
    a(0, 1) = 3.f;
    a(1, 0) = 7.f;
    a(1, 1) = -11.f;
    ggo::vec<float, 2> b { 11.f, 13.f };
    ggo::vec<float, 2> s { 1.f, 1.f };

    ggo::gauss_seidel(a, b, s, 20);

    GGO_CHECK_FABS(s.get<0>(), 0.8122f);
    GGO_CHECK_FABS(s.get<1>(), -0.6650f);
  }

  // ggo::array (size is unknown at compile time).
  {
    ggo::array<float, 2> a(2, 2);
    a(0, 0) = 16.f;
    a(0, 1) = 3.f;
    a(1, 0) = 7.f;
    a(1, 1) = -11.f;
    ggo::array<float, 1> b(2);
    b(0) = 11.f;
    b(1) = 13.f;
    ggo::array<float, 1> s(2);
    s(0) = 1.f;
    s(1) = 1.f;

    ggo::gauss_seidel(a, b, s, 20);

    GGO_CHECK_FABS(s(0), 0.8122f);
    GGO_CHECK_FABS(s(1), -0.6650f);
  }
}