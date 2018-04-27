#ifndef __GGO_VEC__
#define __GGO_VEC__

#include <ostream>
#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////////
// Declaration.
namespace ggo
{
  enum vec_type
  {
    geometry_t,
    color_t,
    none_t
  };

  template <typename data_t, int n_dims, vec_type vtype>
  class vec
  {
  public:

    vec()
    {
    }

    explicit vec(const data_t & v)
    {
      ggo::fill<n_dims>(_coords, v);
    }

    template <typename... args>
    vec(const data_t & v1, const data_t & v2, args... a)
    {
      static_assert(sizeof...(a) + 2 == n_dims, "invalid number of arguments");
      ggo::set<data_t>(_coords, v1, v2, a...);
    }

    vec(const vec<data_t, n_dims, vtype> & rhs)
    {
      ggo::copy<n_dims, data_t>(_coords, rhs._coords);
    }

    const vec<data_t, n_dims, vtype> &	operator=(const vec<data_t, n_dims, vtype> & rhs) { ggo::copy<n_dims, data_t>(_coords, rhs._coords); return *this; }

    // Run-time access.
    data_t & get(int index) { return _coords[index]; }

    const data_t & get(int index) const { return _coords[index]; }

    // Compile-time access.
    template <int index>
    data_t & get() { static_assert(index >= 0 && index < n_dims, "invalid index"); return _coords[index]; }

    template <int index>
    const data_t & get() const { static_assert(index >= 0 && index < n_dims, "invalid index"); return _coords[index]; }

    template <int index>
    void set(const data_t & v) { static_assert(index >= 0 && index < n_dims, "invalid index"); _coords[index] = v; }

    template <typename... args>
    void set(args... a) { static_assert(sizeof...(a) == n_dims, "invalid number of arguments"); ggo::set(_coords, a...); }

    bool                        operator==(const vec<data_t, n_dims, vtype> & rhs) const { return ggo::compare<n_dims>(_coords, rhs._coords); }
    bool                        operator!=(const vec<data_t, n_dims, vtype> & rhs) const { return !ggo::compare<n_dims>(_coords, rhs._coords); }

    vec<data_t, n_dims, vtype>  operator-() const;

    void            		        operator*=(const vec<data_t, n_dims, vtype> & rhs);
    void            		        operator+=(const vec<data_t, n_dims, vtype> & rhs);
    void            		        operator-=(const vec<data_t, n_dims, vtype> & rhs);
                                
    void            		        operator*=(data_t k);
    void            		        operator/=(data_t k);
                                
    data_t *                    data() { return _coords; }
    const data_t *              data() const { return _coords; }
                                
    data_t                      get_hypot() const;
    void				                set_length(data_t length);

    template <typename real_t = data_t>
    real_t                      get_length() const
    {
      static_assert(std::is_floating_point<real_t>::value);
      static_assert(vtype == geometry_t);
      return std::sqrt(static_cast<real_t>(get_hypot()));
    }

    void				                normalize();
    bool				                is_normalized(data_t epsilon = data_t(0.001)) const;
    vec<data_t, n_dims, vtype>  get_normalized() const;

    void                        flip();

    template <typename... args>
    void                        move(args... a)
    {
      static_assert(sizeof...(a) == n_dims, "invalid number of arguments");
      static_assert(vtype == geometry_t);
      ggo::add(_coords, a...);
    }

    // Geometry only.
    const data_t & x() const  { static_assert(vtype == geometry_t && n_dims >= 1 && n_dims <= 3); return _coords[0]; }
          data_t & x()        { static_assert(vtype == geometry_t && n_dims >= 1 && n_dims <= 3); return _coords[0]; }
    const data_t & y() const  { static_assert(vtype == geometry_t && n_dims >= 2 && n_dims <= 3); return _coords[1]; }
          data_t & y()        { static_assert(vtype == geometry_t && n_dims >= 2 && n_dims <= 3); return _coords[1]; }
    const data_t & z() const  { static_assert(vtype == geometry_t && n_dims == 3); return _coords[2]; }
          data_t & z()        { static_assert(vtype == geometry_t && n_dims == 3); return _coords[2]; }

