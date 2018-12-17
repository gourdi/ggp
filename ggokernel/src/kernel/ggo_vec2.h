#ifndef __GGO_VEC2__
#define __GGO_VEC2__

#include "ggo_vec.h"

//////////////////////////////////////////////////////////////
// struct
namespace ggo
{
  template <typename data_t>
  struct vec2 : public vec_base<data_t, 2>
  {
    constexpr vec2() {}
    constexpr vec2(const vec2<data_t> & v) : vec_base<data_t, 2>(v) {}
    constexpr vec2(data_t k1, data_t k2) : vec_base<data_t, 2>(k1, k2) {}
    constexpr vec2(data_t k) : vec_base<data_t, 2>(k) {}

    data_t & x() { return this->_coefs[0]; }
    data_t & y() { return this->_coefs[1]; }

    constexpr data_t x() const { return this->_coefs[0]; }
    constexpr data_t y() const { return this->_coefs[1]; }

    static vec2 from_angle(data_t angle)
    {
      static_assert(std::is_floating_point_v<data_t>);
      return { std::cos(angle), std::sin(angle) };
    }
  };
}

//////////////////////////////////////////////////////////////
// Aliases
namespace ggo
{
  template <typename data_t>
  using pos2  = vec2<data_t>;

  using vec2_i = vec2<int>;
  using pos2_i = vec2<int>;
  using vec2_f = vec2<float>;
  using pos2_f = vec2<float>;
  using vec2_d = vec2<double>;
  using pos2_d = vec2<double>;
}

//////////////////////////////////////////////////////////////
// 2D global functions.
namespace ggo
{
  template <typename vec_t,
    typename data_t = typename vec_t::_data_t,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, 2>, vec_t>>>
    vec_t rotate(const vec_t & v, data_t angle)
  {
    static_assert(std::is_floating_point_v<data_t>);

    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    return {
      v._coefs[0] * cos_tmp - v._coefs[1] * sin_tmp,
      v._coefs[0] * sin_tmp + v._coefs[1] * cos_tmp };
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, 2>, vec_t>>>
    vec_t rotate(vec_t v, vec_t center, data_t angle)
  {
    v -= center;
    v = ggo::rotate(v, angle);

    return center + v;
  }

  template <typename data_t>
  constexpr data_t ortho_dot(const ggo::vec_base<data_t, 2> & v1, const ggo::vec_base<data_t, 2> & v2)
  {
    return v1._coefs[1] * v2._coefs[0] - v1._coefs[0] * v2._coefs[1];
  }

  template <typename data_t>
  data_t angle(const ggo::vec_base<data_t, 2> & v)
  {
    return std::atan2(v._coefs[1], v._coefs[0]);
  }

  template <typename data_t>
  data_t angle(const ggo::vec_base<data_t, 2> & v1, const ggo::vec_base<data_t, 2> & v2)
  {
    GGO_ASSERT(ggo::hypot(v1) > 0 && ggo::hypot(v2) > 0);
    return std::abs(std::acos(ggo::dot(v1, v2) / (ggo::length(v1) * ggo::length(v2))));
  }
}

#endif 
