#ifndef __GGO_VEC__
#define __GGO_VEC__

#include <ostream>
#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////////
// Declaration.
namespace ggo
{
  template <typename data_t, int n_dims>
  class vec
  {
  public:

    vec() {}

    template <typename... args>
    vec(const data_t & v, args... a)
    {
      static_assert(sizeof...(a) + 1 == n_dims, "invalid number of arguments");
      ggo::set(_coords, v, a...);
    }

    vec(const vec<data_t, n_dims> & rhs)
    {
      ggo::copy<n_dims, data_t>(_coords, rhs._coords);
    }

    const vec<data_t, n_dims> &	operator=(const vec<data_t, n_dims> & rhs) { ggo::copy<n_dims, data_t>(_coords, rhs._coords); return *this; }

    // Run-time access.
    data_t &  get(int index) { return _coords[index]; }

    const data_t & get(int index) const { return _coords[index]; }

    // Compile-time access.
    template <int index>
    data_t &  get() { static_assert(index >= 0 && index < n_dims, "invalid index"); return _coords[index]; }

    template <int index>
    const data_t & get() const { static_assert(index >= 0 && index < n_dims, "invalid index"); return _coords[index]; }

    template <int index>
    void set(const data_t & v) { static_assert(index >= 0 && index < n_dims, "invalid index"); _coords[index] = v; }

    template <typename... args>
    void set(args... a) { static_assert(sizeof...(a) == n_dims, "invalid number of arguments"); ggo::set(_coords, a...); }

    bool                      operator==(const vec<data_t, n_dims> & rhs) const { return ggo::compare<n_dims>(_coords, rhs._coords); }
    bool                      operator!=(const vec<data_t, n_dims> & rhs) const { return !ggo::compare<n_dims>(_coords, rhs._coords); }

    vec<data_t, n_dims>       operator-() const;

    void            		      operator+=(const vec<data_t, n_dims> & rhs);
    void            		      operator-=(const vec<data_t, n_dims> & rhs);

    void            		      operator*=(data_t k);
    void            		      operator/=(data_t k);

    data_t *                  data() { return _coords; }
    const data_t *            data() const { return _coords; }

    data_t                    get_hypot() const;
    void				              set_length(data_t length);

    template <typename real_t = data_t>
    real_t                    get_length() const
    {
      static_assert(std::is_floating_point<real_t>::value, "invalid type");
      return std::sqrt(ggo::to<real_t>(get_hypot()));
    }

    void				              normalize();
    bool				              is_normalized(data_t epsilon = data_t(0.001)) const;
    ggo::vec<data_t, n_dims>  get_normalized() const;

    void                      flip();

    template <typename... args>
    void                      move(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      ggo::add(_coords, a...);
    }

  private:

    data_t  _coords[n_dims];
  };
}

//////////////////////////////////////////////////////////////////
// Aliases.
namespace ggo
{
  template <typename data_t> using pos2 = vec<data_t, 2>;
  template <typename data_t> using vec2 = vec<data_t, 2>;
  using pos2f = vec<float, 2>;
  using vec2f = vec<float, 2>;
  using pos2d = vec<double, 2>;
  using vec2d = vec<double, 2>;

  template <typename data_t> using pos3 = vec<data_t, 3>;
  template <typename data_t> using vec3 = vec<data_t, 3>;
  using pos3f = vec<float, 3>;
  using vec3f = vec<float, 3>;
  using pos3d = vec<double, 3>;
  using vec3d = vec<double, 3>;
}

//////////////////////////////////////////////////////////////////
// Global functions.
namespace ggo
{
  template <typename data_t, int n_dims>
  data_t dot(const ggo::vec<data_t, n_dims> & v1, const ggo::vec<data_t, n_dims> & v2)
  {
    return ggo::dot<n_dims>(v1.data(), v2.data());
  }

  template <typename data_t, int n_dims>
  bool compare(const ggo::vec<data_t, n_dims> & v1, const ggo::vec<data_t, n_dims> & v2, data_t tolerance)
  {
    return ggo::compare<n_dims>(v1.data(), v2.data(), tolerance);
  }

