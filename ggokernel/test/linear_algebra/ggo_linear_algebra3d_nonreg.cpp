#include <ggo_nonreg.h>
#include <ggo_linear_algebra3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra3d, linear_algebra3d)
{
  const float m[3][3] = {{2, 3, -1}, {-1, 1, 1}, {1, 0, 5}};
  const float c[3] = {-9, 2, 26};
  float s[3] = {0, 0, 0};
  
  GGO_CHECK(ggo::linsolve3d(m, c, s) == true);
  GGO_CHECK_FABS(s[0], 1);
  GGO_CHECK_FABS(s[1], -2);
  GGO_CHECK_FABS(s[2], 5);
}