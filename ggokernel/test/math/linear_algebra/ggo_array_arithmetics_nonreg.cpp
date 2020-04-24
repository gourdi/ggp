#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/linear_algebra/ggo_array_arithmetics.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, indices)
{
  const ggo::array2_i m({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  // Width is the number of columns, height is the number of rows.
  GGO_CHECK_EQ(m.dim(0), 2); 
  GGO_CHECK_EQ(m.dim(1), 3);

  // Make sure that the first index is the column one, and the second index the row one,
  // unlike the math convention.
  GGO_CHECK_EQ(m(0, 0), 1);
  GGO_CHECK_EQ(m(0, 1), 2);
  GGO_CHECK_EQ(m(0, 2), 3);
  GGO_CHECK_EQ(m(1, 0), 4);
  GGO_CHECK_EQ(m(1, 1), 5);
  GGO_CHECK_EQ(m(1, 2), 6);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply_matrix_vector)
{
  const ggo::array2_i m({
    { 1, 2 },
    { -1, 3 },
    { 2, -1 } });
  const ggo::array_i v({
    1,
    -2 });

  auto r = m * v;

  const ggo::array_i expected({
    -3,
    -7,
    4 });

  GGO_CHECK_EQ(r, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, multiply_matrix_matrix)
{
  const ggo::array2_i m1({
    { 1, 2},
    { 3, -1},
    { 2, 4} });
  const ggo::array2_i m2({
    { 1, 2, 3, 4},
    { -2, 1, -3, 1} });

  auto m = m1 * m2;

  const ggo::array2_i expected({
    { -3, 4, -3, 6 },
    { 5, 5, 12, 11 },
    { -6, 8, -6, 12 } });

  GGO_CHECK_EQ(expected, m);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, add)
{
  const ggo::array2_i m1({
    { 1, 2 },
    { 3, -1},
    { 2, 4 } });
  const ggo::array2_i m2({
    { 2, 1 },
    { -1, 3 },
    { 1, -2 } });

  auto m = m1 + m2;

  const ggo::array2_i expected({
    { 3, 3 },
    { 2, 2 },
    { 3, 2 } });

  GGO_CHECK_EQ(expected, m);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array_arithmetics, sub)
{
  const ggo::array2_i m1({
    { 1, 2 },
    { 3, -1 } });
  const ggo::array2_i m2({
    { 2, -2 },
    { -1, 3 } });

  auto m = m1 - m2;

  const ggo::array2_i expected({
    { -1, 4 },
    { 4, -4 } });

  GGO_CHECK_EQ(expected, m);
}

