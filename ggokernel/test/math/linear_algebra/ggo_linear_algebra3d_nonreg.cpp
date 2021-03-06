#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/linear_algebra/ggo_linear_algebra3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra3d, solution)
{
  const float m[3][3] = {{2, 3, -1}, {-1, 1, 1}, {1, 0, 5}};
  const float c[3] = {-9, 2, 26};
  float s[3] = {0, 0, 0};

  GGO_CHECK(ggo::linsolve3d(m, c, s) == true);
  GGO_CHECK_FLOAT_EQ(s[0], 1);
  GGO_CHECK_FLOAT_EQ(s[1], -2);
  GGO_CHECK_FLOAT_EQ(s[2], 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra3d, no_solution)
{
  const float m[3][3] = { { 2, 2, 2 },{ 4, 4, 4 },{ 1, 0, 5 } };
  const float c[3] = { 1, 2, 3 };
  float s[3] = { 0, 0, 0 };

  GGO_CHECK(ggo::linsolve3d(m, c, s) == false);
}