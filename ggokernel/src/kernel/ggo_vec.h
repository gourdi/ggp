#ifndef __GGO_VEC__
#define __GGO_VEC__

#include <type_traits>
#include <ostream>
#include <kernel/ggo_assign.h>
#include <kernel/ggo_arithmetics.h>
#include <kernel/ggo_compare.h>
#include <kernel/ggo_string_helpers.h>

//////////////////////////////////////////////////////////////
// vec_base
namespace ggo
{
  template <typename data_t, int n_dims>
  struct vec_base
  {
    using _data_t = data_t;
    static constexpr int _n_dims = n_dims;

    // Constructors.
    constexpr vec_base() = default;
    constexpr vec_base(const vec_base & v) = default;

    constexpr vec_base(data_t k1, data_t k2) : _coefs()
    {
      static_assert(n_dims == 2);
      ggo::assign(_coefs, k1, k2);
    }

    template <typename... args>
    constexpr vec_base(data_t k1, data_t k2, args... a) : _coefs()
    {
      static_assert(n_dims > 2 && sizeof...(a) == n_dims - 2);
      ggo::assign(_coefs, k1, k2, a...);
    }

    constexpr vec_base(data_t k) : _coefs()
    {
      for (int i = 0; i < n_dims; ++i) { _coefs[i] = k; }
    }

    // In-place arithmetics operators.
    void operator*=(const vec_base<data_t, n_dims> & v) { ggo::mul(this->_coefs, v._coefs); }
    void operator/=(const vec_base<data_t, n_dims> & v) { ggo::div(this->_coefs, v._coefs); }
    void operator+=(const vec_base<data_t, n_dims> & v) { ggo::add(this->_coefs, v._coefs); }
    void operator-=(const vec_base<data_t, n_dims> & v) { ggo::sub(this->_coefs, v._coefs); }

    void operator*=(data_t k) { ggo::mul<n_dims>(this->_coefs, k); }
    void operator/=(data_t k) { ggo::div<n_dims>(this->_coefs, k); }

    constexpr const data_t &  operator[](int i) const { return _coefs[i]; }
    constexpr data_t &        operator[](int i) { return _coefs[i]; }

    data_t _coefs[n_dims];
  };
}

//////////////////////////////////////////////////////////////
// I/O.
namespace ggo
{
  template <typename data_t, int n_dims>
  std::ostream & operator<<(std::ostream & os, const ggo::vec_base<data_t, n_dims> & v)
  {
    ggo::dump(v._coefs, os);
    return os;
  }
}

//////////////////////////////////////////////////////////////
// Equality.
namespace ggo
{
  template <typename data_t, int n_dims>
  constexpr bool operator==(const ggo::vec_base<data_t, n_dims> & v1, const ggo::vec_base<data_t, n_dims> & v2)
  {
    return ggo::compare(v1._coefs, v2._coefs);
  }

  template <typename data_t, int n_dims>
  constexpr bool operator!=(const ggo::vec_base<data_t, n_dims> & v1, const ggo::vec_base<data_t, n_dims> & v2)
  {
    return !operator==(v1, v2);
  }
}

//////////////////////////////////////////////////////////////
// Arithmetic.
namespace ggo
{
  namespace details
  {
    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_mul(const vec_t & v1, const vec_t & v2, std::index_sequence<indices...>)
    {
      return vec_t((v1._coefs[indices] * v2._coefs[indices])...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_div(const vec_t & v1, const vec_t & v2, std::index_sequence<indices...>)
    {
      return vec_t((v1._coefs[indices] / v2._coefs[indices])...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_add(const vec_t & v1, const vec_t & v2, std::index_sequence<indices...>)
    {
      return vec_t((v1._coefs[indices] + v2._coefs[indices])...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_sub(const vec_t & v1, const vec_t & v2, std::index_sequence<indices...>)
    {
      return vec_t((v1._coefs[indices] - v2._coefs[indices])...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_mul(const vec_t & v, data_t k, std::index_sequence<indices...>)
    {
      return vec_t((v._coefs[indices] * k)...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_div(const vec_t & v, data_t k, std::index_sequence<indices...>)
    {
      return vec_t((v._coefs[indices] / k)...);
    }

    template <typename vec_t, typename data_t, int n_dims, std::size_t... indices>
    constexpr vec_t vec_neg(const vec_t & v, std::index_sequence<indices...>)
    {
      return vec_t((-v._coefs[indices])...);
    }
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator*(const vec_t & v1, const vec_t & v2)
  {
    return details::vec_mul<vec_t, data_t, n_dims>(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator/(const vec_t & v1, const vec_t & v2)
  {
    return details::vec_div<vec_t, data_t, n_dims>(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator+(const vec_t & v1, const vec_t & v2)
  {
    return details::vec_add<vec_t, data_t, n_dims>(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator-(const vec_t & v1, const vec_t & v2)
  {
    return details::vec_sub<vec_t, data_t, n_dims>(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator*(const vec_t & v, data_t k)
  {
    return details::vec_mul<vec_t, data_t, n_dims>(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator*(data_t k, const vec_t & v)
  {
    return details::vec_mul<vec_t, data_t, n_dims>(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator/(const vec_t & v, data_t k)
  {
    return details::vec_div<vec_t, data_t, n_dims>(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator-(const vec_t & v)
  {
    return details::vec_neg<vec_t, data_t, n_dims>(v, std::make_index_sequence<n_dims>{});
  }
}

//////////////////////////////////////////////////////////////
// Global functions.
namespace ggo
{
  template <typename data_t, int n_dims>
  constexpr data_t dot(const vec_base<data_t, n_dims> & v1, const vec_base<data_t, n_dims> & v2)
  {
    return ggo::dot(v1._coefs, v2._coefs);
  }

  template <typename data_t, int n_dims>
  constexpr data_t hypot(const vec_base<data_t, n_dims> & v)
  {
    return dot(v, v);
  }

  template <typename data_t, int n_dims>
  constexpr data_t hypot(const vec_base<data_t, n_dims> & v1, const vec_base<data_t, n_dims> & v2)
  {
    return hypot(v2 - v1);
  }

  template <typename data_t, int n_dims>
  data_t length(const vec_base<data_t, n_dims> & v)
  {
    static_assert(std::is_floating_point_v<data_t>);
    return std::sqrt(hypot(v));
  }

  template <typename data_t, int n_dims>
  data_t distance(const vec_base<data_t, n_dims> & v1, const vec_base<data_t, n_dims> & v2)
  {
    return length(v2 - v1);
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    vec_t normalize(const vec_t & v)
  {
    return v / length(v);
  }

  template <typename data_t, int n_dims>
  bool is_normalized(const vec_base<data_t, n_dims> & v, data_t tolerance = data_t(0.001))
  {
    static_assert(std::is_floating_point_v<data_t>);
    return std::abs(hypot(v) - 1) < tolerance;
  }

  template <typename data_t, int n_dims>
  bool compare(const vec_base<data_t, n_dims> & v1, const vec_base<data_t, n_dims> & v2, data_t tolerance = data_t(0.001))
  {
    static_assert(std::is_floating_point_v<data_t>);
    return hypot(v2 - v1) < tolerance;
  }
}

#endif 
