#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/linear_algebra/ggo_gaussian_elimination.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_elimination, 3x3)
{
  const ggo::array2_32f A({
    { 2.f, 2.f, 2.f },
    { 1.f, -1.f,1.f },
    { 0.f, 1.f, 1.f } });
  const ggo::array_32f b({
    12.f,
    2.f, 
    5.f });

  auto s = ggo::gaussian_elimination(A, b);
  GGO_CHECK_EQ(s(0), 1.f);
  GGO_CHECK_EQ(s(1), 2.f);
  GGO_CHECK_EQ(s(2), 3.f);
}

