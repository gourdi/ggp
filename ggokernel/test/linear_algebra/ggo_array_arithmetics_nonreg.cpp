#include <ggo_nonreg.h>
#include <ggo_array_arithmetics.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply1)
{
  ggo::array<int, 2> m(2, 2);
  ggo::array<int, 2> v(2, 1);

  // Remember the first index is the row (y) and the second one is the column (x).
  m(0, 0) = 1;
  m(0, 1) = 2;
  m(1, 0) = -1;
  m(1, 1) = 3;

  v(0, 0) = 1;
  v(1, 0) = -2;

  auto r = m * v;

  GGO_CHECK_EQ(r(0, 0), -3);
  GGO_CHECK_EQ(r(1, 0), -7);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply2)
{
  ggo::array<int, 2> m1(3, 2);
  ggo::array<int, 2> m2(2, 4);

  // Remember the first index is the row (y) and the second one is the column (x).
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = -1;
  m1(2, 0) = 2;
  m1(2, 1) = 4;

  m2(0, 0) = 1;
  m2(0, 1) = 2;
  m2(0, 2) = 4;
  m2(0, 3) = 3;
  m2(1, 0) = -2;
  m2(1, 1) = 1;
  m2(1, 2) = -3;
  m2(1, 3) = 1;

  auto m = m1 * m2;

  GGO_CHECK_EQ(m(0, 0), -3);
  GGO_CHECK_EQ(m(0, 1), 4);
  GGO_CHECK_EQ(m(0, 2), -2);
  GGO_CHECK_EQ(m(0, 3), 5);
  GGO_CHECK_EQ(m(1, 0), 5);
  GGO_CHECK_EQ(m(1, 1), 5);
  GGO_CHECK_EQ(m(1, 2), 15);
  GGO_CHECK_EQ(m(1, 3), 8);
  GGO_CHECK_EQ(m(2, 0), -6);
  GGO_CHECK_EQ(m(2, 1), 8);
  GGO_CHECK_EQ(m(2, 2), -4);
  GGO_CHECK_EQ(m(2, 3), 10);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, add)
{
  ggo::array<int, 2> m1(3, 2);
  ggo::array<int, 2> m2(3, 2);

  // Remember the first index is the row (y) and the second one is the column (x).
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = -1;
  m1(2, 0) = 2;
  m1(2, 1) = 4;

  m2(0, 0) = 2;
  m2(0, 1) = 1;
  m2(1, 0) = -1;
  m2(1, 1) = 3;
  m2(2, 0) = 1;
  m2(2, 1) = -2;

  auto m = m1 + m2;

  GGO_CHECK_EQ(m(0, 0), 3);
  GGO_CHECK_EQ(m(0, 1), 3);
  GGO_CHECK_EQ(m(1, 0), 2);
  GGO_CHECK_EQ(m(1, 1), 2);
  GGO_CHECK_EQ(m(2, 0), 3);
  GGO_CHECK_EQ(m(2, 1), 2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, sub)
{
  ggo::array<int, 2> m1(2, 2);
  ggo::array<int, 2> m2(2, 2);

  // Remember the first index is the row (y) and the second one is the column (x).
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = -1;

  m2(0, 0) = 2;
  m2(0, 1) = 1;
  m2(1, 0) = -1;
  m2(1, 1) = 3;

  auto m = m1 - m2;

  GGO_CHECK_EQ(m(0, 0), -1);
  GGO_CHECK_EQ(m(0, 1), 1);
  GGO_CHECK_EQ(m(1, 0), 4);
  GGO_CHECK_EQ(m(1, 1), -4);
}