  template <typename data_t, int n_dims>
  std::ostream & operator<<(std::ostream & os, const ggo::vec<data_t, n_dims> & v)
  {
    ggo::dump<n_dims>(v.data(), os);
    return os;
  }
}

//////////////////////////////////////////////////////////////////
// Arithmetic global operators.
namespace ggo
{
  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> operator+(const ggo::vec<data_t, n_dims> & v1, const ggo::vec<data_t, n_dims> & v2)
  {
    ggo::vec<data_t, n_dims> r;
    ggo::binary_operation<n_dims>(r.data(), v1.data(), v2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 + src2; });
    return r;
  }

  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> operator-(const ggo::vec<data_t, n_dims> & v1, const ggo::vec<data_t, n_dims> & v2)
  {
    ggo::vec<data_t, n_dims> r;
    ggo::binary_operation<n_dims>(r.data(), v1.data(), v2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 - src2; });
    return r;
  }

  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> operator*(data_t k, const ggo::vec<data_t, n_dims> & v)
  {
    ggo::vec<data_t, n_dims> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = k * src; });
    return r;
  }

  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> operator*(const ggo::vec<data_t, n_dims> & v, data_t k)
  {
    ggo::vec<data_t, n_dims> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = src * k; });
    return r;
  }

  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> operator/(const ggo::vec<data_t, n_dims> & v, data_t k)
  {
    GGO_ASSERT(k != 0);
    ggo::vec<data_t, n_dims> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = src / k; });
    return r;
  }
}

/////////////////////////////////////////////////////////////////////
// 2D only functions.
namespace ggo
{
  template <typename data_t>
  ggo::vec<data_t, 2> rotate(const ggo::vec<data_t, 2> & v, data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    ggo::vec<data_t, 2> r;

    r.template set<0>(v.template get<0>() * cos_tmp - v.template get<1>() * sin_tmp);
    r.template set<1>(v.template get<0>() * sin_tmp + v.template get<1>() * cos_tmp);

    return r;
  }

  template <typename data_t>
  ggo::vec<data_t, 2> rotate(const ggo::vec<data_t, 2> & v, const ggo::vec<data_t, 2> & center, data_t angle)
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");

    ggo::vec<data_t, 2> rotated(v); // We have to use a temporary in case &center == this.

    rotated -= center;
    rotated = ggo::rotate(rotated, angle);

    return center + rotated;
  }

  template <typename data_t>
  data_t ortho_dot(const ggo::vec<data_t, 2> & v1, const ggo::vec<data_t, 2> & v2)
  {
    return v1.template get<1>() * v2.template get<0>() - v1.template get<0>() * v2.template get<1>();
  }

  template <typename data_t>
  ggo::vec<data_t, 2> from_polar(data_t angle, data_t length)
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");
    return ggo::vec<data_t, 2>(length * std::cos(angle), length * std::sin(angle));
  }

  template <typename data_t>
  data_t get_angle(const ggo::vec<data_t, 2> & v)
  {
    return std::atan2(v.template get<1>(), v.template get<0>());
  }
}

/////////////////////////////////////////////////////////////////////
// 3D only functions.
namespace ggo
{
  template <typename data_t>
  ggo::vec<data_t, 3> cross(const ggo::vec<data_t, 3> & v1, const ggo::vec<data_t, 3> & v2)
  {
    return ggo::vec<data_t, 3>(
      v1.template get<1>()*v2.template get<2>() - v1.template get<2>()*v2.template get<1>(),
      v1.template get<2>()*v2.template get<0>() - v1.template get<0>()*v2.template get<2>(),
      v1.template get<0>()*v2.template get<1>() - v1.template get<1>()*v2.template get<0>());
  }

  template <typename data_t>
  bool is_basis(const ggo::vec<data_t, 3> & v1, const ggo::vec<data_t, 3> & v2, const ggo::vec<data_t, 3> & v3)
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");

    if (v1.is_normalized() == false || v2.is_normalized() == false || v3.is_normalized() == false)
    {
      return false;
    }

    // v3 should be equal to cross(v1, v2).
    auto c = ggo::cross(v1, v2);

