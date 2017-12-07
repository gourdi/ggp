#include "ggo_nonreg.h"
#include <ggo_quadratic.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(quadratic_solve, quadratic_solve)
{
	float sol1, sol2;

	GGO_CHECK(ggo::quadratic_solve<float>(-1, 6, -10, sol1, sol2) == false);
	
	GGO_CHECK(ggo::quadratic_solve<float>(1, 4, -21, sol1, sol2) == true);
  GGO_CHECK_FLOAT_EQ(sol1, 3);
  GGO_CHECK_FLOAT_EQ(sol2, -7);
	
	GGO_CHECK(ggo::quadratic_solve<float>(9, 6, 1, sol1, sol2) == true);
  GGO_CHECK_FLOAT_EQ(sol1, -1.f / 3);
  GGO_CHECK_FLOAT_EQ(sol2, -1.f / 3);
}
