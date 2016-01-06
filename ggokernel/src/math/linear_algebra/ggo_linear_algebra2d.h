#ifndef __GGO_LINEAR_ALGEBRA_2D__
#define __GGO_LINEAR_ALGEBRA_2D__

//////////////////////////////////////////////////////////////////
// Determinant.
namespace ggo
{
  template <typename T>
  T determinant(T m00, T m01, T m10, T m11)
  {
   return m00 * m11 - m10 * m01;
  }
}

//////////////////////////////////////////////////////////////////
// Linear system.
namespace ggo
{
  template <typename T>
  bool linsolve2d(const T m[2][2], const T c[2], T s[2])
  {
    T det = determinant(m[0][0], m[0][1],
                        m[1][0], m[1][1]);

    if (std::abs(det) < 0.000001) 
    {
      return false;
    }

    T inv_det = 1 / det;
    s[0] = (c[0] * m[1][1] - c[1] * m[0][1]) * inv_det;
    s[1] = (c[1] * m[0][0] - c[0] * m[1][0]) * inv_det;

    return true;
  }
}

#endif