    // Color only.
    const data_t & r() const  { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[0]; }
          data_t & r()        { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[0]; }
    const data_t & g() const  { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[1]; }
          data_t & g()        { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[1]; }
    const data_t & b() const  { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[2]; }
          data_t & b()        { static_assert(vtype == color_t && n_dims >= 3 && n_dims <= 4); return _coords[2]; }
    const data_t & a() const  { static_assert(vtype == color_t && n_dims == 4); return _coords[3]; }
          data_t & a()        { static_assert(vtype == color_t && n_dims == 4); return _coords[3]; }

  private:

    data_t  _coords[n_dims];
  };
}

//////////////////////////////////////////////////////////////////
// Aliases.
namespace ggo
{
  template <typename data_t> using pos2 = vec<data_t, 2, geometry_t>;
  template <typename data_t> using vec2 = vec<data_t, 2, geometry_t>;
  using pos2i = vec<int, 2, geometry_t>;
  using vec2i = vec<int, 2, geometry_t>;
  using pos2f = vec<float, 2, geometry_t>;
  using vec2f = vec<float, 2, geometry_t>;
  using pos2d = vec<double, 2, geometry_t>;
  using vec2d = vec<double, 2, geometry_t>;

  template <typename data_t> using pos3 = vec<data_t, 3, geometry_t>;
  template <typename data_t> using vec3 = vec<data_t, 3, geometry_t>;
  using pos3f = vec<float, 3, geometry_t>;
  using vec3f = vec<float, 3, geometry_t>;
  using pos3d = vec<double, 3, geometry_t>;
  using vec3d = vec<double, 3, geometry_t>;

  template <typename data_t> using color = vec<data_t, 3, color_t>;
  template <typename data_t> using alpha_color = vec<data_t, 4, color_t>;
}

//////////////////////////////////////////////////////////////////
// Global functions.
namespace ggo
{
  template <typename data_t, int n_dims, vec_type vtype>
  data_t dot(const ggo::vec<data_t, n_dims, vtype> & v1, const ggo::vec<data_t, n_dims, vtype> & v2)
  {
    return ggo::dot<n_dims>(v1.data(), v2.data());
  }

  template <typename data_t, int n_dims, vec_type vtype>
  bool compare(const ggo::vec<data_t, n_dims, vtype> & v1, const ggo::vec<data_t, n_dims, vtype> & v2, data_t tolerance)
  {
    return ggo::compare<n_dims>(v1.data(), v2.data(), tolerance);
  }

  template <typename data_t, int n_dims, vec_type vtype>
  std::ostream & operator<<(std::ostream & os, const ggo::vec<data_t, n_dims, vtype> & v)
  {
    ggo::dump<n_dims>(v.data(), os);
    return os;
  }
}

//////////////////////////////////////////////////////////////////
// Arithmetic global operators.
namespace ggo
{
  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator*(const ggo::vec<data_t, n_dims, vtype> & v1, const ggo::vec<data_t, n_dims, vtype> & v2)
  {
    ggo::vec<data_t, n_dims, vtype> r;
    ggo::binary_operation<n_dims>(r.data(), v1.data(), v2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 * src2; });
    return r;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator+(const ggo::vec<data_t, n_dims, vtype> & v1, const ggo::vec<data_t, n_dims, vtype> & v2)
  {
    /*
    // Special case for alpha color.
    if constexpr(vtype == color_t && n_dims == 4)
    {
      if (v1.a() == 0 && v2.a() == 0)
      {
        return { data_t(0), data_t(0), data_t(0), data_t(0) };
      }

      if constexpr(std::is_floating_point<data_t>::value == true)
      {
        data_t a = v1.a() + v2.a() * (1 - v1.a());
        data_t inv = 1 / (v1.a() + v2.a());
        data_t r = inv * (v1.r() * v1.a() + v2.r() * v2.a());
        data_t g = inv * (v1.g() * v1.a() + v2.g() * v2.a());
        data_t b = inv * (v1.b() * v1.a() + v2.b() * v2.a());
        return { r, g, b, a };
      }
      else
      {
        return { 0, 0, 0, 0 };
      }
    }
    else*/
    {
      ggo::vec<data_t, n_dims, vtype> r;
      ggo::binary_operation<n_dims>(r.data(), v1.data(), v2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 + src2; });
      return r;
    }
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator-(const ggo::vec<data_t, n_dims, vtype> & v1, const ggo::vec<data_t, n_dims, vtype> & v2)
  {
    static_assert(vtype != color_t || n_dims != 4);

    ggo::vec<data_t, n_dims, vtype> r;
    ggo::binary_operation<n_dims>(r.data(), v1.data(), v2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 - src2; });
    return r;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator*(data_t k, const ggo::vec<data_t, n_dims, vtype> & v)
  {
    ggo::vec<data_t, n_dims, vtype> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = k * src; });
    return r;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator*(const ggo::vec<data_t, n_dims, vtype> & v, data_t k)
  {
    ggo::vec<data_t, n_dims, vtype> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = src * k; });
    return r;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> operator/(const ggo::vec<data_t, n_dims, vtype> & v, data_t k)
  {
    GGO_ASSERT(k != 0);
    ggo::vec<data_t, n_dims, vtype> r;
    ggo::unary_operation<n_dims>(r.data(), v.data(), [&](data_t & dst, const data_t & src) { dst = src / k; });
    return r;
  }
}

