#include "ggo_matrix4x4.h"
#include <ggo_linear_algebra3d.h>
#include <algorithm>

//////////////////////////////////////////////////////////////
ggo_matrix4x4::ggo_matrix4x4()
  :
  _coefs{1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0, 
  0, 0, 0, 1}
{
}

//////////////////////////////////////////////////////////////
float ggo_matrix4x4::operator()(int x, int y) const
{
    return _coefs[4 * x + y];
}

//////////////////////////////////////////////////////////////
float & ggo_matrix4x4::operator()(int x, int y)
{
    return _coefs[4 * x + y];
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_identity()
{
  _coefs = {{1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f, 
    0.f, 0.f, 0.f, 1.f}};
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_translation(float dx, float dy, float dz)
{
  set_identity();

  _coefs[12] = dx;
  _coefs[13] = dy;
  _coefs[14] = dz;
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_rotation_x(float angle)
{
  set_identity();
  
  float cosine = std::cos(angle);
  float sine = std::sin(angle);
  
  operator()(1, 1) = cosine;
  operator()(2, 1) = -sine;
  operator()(1, 2) = sine;
  operator()(2, 2) = cosine;
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_rotation_y(float angle)
{
  set_identity();
  
  float cosine = std::cos(angle);
  float sine = std::sin(angle);

  operator()(0, 0) = cosine;
  operator()(2, 0) = sine;
  operator()(0, 2) = -sine;
  operator()(2, 2) = cosine;
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_rotation_z(float angle)
{
  set_identity();
  
  float cosine = std::cos(angle);
  float sine = std::sin(angle);

  operator()(0, 0) = cosine;
  operator()(1, 0) = -sine;
  operator()(0, 1) = sine;
  operator()(1, 1) = cosine;
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_rotation(const ggo::vector3d_float & axis, float angle)
{
  float m[3][3];

  ggo::fill_rotation_matrix(axis, angle, m);
  
  set_identity();
  
  operator()(0, 0) = m[0][0];
  operator()(1, 0) = m[0][1];
  operator()(2, 0) = m[0][2];
  
  operator()(0, 1) = m[1][0];
  operator()(1, 1) = m[1][1];
  operator()(2, 1) = m[1][2];
  
  operator()(0, 2) = m[2][0];
  operator()(1, 2) = m[2][1];
  operator()(2, 2) = m[2][2];
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::set_projection(float fovy, float aspect, float near, float far)
{
  GGO_ASSERT(near < far);

  this->fill(0);

  operator()(0, 0) = 1 / (aspect * std::tan(fovy / 2));
  operator()(1, 1) = 1 / std::tan(fovy / 2);
  operator()(2, 2) = -(far + near) / (far - near);
  operator()(3, 2) = -(2 * near * far) / (far - near);
  operator()(2, 3) = -1;
}

//////////////////////////////////////////////////////////////
void ggo_matrix4x4::fill(float v)
{
    std::fill(_coefs.begin(), _coefs.end(), v);
}

//////////////////////////////////////////////////////////////
ggo_matrix4x4 ggo_matrix4x4::operator*(const ggo_matrix4x4 & matrix) const
{
  ggo_matrix4x4 res;

  for (int y = 0; y < 4; ++y)
  {
    for (int x = 0; x < 4; ++x)
    {
      res(x, y) = operator()(0, y) * matrix(x, 0) +
                  operator()(1, y) * matrix(x, 1) +
                  operator()(2, y) * matrix(x, 2) +
                  operator()(3, y) * matrix(x, 3);
    }
  }

  return res;
}

//////////////////////////////////////////////////////////////
bool ggo_matrix4x4::compare(const ggo_matrix4x4 & matrix, float epsilon) const
{
  for (int i = 0; i < 16; ++i)
  {
    if (std::abs(_coefs[i] - matrix._coefs[i]) > epsilon)
    {
      return false;
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////
std::array<float, 4> operator*(const ggo_matrix4x4 & matrix, const std::array<float, 4> & v)
{
  std::array<float, 4> res;
  
  res[0] = matrix(0, 0) * v[0] +
           matrix(1, 0) * v[1] +
           matrix(2, 0) * v[2] +
           matrix(3, 0) * v[3];
  res[1] = matrix(0, 1) * v[0] +
           matrix(1, 1) * v[1] +
           matrix(2, 1) * v[2] +
           matrix(3, 1) * v[3];
  res[2] = matrix(0, 2) * v[0] +
           matrix(1, 2) * v[1] +
           matrix(2, 2) * v[2] +
           matrix(3, 2) * v[3];
  res[3] = matrix(0, 3) * v[0] +
           matrix(1, 3) * v[1] +
           matrix(2, 3) * v[2] +
           matrix(3, 3) * v[3];
           
  return res;
}

//////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream& os, const ggo_matrix4x4 & matrix)
{
  os << "[";
  for (int y = 0; y < 4; ++y)
  {
    os << "[";
    for (int x = 0; x < 4; ++x)
    {
      os << matrix(x, y);
      if (x < 3)
      {
        os << ", ";
      }
    }
    os << "]";
    if (y < 3)
    {
      os << "\n";
    }
  }
  os << "]";

  return os;
}

//////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream & os, const std::array<float, 4> & v)
{
  os << '[' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ']';

  return os;
}