    if (std::abs(v3.template get<0>() - c.template get<0>()) > static_cast<data_t>(0.0001) ||
        std::abs(v3.template get<1>() - c.template get<1>()) > static_cast<data_t>(0.0001) ||
        std::abs(v3.template get<2>() - c.template get<2>()) > static_cast<data_t>(0.0001))
    {
      return false;
    }

    return true;
  }

  template <typename data_t>
  std::pair<ggo::vec<data_t, 3>, ggo::vec<data_t, 3>> build_basis(const ggo::vec<data_t, 3> & v)
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");

    GGO_ASSERT(v.is_normalized() == true);

    // Get 2 orthogonal vectors.
    data_t x = std::abs(v.template get<0>());
    data_t y = std::abs(v.template get<1>());
    data_t z = std::abs(v.template get<2>());

    std::pair<ggo::vec<data_t, 3>, ggo::vec<data_t, 3>> basis;

    if ((x <= y) && (x <= z)) // Mininum is X.
    {
      data_t inv_norm = 1 / std::sqrt(v.template get<2>() * v.template get<2>() + v.template get<1>() * v.template get<1>());
      basis.first.set(data_t(0), v.template get<2>() * inv_norm, -v.template get<1>() * inv_norm);
    }
    else if ((y <= x) && (y <= z)) // Mininum is Y.
    {
      data_t inv_norm = 1 / std::sqrt(v.template get<2>() * v.template get<2>() + v.template get<0>() * v.template get<0>());
      basis.first.set(v.template get<2>() * inv_norm, data_t(0), -v.template get<0>() * inv_norm);
    }
    else // Mininum is Z.
    {
      data_t inv_norm = 1 / std::sqrt(v.template get<1>() * v.template get<1>() + v.template get<0>() * v.template get<0>());
      basis.first.set(v.template get<1>() * inv_norm, -v.template get<0>() * inv_norm, data_t(0));
    }

    basis.second = ggo::cross(v, basis.first);

    GGO_ASSERT(ggo::is_basis(v, basis.first, basis.second));

    return basis;
  }
}

//////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename data_t, int n_dims>
  vec<data_t, n_dims> vec<data_t, n_dims>::operator-() const
  {
    vec<data_t, n_dims> v;
    ggo::unary_operation<n_dims>(v._coords, _coords, [](data_t & dst, const data_t & src) { dst = -src; });
    return v;
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::operator+=(const vec<data_t, n_dims> & rhs)
  {
    ggo::unary_operation<n_dims>(_coords, rhs._coords, [](data_t & dst, const data_t & src) { dst += src; });
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::operator-=(const vec<data_t, n_dims> & rhs)
  {
    ggo::unary_operation<n_dims>(_coords, rhs._coords, [](data_t & dst, const data_t & src) { dst -= src; });
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::operator*=(data_t k)
  {
    ggo::for_each<n_dims>(_coords, [&](data_t & d) { d *= k; });
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::operator/=(data_t k)
  {
    GGO_ASSERT(k != 0);
    ggo::for_each<n_dims>(_coords, [&](data_t & d) { return d /= k; });
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::flip()
  {
    ggo::for_each<n_dims>(_coords, [](data_t & d) { d = -d; });
  }

  template <typename data_t, int n_dims>
  data_t vec<data_t, n_dims>::get_hypot() const
  {
    return ggo::dot<n_dims>(_coords, _coords);
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::set_length(data_t length)
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");
    GGO_ASSERT(get_hypot() > data_t(0));
    this->operator*=(length / get_length());
  }

  template <typename data_t, int n_dims>
  void vec<data_t, n_dims>::normalize()
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");
    set_length(1);
  }

  template <typename data_t, int n_dims>
  bool vec<data_t, n_dims>::is_normalized(data_t epsilon) const
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");
    return std::abs(get_hypot() - 1) < epsilon;
  }

  template <typename data_t, int n_dims>
  ggo::vec<data_t, n_dims> vec<data_t, n_dims>::get_normalized() const
  {
    static_assert(std::is_floating_point<data_t>::value, "invalid type");
    ggo::vec<data_t, n_dims> r(*this);
    r.normalize(); 
    return r;
  }
}

#endif 