/////////////////////////////////////////////////////////////////////
// 2D only functions.
namespace ggo
{
  template <typename data_t>
  ggo::vec<data_t, 2, geometry_t> rotate(const ggo::vec<data_t, 2, geometry_t> & v, data_t angle)
  {
    data_t cos_tmp = std::cos(angle);
    data_t sin_tmp = std::sin(angle);

    ggo::vec<data_t, 2, geometry_t> r;

    r.x() = v.x() * cos_tmp - v.y() * sin_tmp;
    r.y() = v.x() * sin_tmp + v.y() * cos_tmp;

    return r;
  }

  template <typename data_t>
  ggo::vec<data_t, 2, geometry_t> rotate(const ggo::vec<data_t, 2, geometry_t> & v, const ggo::vec<data_t, 2, geometry_t> & center, data_t angle)
  {
    static_assert(std::is_floating_point<data_t>::value);

    ggo::vec<data_t, 2, geometry_t> rotated(v); // We have to use a temporary in case &center == this.

    rotated -= center;
    rotated = ggo::rotate(rotated, angle);

    return center + rotated;
  }

  template <typename data_t>
  data_t ortho_dot(const ggo::vec<data_t, 2, geometry_t> & v1, const ggo::vec<data_t, 2, geometry_t> & v2)
  {
    return v1.y() * v2.x() - v1.x() * v2.y();
  }

  template <typename data_t>
  ggo::vec<data_t, 2, geometry_t> from_polar(data_t angle, data_t length)
  {
    static_assert(std::is_floating_point<data_t>::value);
    return ggo::vec<data_t, 2, geometry_t>(length * std::cos(angle), length * std::sin(angle));
  }

  template <typename data_t>
  data_t get_angle(const ggo::vec<data_t, 2, geometry_t> & v)
  {
    return std::atan2(v.y(), v.x());
  }

  template <typename data_t>
  data_t get_angle(const ggo::vec<data_t, 2, geometry_t> & v1, const ggo::vec<data_t, 2, geometry_t> & v2)
  {
    GGO_ASSERT(v1.get_hypot() > 0 && v2.get_hypot() > 0);
    return std::abs(std::acos(ggo::dot(v1, v2) / (v1.get_length() * v2.get_length())));
  }
}

/////////////////////////////////////////////////////////////////////
// 3D only functions.
namespace ggo
{
  template <typename data_t>
  ggo::vec<data_t, 3, geometry_t> cross(const ggo::vec<data_t, 3, geometry_t> & v1, const ggo::vec<data_t, 3, geometry_t> & v2)
  {
    return ggo::vec<data_t, 3, geometry_t>(
      v1.y()*v2.z() - v1.z()*v2.y(),
      v1.z()*v2.x() - v1.x()*v2.z(),
      v1.x()*v2.y() - v1.y()*v2.x());
  }

  template <typename data_t>
  bool is_basis(const ggo::vec<data_t, 3, geometry_t> & v1, const ggo::vec<data_t, 3, geometry_t> & v2, const ggo::vec<data_t, 3, geometry_t> & v3)
  {
    static_assert(std::is_floating_point<data_t>::value);

    if (v1.is_normalized() == false || v2.is_normalized() == false || v3.is_normalized() == false)
    {
      return false;
    }

    // v3 should be equal to cross(v1, v2).
    auto c = ggo::cross(v1, v2);

    if (std::abs(v3.x() - c.x()) > static_cast<data_t>(0.0001) ||
        std::abs(v3.y() - c.y()) > static_cast<data_t>(0.0001) ||
        std::abs(v3.z() - c.z()) > static_cast<data_t>(0.0001))
    {
      return false;
    }

    return true;
  }

