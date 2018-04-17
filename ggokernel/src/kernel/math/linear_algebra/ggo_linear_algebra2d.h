#ifndef __GGO_LINEAR_ALGEBRA_2D__
#define __GGO_LINEAR_ALGEBRA_2D__

//////////////////////////////////////////////////////////////////
// Determinant.
namespace ggo
{
  template <typename data_t>
  data_t determinant(data_t m00, data_t m01, data_t m10, data_t m11)
  {
   return m00 * m11 - m10 * m01;
  }
}

//////////////////////////////////////////////////////////////////
// Linear system.
namespace ggo
{
  template <typename data_t>
  bool linsolve2d(const data_t m[2][2], const data_t c[2], data_t s[2])
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    data_t det = determinant(m[0][0], m[0][1],
                             m[1][0], m[1][1]);

    if (std::abs(det) < 0.000001) 
    {
      return false;
    }

    data_t inv_det = 1 / det;
    s[0] = (c[0] * m[1][1] - c[1] * m[0][1]) * inv_det;
    s[1] = (c[1] * m[0][0] - c[0] * m[1][0]) * inv_det;

    return true;
  }
}

#endif
