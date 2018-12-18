#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/ggo_vec3.h>
#include <kernel/math/linear_algebra/ggo_matrix.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, constuction)
{
#ifdef GGO_MATRIX
  constexpr ggo::matrix<int, 3, 2> m({
    { 3, 2 },
    { 5, 7 },
    { 8, 1 } });

  // First column.
  static_assert(m(0, 0) == 3);
  static_assert(m(1, 0) == 5);
  static_assert(m(2, 0) == 8);

  // Second column.
  static_assert(m(0, 1) == 2);
  static_assert(m(1, 1) == 7);
  static_assert(m(2, 1) == 1);
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, equality)
{
#ifdef GGO_MATRIX
  constexpr ggo::matrix<int, 2, 3> m1({
    { 4, 2, 3 },
    { 1, 3, 2 } });

  constexpr ggo::matrix<int, 2, 3> m2({
    { 4, 2, 3 },
    { 1, 3, 2 } });

  constexpr ggo::matrix<int, 2, 3> m3({
    { 0, 2, 3 },
    { 1, 3, 2 } });

  constexpr ggo::matrix<int, 2, 3> m4({
    { 4, 2, 3 },
    { 1, 3, 0 } });

  static_assert(m1 == m2);
  static_assert(m1 != m3);
  static_assert(m1 != m4);
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, multiplication)
{
#ifdef GGO_MATRIX
  constexpr ggo::matrix<int, 4, 3> m1({
    { 14,  9,  3 },
    {  2, 11, 15 },
    {  0, 12, 17 },
    {  5,  2,  3 } });

  constexpr ggo::matrix<int, 3, 2> m2({
    { 12, 25 },
    {  9, 10 },
    {  8,  5 } });

  constexpr ggo::matrix<int, 4, 2> m({
    { 273, 455 },
    { 243, 235 },
    { 244, 205 },
    { 102, 160 } });

  static_assert(m == m1 * m2);
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, multiplication_vec)
{
#ifdef GGO_MATRIX
  {
    constexpr ggo::square_matrix<int, 2> m({
      { 3, 5 },
      { 7, 6 } });
    constexpr ggo::vec2_i v({ 49, 36 });
    static_assert(m * v == ggo::vec2_i({ 327, 559 }));
  }
  {
    constexpr ggo::square_matrix<int, 3> m({
      { 3, 5, 2 },
      { 7, 9, 1 },
      { 5, 2, 8 } });
    constexpr ggo::vec3_i v({ 11, 45, 73 });
    static_assert(m * v == ggo::vec3_i({ 404, 555, 729 }));
  }
#endif
}


/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, is_symmetric)
{
#ifdef GGO_MATRIX
  {
    constexpr ggo::square_matrix<int, 3> m({
      { 1, 2, 3 },
      { 2, 4, 5 },
      { 3, 5, 6 } });
    static_assert(ggo::is_symmetric(m) == true);
  }

  {
    constexpr ggo::square_matrix<int, 3> m({
      { 1, 2, 3 },
      { 2, 4, 5 },
      { 3, 7, 6 } });
    static_assert(ggo::is_symmetric(m) == false);
  }
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, det_2d)
{
#ifdef GGO_MATRIX
  constexpr ggo::square_matrix2d<int> m({
    { 1, 2 },
    { 3, 4 } });
  static_assert(ggo::det(m) == -2);
#endif
}


