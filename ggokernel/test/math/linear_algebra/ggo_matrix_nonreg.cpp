#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/linear_algebra/ggo_matrix.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, constuction)
{
#ifndef _MSC_VER
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
#ifndef _MSC_VER
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
#ifndef _MSC_VER
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

#if 0
/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, addition)
{
  ggo::matrix<int, 3, 2> m, m1, m2;
  
  m1(0, 0) = -4; m1(0, 1) =  2;
  m1(1, 0) =  1; m1(1, 1) =  6;
  m1(2, 0) =  2; m1(2, 1) =  1;
  
  m2(0, 0) = 0; m2(0, 1) = -1;
  m2(1, 0) = 1; m2(1, 1) = -2;
  m2(2, 0) = 2; m2(2, 1) = -3;

  m = m1 + m2;
  GGO_CHECK(m(0, 0) == -4);
  GGO_CHECK(m(1, 0) ==  2);
  GGO_CHECK(m(2, 0) ==  4);
  GGO_CHECK(m(0, 1) ==  1);
  GGO_CHECK(m(1, 1) ==  4);
  GGO_CHECK(m(2, 1) == -2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, subtraction)
{
  ggo::matrix<int, 2, 3> m, m1, m2;
  
  m1(0, 0) = -4; m1(0, 1) =  2; m1(0, 2) = 5;
  m1(1, 0) =  1; m1(1, 1) = -2; m1(1, 2) = 2;
  
  m2(0, 0) = -2; m2(0, 1) =  3; m2(0, 2) = 4;
  m2(1, 0) = -1; m2(1, 1) = -1; m2(1, 2) = 4;

  m = m1 - m2;
  GGO_CHECK(m(0, 0) == -2);
  GGO_CHECK(m(1, 0) ==  2);
  GGO_CHECK(m(0, 1) == -1);
  GGO_CHECK(m(1, 1) == -1);
  GGO_CHECK(m(0, 2) ==  1);
  GGO_CHECK(m(1, 2) == -2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, scalar_multiplication)
{
  ggo::matrix<int, 2, 3> m;
  
  m(0, 0) = -4; m(0, 1) =  2; m(0, 2) = -2;
  m(1, 0) =  1; m(1, 1) =  6; m(1, 2) =  3;
  
  m *= 2;
  GGO_CHECK(m(0, 0) == -8);
  GGO_CHECK(m(1, 0) ==  2);
  GGO_CHECK(m(0, 1) ==  4);
  GGO_CHECK(m(1, 1) == 12);
  GGO_CHECK(m(0, 2) == -4);
  GGO_CHECK(m(1, 2) ==  6);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, multiplication)
{
  ggo::matrix<int, 4, 3> m1;
  ggo::matrix<int, 3, 2> m2;
  ggo::matrix<int, 4, 2> m;
  
  m1(0, 0) = 14; m1(0, 1) =  9; m1(0, 2) =  3; 
  m1(1, 0) =  2; m1(1, 1) = 11; m1(1, 2) = 15; 
  m1(2, 0) =  0; m1(2, 1) = 12; m1(2, 2) = 17; 
  m1(3, 0) =  5; m1(3, 1) =  2; m1(3, 2) =  3;
  
  m2(0, 0) = 12; m2(0, 1) = 25; 
  m2(1, 0) =  9; m2(1, 1) = 10; 
  m2(2, 0) =  8; m2(2, 1) =  5; 
  
  m = m1 * m2;
  GGO_CHECK(m(0, 0) == 273);
  GGO_CHECK(m(0, 1) == 455);
  GGO_CHECK(m(1, 0) == 243);
  GGO_CHECK(m(1, 1) == 235);
  GGO_CHECK(m(2, 0) == 244);
  GGO_CHECK(m(2, 1) == 205);
  GGO_CHECK(m(3, 0) == 102);
  GGO_CHECK(m(3, 1) == 160);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, symmetric)
{
  {
    ggo::square_matrix<int, 3> m;

    m(0, 0) = -4; m(0, 1) = 2; m(0, 2) = 1;
    m(1, 0) = 2; m(1, 1) = 6; m(1, 2) = -2;
    m(2, 0) = 1; m(2, 1) = -2; m(2, 2) = 5;
    GGO_CHECK(m.is_symmetric());
  }

  {
    ggo::square_matrix<float, 3> m;

    m(0, 0) = -4.f; m(0, 1) =  2.f; m(0, 2) =  1.f;
    m(1, 0) =  2.f; m(1, 1) =  6.f; m(1, 2) = -2.f;
    m(2, 0) =  1.f; m(2, 1) = -2.f; m(2, 2) =  5.f;
    GGO_CHECK(m.is_symmetric([](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, diagonally_dominant)
{
  ggo::square_matrix<int, 3> m;
  
  m(0, 0) = -4; m(0, 1) =  2; m(0, 2) = 1;
  m(1, 0) =  1; m(1, 1) =  6; m(1, 2) = 2;
  m(2, 0) =  1; m(2, 1) = -2; m(2, 2) = 5;
  GGO_CHECK(m.is_diagonally_dominant());
  
  m(0, 0) = -2; m(0, 1) =  2; m(0, 2) = 1;
  m(1, 0) =  1; m(1, 1) =  3; m(1, 2) = 2;
  m(2, 0) =  1; m(2, 1) = -2; m(2, 2) = 0;
  GGO_CHECK(!m.is_diagonally_dominant());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, matrix_vector_multiplication)
{
  //ggo::square_matrix<int, 2>  m;
  //ggo::vec2<int>              v;
  //
  //m(0, 0) = 5; m(0, 1) =  9;
  //m(1, 0) = 2; m(1, 1) = -3;
  //
  //v.x() =  2;
  //v.y() = -3;
  //
  //v = m * v;
  //
  //GGO_CHECK(v.x() == -17);
  //GGO_CHECK(v.y() ==  13);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, matrix_rotation)
{
  //ggo::square_matrix2d<float> m;
  //ggo::vec2<float>            v{ 1.f, 0.f };

  //m.set_rotation(0.5f * ggo::pi<float>());

  //v = m * v;

  //GGO_CHECK_FLOAT_EQ(v.get<0>(), 0.f);
  //GGO_CHECK_FLOAT_EQ(v.get<1>(), 1.f);
}


#endif
