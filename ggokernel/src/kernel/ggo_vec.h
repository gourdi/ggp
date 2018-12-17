#ifndef __GGO_VEC__
#define __GGO_VEC__

#include <type_traits>
#include <ostream>
#include <kernel/ggo_details.h>

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
      ggo::details::set<data_t, n_dims, n_dims - 1>(_coefs, k1, k2);
    }

    template <typename... args>
    constexpr vec_base(data_t k1, data_t k2, args... a) : _coefs()
    {
      static_assert(n_dims > 2 && sizeof...(a) == n_dims - 2);
      ggo::details::set<data_t, n_dims, n_dims - 1>(_coefs, k1, k2, a...);
    }

    constexpr vec_base(data_t k) : _coefs()
    {
      for (int i = 0; i < n_dims; ++i) { _coefs[i] = k; }
    }

    // In-place arithmetics operators.
    void operator*=(const vec_base<data_t, n_dims> & v) { details::mul<n_dims>(this->_coefs, v._coefs); }
    void operator/=(const vec_base<data_t, n_dims> & v) { details::div<n_dims>(this->_coefs, v._coefs); }
    void operator+=(const vec_base<data_t, n_dims> & v) { details::add<n_dims>(this->_coefs, v._coefs); }
    void operator-=(const vec_base<data_t, n_dims> & v) { details::sub<n_dims>(this->_coefs, v._coefs); }

    void operator*=(data_t k) { details::mul<n_dims>(this->_coefs, k); }
    void operator/=(data_t k) { details::div<n_dims>(this->_coefs, k); }

    data_t _coefs[n_dims];
  };
}

//////////////////////////////////////////////////////////////
// I/O.
namespace ggo
{
  namespace details
  {
    template <typename data_t, int n_dims, int dim>
    struct dump_t
    {
      static void call(std::ostream & os, const vec_base<data_t, n_dims> & v)
      {
        dump_t<data_t, n_dims, dim - 1>::call(os, v);
        os << "; " << v._coefs[dim];
      }
    };

    template <typename data_t, int n_dims>
    struct dump_t<data_t, n_dims, 0>
    {
      static void call(std::ostream & os, const vec_base<data_t, n_dims> & v)
      {
        os << v._coefs[0];
      }
    };
  }

  template <typename data_t, int n_dims>
  std::ostream & operator<<(std::ostream & os, const ggo::vec_base<data_t, n_dims> & v)
  {
    os << '(';
    details::dump_t<data_t, n_dims, n_dims - 1>::call(os, v);
    os << ')';
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
    return ggo::details::eq<data_t, n_dims>(v1._coefs, v2._coefs);
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
  namespace details
  {
    template <typename data_t, int n_dims, int dim>
    struct vec_dot
    {
      static constexpr data_t call(const ggo::vec_base<data_t, n_dims> & v1, const ggo::vec_base<data_t, n_dims> & v2)
      {
        return vec_dot<data_t, n_dims, dim - 1>::call(v1, v2) + v1._coefs[dim] * v2._coefs[dim];
      }
    };

    template <typename data_t, int n_dims>
    struct vec_dot<data_t, n_dims, 0>
    {
      static constexpr data_t call(const ggo::vec_base<data_t, n_dims> & v1, const ggo::vec_base<data_t, n_dims> & v2)
      {
        return v1._coefs[0] * v2._coefs[0];
      }
    };
  }

  template <typename data_t, int n_dims>
  constexpr data_t dot(const vec_base<data_t, n_dims> & v1, const vec_base<data_t, n_dims> & v2)
  {
    return details::vec_dot<data_t, n_dims, n_dims - 1>::call(v1, v2);
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
