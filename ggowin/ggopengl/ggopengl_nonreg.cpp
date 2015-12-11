#include <ggo_nonreg.h>
#include "ggo_matrix4x4.h"
#include <iostream>
#include <ggo_kernel.h>

void test_copy()
{
  // Test assignment (make sure matrices are copied, and not just the internal pointer).
  {
    ggo_matrix4x4 m1, m2;
    m1.set_translation(1, 2, 3);
    m2 = m1;
    GGO_CHECK(m1.compare(m2, 0.001));
    m1.set_translation(0, 0, 1);
    GGO_CHECK(m1.compare(m2, 0.001) == false);
  }
  
  // Test copy constructor (make sure matrices are copied, and not just the internal pointer).
  {
    ggo_matrix4x4 m1;
    m1.set_translation(1, 2, 3);
    ggo_matrix4x4 m2(m1);
    GGO_CHECK(m1.compare(m2, 0.001));
    m1.set_translation(0, 0, 1);
    GGO_CHECK(m1.compare(m2, 0.001) == false);
  }
}

void test_translation()
{
  ggo_matrix4x4 m;
  m.set_translation(2, -3, 5);
  
  GGO_CHECK_FABS(m(0, 0), 1);
  GGO_CHECK_FABS(m(1, 0), 0);
  GGO_CHECK_FABS(m(2, 0), 0);
  GGO_CHECK_FABS(m(3, 0), 2);
  
  GGO_CHECK_FABS(m(0, 1), 0);
  GGO_CHECK_FABS(m(1, 1), 1);
  GGO_CHECK_FABS(m(2, 1), 0);
  GGO_CHECK_FABS(m(3, 1), -3);
  
  GGO_CHECK_FABS(m(0, 2), 0);
  GGO_CHECK_FABS(m(1, 2), 0);
  GGO_CHECK_FABS(m(2, 2), 1);
  GGO_CHECK_FABS(m(3, 2), 5);
  
  GGO_CHECK_FABS(m(0, 3), 0);
  GGO_CHECK_FABS(m(1, 3), 0);
  GGO_CHECK_FABS(m(2, 3), 0);
  GGO_CHECK_FABS(m(3, 3), 1);

  std::array<float, 4> v {7, 2, 3, 1};
  v = m * v;
  
  GGO_CHECK_FABS(v[0], 9);
  GGO_CHECK_FABS(v[1], -1);
  GGO_CHECK_FABS(v[2], 8);
  GGO_CHECK_FABS(v[3], 1);
  
  ggo_matrix4x4 m2;
  m2.set_translation(3, -2, 1);
  
  ggo_matrix4x4 m3(m * m2);
  ggo_matrix4x4 m4;
  m4.set_translation(5, -5, 6);
  GGO_CHECK(m3.compare(m4, 0.002) == true);
}

