#ifndef __GGO_LINEAR_ALGEBRA_3D__
#define __GGO_LINEAR_ALGEBRA_3D__

#include <ggo_set3.h>

//////////////////////////////////////////////////////////////////
// Determinant.
namespace ggo
{
  template <typename T>
  T determinant(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
  {
   return m00 * (m11 * m22 - m21 * m12) -
          m10 * (m01 * m22 - m21 * m02) +
          m20 * (m01 * m12 - m11 * m02);
  }

  template <typename T>
  T determinant(const ggo::set3<T> & v1, const ggo::set3<T> & v2, const ggo::set3<T> & v3)
  {
    return ggo_determinant(v1.x(), v1.y(), v1.z(),
                           v2.x(), v2.y(), v2.z(),
                           v3.x(), v3.y(), v3.z());
  }
}

//////////////////////////////////////////////////////////////////
// Linear system.
namespace ggo
{
  template <typename T>
  bool linsolve3d(const T m[3][3], const T c[3], T s[3])
  {
    T det = determinant(m[0][0], m[0][1], m[0][2],
                        m[1][0], m[1][1], m[1][2],
                        m[2][0], m[2][1], m[2][2]);

    if (std::abs(det) < 0.000001) 
    {
      return false;
    }

    T  t1 = c[1] * m[2][2] - c[2] * m[1][2];
    T  t2 = c[1] * m[2][1] - c[2] * m[1][1];
    T  t3 = c[2] * m[1][0] - c[1] * m[2][0];

    T inv_det = 1 / det;
    s[0] = (c[0] * (m[1][1]*m[2][2]-m[1][2]*m[2][1]) - m[0][1]*t1 + m[0][2]*t2) * inv_det;
    s[1] = (c[0] * (m[1][2]*m[2][0]-m[1][0]*m[2][2]) + m[0][0]*t1 + m[0][2]*t3) * inv_det;
    s[2] = (c[0] * (m[1][0]*m[2][1]-m[1][1]*m[2][0]) - m[0][0]*t2 - m[0][1]*t3) * inv_det;

    return true;
  }
}

//////////////////////////////////////////////////////////////////
// Apply 3x3 matrix.
namespace ggo
{
  template <typename T>
  void apply_matrix3d(const T m[3][3], const T in[3], T out[3])
  {
    out[0] = m[0][0]*in[0] + m[0][1]*in[1] + m[0][2]*in[2];
    out[1] = m[1][0]*in[0] + m[1][1]*in[1] + m[1][2]*in[2];
    out[2] = m[2][0]*in[0] + m[2][1]*in[1] + m[2][2]*in[2];
  }

  template <typename T>
  ggo::set3<T> apply_matrix3d(const T m[3][3], const ggo::set3<T> & in)
  {
    T in_aux[3], out_aux[3];

    in_aux[0] = in.x();
    in_aux[1] = in.y();
    in_aux[2] = in.z();

    apply_matrix3d(m, in_aux, out_aux);

    return ggo::set3<T>(out_aux[0], out_aux[1], out_aux[2]);
  }

  template <typename T>
  void apply_matrix3d(const T m[3][3], T & x, T & y, T & z)
  {
    T in_aux[3], out_aux[3];

    in_aux[0] = x;
    in_aux[1] = y;
    in_aux[2] = z;

    apply_matrix3d(m, in_aux, out_aux);

    x = out_aux[0];
    y = out_aux[1];
    z = out_aux[2];
  }
}

//////////////////////////////////////////////////////////////////
// Fill rotation matrix.
// Reference: http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
namespace ggo
{
  template <typename T>
  void fill_rotation_matrix(const ggo::set3<T> & axis, T angle, T m[3][3])
  {
    T l = axis.get_length();
    T x = axis.x()/l;
    T y = axis.y()/l;
    T z = axis.z()/l;

    T c = std::cos(angle);
    T s = std::sin(angle);
    T t = 1 - c;

    m[0][0] = t*x*x + c;
    m[0][1] = t*x*y - z*s;
    m[0][2] = t*x*z + y*s;

    m[1][0] = t*y*x + z*s;
    m[1][1] = t*y*y + c;
    m[1][2] = t*y*z - x*s;

    m[2][0] = t*z*x - y*s;
    m[2][1] = t*z*y + x*s;
    m[2][2] = t*z*z + c;
  }
}

#endif
