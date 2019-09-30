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
  template <typename data_t, int n_dims, typename subtype_t>
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
    void operator*=(const vec_base<data_t, n_dims, subtype_t> & v) { ggo::mul(this->_coefs, v._coefs); }
    void operator/=(const vec_base<data_t, n_dims, subtype_t> & v) { ggo::div(this->_coefs, v._coefs); }
    void operator+=(const vec_base<data_t, n_dims, subtype_t> & v) { ggo::add(this->_coefs, v._coefs); }
    void operator-=(const vec_base<data_t, n_dims, subtype_t> & v) { ggo::sub(this->_coefs, v._coefs); }

    void operator*=(data_t k) { ggo::mul<n_dims>(this->_coefs, k); }
    void operator/=(data_t k) { ggo::div<n_dims>(this->_coefs, k); }

    // Member access.
    constexpr const data_t &  operator[](int i) const { return _coefs[i]; }
    constexpr data_t &        operator[](int i) { return _coefs[i]; }

    // Equality.
    constexpr bool operator==(const ggo::vec_base<data_t, n_dims, subtype_t> & v) const { return ggo::compare(_coefs, v._coefs); }
    constexpr bool operator!=(const ggo::vec_base<data_t, n_dims, subtype_t> & v) const { return !operator==(v); }

    data_t _coefs[n_dims];
  };
}

//////////////////////////////////////////////////////////////
// I/O.
namespace ggo
{
  template <typename data_t, int n_dims, typename subtype_t>
  std::ostream & operator<<(std::ostream & os, const ggo::vec_base<data_t, n_dims, subtype_t> & v)
  {
    ggo::dump(v._coefs, os);
    return os;
  }
}

//////////////////////////////////////////////////////////////
// Arithmetic.
namespace ggo
{
  namespace details
  {
    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_mul(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2, std::index_sequence<indices...>)
    {
      return subtype_t((v1._coefs[indices] * v2._coefs[indices])...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_div(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2, std::index_sequence<indices...>)
    {
      return subtype_t((v1._coefs[indices] / v2._coefs[indices])...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_add(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2, std::index_sequence<indices...>)
    {
      return subtype_t((v1._coefs[indices] + v2._coefs[indices])...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_sub(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2, std::index_sequence<indices...>)
    {
      return subtype_t((v1._coefs[indices] - v2._coefs[indices])...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_mul(const ggo::vec_base<data_t, n_dims, subtype_t> & v, data_t k, std::index_sequence<indices...>)
    {
      return subtype_t((v._coefs[indices] * k)...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_div(const ggo::vec_base<data_t, n_dims, subtype_t> & v, data_t k, std::index_sequence<indices...>)
    {
      return subtype_t((v._coefs[indices] / k)...);
    }

    template <typename data_t, int n_dims, typename subtype_t, std::size_t... indices>
    constexpr subtype_t vec_neg(const ggo::vec_base<data_t, n_dims, subtype_t> & v, std::index_sequence<indices...>)
    {
      return subtype_t((-v._coefs[indices])...);
    }
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator*(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return details::vec_mul(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator/(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return details::vec_div(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator+(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return details::vec_add(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator-(const ggo::vec_base<data_t, n_dims, subtype_t> & v1, const ggo::vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return details::vec_sub(v1, v2, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator*(const ggo::vec_base<data_t, n_dims, subtype_t> & v, data_t k)
  {
    return details::vec_mul(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator*(data_t k, const ggo::vec_base<data_t, n_dims, subtype_t> & v)
  {
    return details::vec_mul(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator/(const ggo::vec_base<data_t, n_dims, subtype_t> & v, data_t k)
  {
    return details::vec_div(v, k, std::make_index_sequence<n_dims>{});
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr subtype_t operator-(const ggo::vec_base<data_t, n_dims, subtype_t> & v)
  {
    return details::vec_neg(v, std::make_index_sequence<n_dims>{});
  }
}

//////////////////////////////////////////////////////////////
// Global functions.
namespace ggo
{
  template <typename data_t, int n_dims, typename subtype_t>
  constexpr data_t dot(const vec_base<data_t, n_dims, subtype_t> & v1, const vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return ggo::dot(v1._coefs, v2._coefs);
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr data_t hypot(const vec_base<data_t, n_dims, subtype_t> & v)
  {
    return dot(v, v);
  }

  template <typename data_t, int n_dims, typename subtype_t>
  constexpr data_t hypot(const vec_base<data_t, n_dims, subtype_t> & v1, const vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return hypot(v2 - v1);
  }

  template <typename data_t, int n_dims, typename subtype_t>
  data_t length(const vec_base<data_t, n_dims, subtype_t> & v)
  {
    static_assert(std::is_floating_point_v<data_t>);
    return std::sqrt(hypot(v));
  }

  template <typename data_t, int n_dims, typename subtype_t>
  data_t distance(const vec_base<data_t, n_dims, subtype_t> & v1, const vec_base<data_t, n_dims, subtype_t> & v2)
  {
    return length(v2 - v1);
  }

  template <typename data_t, int n_dims, typename subtype_t>
  subtype_t normalize(const vec_base<data_t, n_dims, subtype_t> & v)
  {
    return v / length(v);
  }

  template <typename data_t, int n_dims, typename subtype_t>
  bool is_normalized(const vec_base<data_t, n_dims, subtype_t> & v, data_t tolerance = data_t(0.001))
  {
    static_assert(std::is_floating_point_v<data_t>);
    return std::abs(hypot(v) - 1) < tolerance;
  }

  template <typename data_t, int n_dims, typename subtype_t>
  bool compare(const vec_base<data_t, n_dims, subtype_t> & v1, const vec_base<data_t, n_dims, subtype_t> & v2, data_t tolerance = data_t(0.001))
  {
    static_assert(std::is_floating_point_v<data_t>);
    return hypot(v2 - v1) < tolerance;
  }
}

#endif 
