#include <ggo_nonreg.h>
#include <ggo_matrix.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, addition)
{
  ggo::matrix<3, 2, int> m, m1, m2;
  
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
  ggo::matrix<2, 3, int> m, m1, m2;
  
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
  ggo::matrix<2, 3, int> m;
  
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
  ggo::matrix<4, 3, int> m1;
  ggo::matrix<3, 2, int> m2;
  ggo::matrix<4, 2, int> m;
  
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
GGO_TEST(matrix, diagonally_dominant)
{
  ggo::square_matrix<3, int> m;
  
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
GGO_TEST(matrix, matrix2d)
{
  ggo::square_matrix2d<int> m;
  ggo::vec2<int>            v;
  
  m(0, 0) = 5; m(0, 1) =  9;
  m(1, 0) = 2; m(1, 1) = -3;
  
  v.get<0>() =  2;
  v.get<1>() = -3;
  
  v = m * v;
  
  GGO_CHECK(v.get<0>() == -17);
  GGO_CHECK(v.get<1>() ==  13);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(matrix, symmetric)
{
  {
    auto compare_func = [](int v1, int v2) { return v1 == v2; };
    
    ggo::square_matrix<3, int> m;
    m(0, 0) = 1; m(1, 0) = 2; m(2, 0) = 3;
    m(0, 1) = 2; m(1, 1) = 7; m(2, 1) = 6;
    m(0, 2) = 3; m(1, 2) = 6; m(2, 2) = 2;
    
    GGO_CHECK(m.is_symmetric(compare_func));
    
    m(0, 2) = 4;
    GGO_CHECK(!m.is_symmetric(compare_func));
  }
  
  // Same with floats.
  {
    auto compare_func = [](float v1, float v2) { return std::abs(v1 - v2) < 0.0001f; };
    
    ggo::square_matrix<3, float> m;
    m(0, 0) = 1; m(1, 0) = 2; m(2, 0) = 3;
    m(0, 1) = 2; m(1, 1) = 7; m(2, 1) = 6;
    m(0, 2) = 3; m(1, 2) = 6; m(2, 2) = 2;
    
    GGO_CHECK(m.is_symmetric(compare_func));
    
    m(0, 2) = 3.1f;
    GGO_CHECK(!m.is_symmetric(compare_func));
  }
}