void test_rotation()
{
  // X-axis
  {
    ggo_matrix4x4 m;
    m.set_rotation_x(-GGO_PI / 2);
    
    GGO_CHECK_FABS(m(0, 0), 1);
    GGO_CHECK_FABS(m(1, 0), 0);
    GGO_CHECK_FABS(m(2, 0), 0);
    GGO_CHECK_FABS(m(3, 0), 0);
    
    GGO_CHECK_FABS(m(0, 1), 0);
    GGO_CHECK_FABS(m(1, 1), 0);
    GGO_CHECK_FABS(m(2, 1), 1);
    GGO_CHECK_FABS(m(3, 1), 0);
    
    GGO_CHECK_FABS(m(0, 2), 0);
    GGO_CHECK_FABS(m(1, 2), -1);
    GGO_CHECK_FABS(m(2, 2), 0);
    GGO_CHECK_FABS(m(3, 2), 0);
    
    GGO_CHECK_FABS(m(0, 3), 0);
    GGO_CHECK_FABS(m(1, 3), 0);
    GGO_CHECK_FABS(m(2, 3), 0);
    GGO_CHECK_FABS(m(3, 3), 1);
    
    std::array<float, 4> v {5, -2, 7, 1};
    m.set_rotation_x(GGO_PI / 2);
    v = m * v;
 
    GGO_CHECK_FABS(v[0], 5);
    GGO_CHECK_FABS(v[1], -7);
    GGO_CHECK_FABS(v[2], -2);
    GGO_CHECK_FABS(v[3], 1);
  }
  
  // Y-axis
  {
    ggo_matrix4x4 m;
    m.set_rotation_y(GGO_PI / 4);
    
    GGO_CHECK_FABS(m(0, 0), std::sqrt(2) / 2);
    GGO_CHECK_FABS(m(1, 0), 0);
    GGO_CHECK_FABS(m(2, 0), std::sqrt(2) / 2);
    GGO_CHECK_FABS(m(3, 0), 0);
    
    GGO_CHECK_FABS(m(0, 1), 0);
    GGO_CHECK_FABS(m(1, 1), 1);
    GGO_CHECK_FABS(m(2, 1), 0);
    GGO_CHECK_FABS(m(3, 1), 0);
    
    GGO_CHECK_FABS(m(0, 2), -std::sqrt(2) / 2);
    GGO_CHECK_FABS(m(1, 2), 0);
    GGO_CHECK_FABS(m(2, 2), std::sqrt(2) / 2);
    GGO_CHECK_FABS(m(3, 2), 0);
    
    GGO_CHECK_FABS(m(0, 3), 0);
    GGO_CHECK_FABS(m(1, 3), 0);
    GGO_CHECK_FABS(m(2, 3), 0);
    GGO_CHECK_FABS(m(3, 3), 1);
    
    std::array<float, 4> v {5, -2, 7, 1};
    m.set_rotation_y(GGO_PI / 2);
    v = m * v;
 
    GGO_CHECK_FABS(v[0], 7);
    GGO_CHECK_FABS(v[1], -2);
    GGO_CHECK_FABS(v[2], -5);
    GGO_CHECK_FABS(v[3], 1);
  }

  // Z-axis
  {
    ggo_matrix4x4 m;
    m.set_rotation_z(GGO_PI / 6);
    
    GGO_CHECK_FABS(m(0, 0), std::sqrt(3) / 2);
    GGO_CHECK_FABS(m(1, 0), -0.5);
    GGO_CHECK_FABS(m(2, 0), 0);
    GGO_CHECK_FABS(m(3, 0), 0);
    
    GGO_CHECK_FABS(m(0, 1), 0.5);
    GGO_CHECK_FABS(m(1, 1), std::sqrt(3) / 2);
    GGO_CHECK_FABS(m(2, 1), 0);
    GGO_CHECK_FABS(m(3, 1), 0);
    
    GGO_CHECK_FABS(m(0, 2), 0);
    GGO_CHECK_FABS(m(1, 2), 0);
    GGO_CHECK_FABS(m(2, 2), 1);
    GGO_CHECK_FABS(m(3, 2), 0);
    
    GGO_CHECK_FABS(m(0, 3), 0);
    GGO_CHECK_FABS(m(1, 3), 0);
    GGO_CHECK_FABS(m(2, 3), 0);
    GGO_CHECK_FABS(m(3, 3), 1);

    std::array<float, 4> v {5, -2, 7, 1};
    m.set_rotation_z(GGO_PI / 2);
    v = m * v;
 
    GGO_CHECK_FABS(v[0], 2);
    GGO_CHECK_FABS(v[1], 5);
    GGO_CHECK_FABS(v[2], 7);
    GGO_CHECK_FABS(v[3], 1);
  }
  
  // Rotation composition.
  {
    ggo_matrix4x4 m1, m2, m3, m4;
    m1.set_rotation_x(0.2);
    m2.set_rotation_x(0.1);
    m3 = m1 * m2;
    m4.set_rotation_x(0.3);
    GGO_CHECK(m3.compare(m4, 0.001));
  }
  
  // Axis rotation.
  {
    ggo_matrix4x4 m1, m2;
    
    m1.set_rotation_x(0.2);
    m2.set_rotation(ggo_vector3d_float(1, 0, 0), 0.2);
    GGO_CHECK(m1.compare(m2, 0.001));

    m1.set_rotation_y(0.2);
    m2.set_rotation(ggo_vector3d_float(0, 1, 0), 0.2);
    GGO_CHECK(m1.compare(m2, 0.001));
    
    m1.set_rotation_z(0.2);
    m2.set_rotation(ggo_vector3d_float(0, 0, 1), 0.2);
    GGO_CHECK(m1.compare(m2, 0.001));
  }
}

