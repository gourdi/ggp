#ifndef __GGO_VEC3__
#define __GGO_VEC3__

#include "ggo_vec.h"
#include <utility>

//////////////////////////////////////////////////////////////
// struct
namespace ggo
{
  template <typename data_t>
  struct vec3 : public vec_base<data_t, 3, vec3<data_t>>
  {
    constexpr vec3() : vec_base<data_t, 3, vec3<data_t>>() {}
    constexpr vec3(const vec3<data_t> & v) : vec_base<data_t, 3, vec3<data_t>>(v) {}
    constexpr vec3(data_t k1, data_t k2, data_t k3) : vec_base<data_t, 3, vec3<data_t>>(k1, k2, k3) {}
    constexpr vec3(data_t k) : vec_base<data_t, 3, vec3<data_t>>(k) {}

    data_t & x() { return this->_coefs[0]; }
    data_t & y() { return this->_coefs[1]; }
    data_t & z() { return this->_coefs[2]; }

    constexpr data_t x() const { return this->_coefs[0]; }
    constexpr data_t y() const { return this->_coefs[1]; }
    constexpr data_t z() const { return this->_coefs[2]; }
  };
}

//////////////////////////////////////////////////////////////
// Aliases
namespace ggo
{
  template <typename data_t>
  using pos3  = vec3<data_t>;

  using vec3_i = vec3<int>;
  using pos3_i = vec3<int>;
  using vec3_f = vec3<float>;
  using pos3_f = vec3<float>;
  using vec3_d = vec3<double>;
  using pos3_d = vec3<double>;
}

/////////////////////////////////////////////////////////////////////
// 3D global functions.
namespace ggo
{
  template <typename data_t, typename subtype_t>
  constexpr subtype_t cross(const ggo::vec_base<data_t, 3, subtype_t> & v1, const ggo::vec_base<data_t, 3, subtype_t> & v2)
  {
    return {
      v1._coefs[1] * v2._coefs[2] - v1._coefs[2] * v2._coefs[1],
      v1._coefs[2] * v2._coefs[0] - v1._coefs[0] * v2._coefs[2],
      v1._coefs[0] * v2._coefs[1] - v1._coefs[1] * v2._coefs[0] };
  }

  template <typename data_t, typename subtype_t>
  bool is_basis(const ggo::vec_base<data_t, 3, subtype_t> & v1, const ggo::vec_base<data_t, 3, subtype_t> & v2, const ggo::vec_base<data_t, 3, subtype_t> & v3)
  {
    static_assert(std::is_floating_point_v<data_t>);

    if (is_normalized(v1) == false || is_normalized(v2) == false || is_normalized(v3) == false)
    {
      return false;
    }

    // v3 should be equal to cross(v1, v2).
    return compare(ggo::cross(v1, v2), v3);
  }

  template <typename data_t, typename subtype_t>
  std::pair<subtype_t, subtype_t> build_basis(const ggo::vec_base<data_t, 3, subtype_t> & v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    GGO_ASSERT(is_normalized(v) == true);

    // Get 2 orthogonal vectors.
    data_t x = std::abs(v[0]);
    data_t y = std::abs(v[1]);
    data_t z = std::abs(v[2]);

    std::pair<subtype_t, subtype_t> basis;

    if ((x <= y) && (x <= z)) // Mininum is X.
    {
      data_t inv_norm = 1 / std::sqrt(v[2] * v[2] + v[1] * v[1]);
      basis.first = { data_t(0), v[2] * inv_norm, -v[1] * inv_norm };
    }
    else if ((y <= x) && (y <= z)) // Mininum is Y.
    {
      data_t inv_norm = 1 / std::sqrt(v[2] * v[2] + v[0] * v[0]);
      basis.first = { v[2] * inv_norm, data_t(0), -v[0] * inv_norm };
    }
    else // Mininum is Z.
    {
      data_t inv_norm = 1 / std::sqrt(v[1] * v[1] + v[0] * v[0]);
      basis.first = { v[1] * inv_norm, -v[0] * inv_norm, data_t(0) };
    }

    basis.second = ggo::cross(v, basis.first);

    GGO_ASSERT(ggo::is_basis(v, basis.first, basis.second));

    return basis;
  }
}

#endif 
