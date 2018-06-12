#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/linear_algebra/ggo_array_arithmetics.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply1)
{
  const ggo::array<int, 2> m({
    { 1, 2 },
    { -1, 3 } });
  const ggo::array<int, 2> v({
    { 1 },
    { -2 } });

  auto r = m * v;

  const ggo::array<int, 2> expected({
    { -3 },
    { -7 } });

  GGO_CHECK_EQ(r, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply2)
{
  const ggo::array<int, 2> m1({
    { 1, 2},
    { 3, -1},
    { 2, 4} });
  const ggo::array<int, 2> m2({
    { 1, 2, 3, 4},
    { -2, 1, -3, 1} });

  auto m = m1 * m2;

  const ggo::array<int, 2> expected({
    { -3, 4, -3, 6 },
    { 5, 5, 12, 11 },
    { -6, 8, -6, 12 } });

  GGO_CHECK_EQ(expected, m);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, add)
{
  const ggo::array<int, 2> m1({
    { 1, 2 },
    { 3, -1},
    { 2, 4 } });
  const ggo::array<int, 2> m2({
    { 2, 1 },
    { -1, 3 },
    { 1, -2 } });

  auto m = m1 + m2;

  const ggo::array<int, 2> expected({
    { 3, 3 },
    { 2, 2 },
    { 3, 2 } });

  GGO_CHECK_EQ(expected, m);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, sub)
{
  const ggo::array<int, 2> m1({
    { 1, 2 },
    { 3, -1 } });
  const ggo::array<int, 2> m2({
    { 2, -2 },
    { -1, 3 } });

  auto m = m1 - m2;

  const ggo::array<int, 2> expected({
    { -1, 4 },
    { 4, -4 } });

  GGO_CHECK_EQ(expected, m);
}