void test_multiplication()
{
  ggo_matrix4x4 m1;
  
  m1(0, 0) = 3;
  m1(1, 0) = 2;
  m1(2, 0) = -1;
  m1(3, 0) = -2;
  
  m1(0, 1) = 1;
  m1(1, 1) = 0;
  m1(2, 1) = 4;
  m1(3, 1) = -1;
  
  m1(0, 2) = 2;
  m1(1, 2) = 3;
  m1(2, 2) = 1;
  m1(3, 2) = 1;
  
  m1(0, 3) = 3;
  m1(1, 3) = -2;
  m1(2, 3) = -1;
  m1(3, 3) = 1;
  
  ggo_matrix4x4 m2;
  
  m2(0, 0) = 2;
  m2(1, 0) = 1;
  m2(2, 0) = -2;
  m2(3, 0) = -1;
  
  m2(0, 1) = 1;
  m2(1, 1) = 3;
  m2(2, 1) = 2;
  m2(3, 1) = -1;
  
  m2(0, 2) = 3;
  m2(1, 2) = -3;
  m2(2, 2) = -2;
  m2(3, 2) = 1;
  
  m2(0, 3) = 2;
  m2(1, 3) = 3;
  m2(2, 3) = 4;
  m2(3, 3) = -1;
  
  ggo_matrix4x4 m3(m1 * m2);

  GGO_CHECK_FABS(m3(0, 0), 1);
  GGO_CHECK_FABS(m3(1, 0), 6);
  GGO_CHECK_FABS(m3(2, 0), -8);
  GGO_CHECK_FABS(m3(3, 0), -4);
  
  GGO_CHECK_FABS(m3(0, 1), 12);
  GGO_CHECK_FABS(m3(1, 1), -14);
  GGO_CHECK_FABS(m3(2, 1), -14);
  GGO_CHECK_FABS(m3(3, 1), 4);
  
  GGO_CHECK_FABS(m3(0, 2), 12);
  GGO_CHECK_FABS(m3(1, 2), 11);
  GGO_CHECK_FABS(m3(2, 2), 4);
  GGO_CHECK_FABS(m3(3, 2), -5);
  
  GGO_CHECK_FABS(m3(0, 3), 3);
  GGO_CHECK_FABS(m3(1, 3), 3);
  GGO_CHECK_FABS(m3(2, 3), -4);
  GGO_CHECK_FABS(m3(3, 3), -3);
}

void test_projection()
{
    ggo_matrix4x4 m;
    m.set_projection(GGO_PI / 2, 1, 1, 2);
    
    std::cout << m << "\n\n";

    {
        std::array<float, 4> v {1, 1, -1, 1};
        v = m * v;
        GGO_CHECK_FABS(v[0], 1);
        GGO_CHECK_FABS(v[1], 1);
        GGO_CHECK_FABS(v[2], -1);
        GGO_CHECK_FABS(v[3], 1);
        std::cout << v << "\n\n";
    }
    
    {
        std::array<float, 4> v {2, 2, -2, 1};
        v = m * v;
        GGO_CHECK_FABS(v[0], 2);
        GGO_CHECK_FABS(v[1], 2);
        GGO_CHECK_FABS(v[2], 2);
        GGO_CHECK_FABS(v[3], 2);
        std::cout << v << "\n\n";
    }
}

int main(int argc, char ** argv)
{
    test_copy();
    test_translation();
    test_rotation();
    test_multiplication();
    test_projection();
    
    std::cout << std::endl << GGO_NONREG_SUCCESS_MSG << std::endl << std::endl;
    
    return 0;
}