  template <typename data_t>
  std::pair<ggo::vec<data_t, 3, geometry_t>, ggo::vec<data_t, 3, geometry_t>> build_basis(const ggo::vec<data_t, 3, geometry_t> & v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    GGO_ASSERT(v.is_normalized() == true);

    // Get 2 orthogonal vectors.
    data_t x = std::abs(v.x());
    data_t y = std::abs(v.y());
    data_t z = std::abs(v.z());

    std::pair<ggo::vec<data_t, 3, geometry_t>, ggo::vec<data_t, 3, geometry_t>> basis;

    if ((x <= y) && (x <= z)) // Mininum is X.
    {
      data_t inv_norm = 1 / std::sqrt(v.z() * v.z() + v.y() * v.y());
      basis.first.set(data_t(0), v.z() * inv_norm, -v.y() * inv_norm);
    }
    else if ((y <= x) && (y <= z)) // Mininum is Y.
    {
      data_t inv_norm = 1 / std::sqrt(v.z() * v.z() + v.x() * v.x());
      basis.first.set(v.z() * inv_norm, data_t(0), -v.x() * inv_norm);
    }
    else // Mininum is Z.
    {
      data_t inv_norm = 1 / std::sqrt(v.y() * v.y() + v.x() * v.x());
      basis.first.set(v.y() * inv_norm, -v.x() * inv_norm, data_t(0));
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
  template <typename data_t, int n_dims, vec_type vtype>
  vec<data_t, n_dims, vtype> vec<data_t, n_dims, vtype>::operator-() const
  {
    vec<data_t, n_dims, vtype> v;
    ggo::unary_operation<n_dims>(v._coords, _coords, [](data_t & dst, const data_t & src) { dst = -src; });
    return v;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::operator*=(const vec<data_t, n_dims, vtype> & rhs)
  {
    ggo::unary_operation<n_dims>(_coords, rhs._coords, [](data_t & dst, const data_t & src) { dst *= src; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::operator+=(const vec<data_t, n_dims, vtype> & rhs)
  {
    ggo::unary_operation<n_dims>(_coords, rhs._coords, [](data_t & dst, const data_t & src) { dst += src; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::operator-=(const vec<data_t, n_dims, vtype> & rhs)
  {
    ggo::unary_operation<n_dims>(_coords, rhs._coords, [](data_t & dst, const data_t & src) { dst -= src; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::operator*=(data_t k)
  {
    ggo::for_each<n_dims>(_coords, [&](data_t & d) { d *= k; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::operator/=(data_t k)
  {
    GGO_ASSERT(k != 0);
    ggo::for_each<n_dims>(_coords, [&](data_t & d) { return d /= k; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::flip()
  {
    static_assert(vtype == geometry_t);
    ggo::for_each<n_dims>(_coords, [](data_t & d) { d = -d; });
  }

  template <typename data_t, int n_dims, vec_type vtype>
  data_t vec<data_t, n_dims, vtype>::get_hypot() const
  {
    static_assert(vtype == geometry_t);
    return ggo::dot<n_dims>(_coords, _coords);
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::set_length(data_t length)
  {
    static_assert(std::is_floating_point<data_t>::value);
    static_assert(vtype == geometry_t);
    GGO_ASSERT(get_hypot() > data_t(0));
    this->operator*=(length / get_length());
  }

  template <typename data_t, int n_dims, vec_type vtype>
  void vec<data_t, n_dims, vtype>::normalize()
  {
    static_assert(std::is_floating_point<data_t>::value);
    static_assert(vtype == geometry_t);
    set_length(1);
  }

  template <typename data_t, int n_dims, vec_type vtype>
  bool vec<data_t, n_dims, vtype>::is_normalized(data_t epsilon) const
  {
    static_assert(std::is_floating_point<data_t>::value);
    static_assert(vtype == geometry_t);
    return std::abs(get_hypot() - 1) < epsilon;
  }

  template <typename data_t, int n_dims, vec_type vtype>
  ggo::vec<data_t, n_dims, vtype> vec<data_t, n_dims, vtype>::get_normalized() const
  {
    static_assert(std::is_floating_point<data_t>::value);
    static_assert(vtype == geometry_t);
    ggo::vec<data_t, n_dims, vtype> r(*this);
    r.normalize(); 
    return r;
  }
}

#endif 