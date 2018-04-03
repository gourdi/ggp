#include <ggo_nonreg.h>
#include <ggo_linear_algebra2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra2d, determinant)
{
  GGO_CHECK_FLOAT_EQ(ggo::determinant(2, -5, -1, 2), -1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra2d, solve)
{
  const float m[2][2] = {{2, -5}, {-1, 2}};
  const float c[2] = {12, -5};
  float s[2] = {0, 0};
  
  GGO_CHECK(ggo::linsolve2d(m, c, s) == true);
  GGO_CHECK_FLOAT_EQ(s[0], 1);
  GGO_CHECK_FLOAT_EQ(s[1], -2);
}