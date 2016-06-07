#ifndef __GGO_SET3__
#define __GGO_SET3__

#include <ggo_kernel.h>
#include <ggo_set2.h>

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class set3
  {
  public:

                    set3() {};
                    set3(T x, T y, T z);
                    set3(const set3 & param);

    const set3<T> & operator=(const set3 & param);

    bool				    operator==(const set3 & param) const;
    bool				    operator!=(const set3 & param) const;

    set3<T> &		    operator+=(const set3<T> & param);
    set3<T> &		    operator-=(const set3<T> & param);

    template <typename T2>
    set3<T> &		    operator*=(T2 param) { _x *= param; _y *= param; _z *= param; return *(this); };
    template <typename T2>
    set3<T> &		    operator/=(T2 param) { _x /= param; _y /= param; _z /= param; return *(this); };

    set3<T>			    operator-() const;

    // Vector/position interface.
    T					      x() const { return _x; }
    T					      y() const { return _y; }
    T					      z() const { return _z; }

    T &					    x() { return _x; }
    T &					    y() { return _y; }
    T &					    z() { return _z; }
    
    void            set(T x, T y, T z) { _x = x; _y = y; _z = z; }

    T					      get_hypot() const { return _x*_x+_y*_y+_z*_z; }
    T					      get_length() const { return std::sqrt(_x*_x+_y*_y+_z*_z); }

    void            flip() { _x = -_x; _y = -_y; _z = -_z; }

    void				    normalize();
    void				    set_length(T length);
    bool				    is_normalized(T epsilon = T(0.001)) const;
    ggo::set3<T>    get_normalized() const { ggo::set3<T> v(*this); v.normalize(); return v; }

    void            build_basis(set3 & b1, set3 & b2) const;
    
    ggo::set2<T>    xy() const { return ggo::set2<T>(_x, _y); }
    ggo::set2<T>    yz() const { return ggo::set2<T>(_y, _z); }
    ggo::set2<T>    xz() const { return ggo::set2<T>(_x, _z); }

  private:

    T	_x;
    T	_y;
    T	_z;
  };
}

//////////////////////////////////////////////////////////////////
// Dot product.
namespace ggo
{
  template <typename T>
  T dot(const set3<T> & v1, const set3<T> & v2)
  {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
  }
}

//////////////////////////////////////////////////////////////////
// Cross product.
namespace ggo
{
  template <typename T>
  set3<T> cross(const set3<T> & v1, const set3<T> & v2)
  {
    return set3<T>(v1.y()*v2.z() - v1.z()*v2.y(),
      v1.z()*v2.x() - v1.x()*v2.z(),
      v1.x()*v2.y() - v1.y()*v2.x());
  }
}

//////////////////////////////////////////////////////////////////
// Basis test.
namespace ggo
{
  template <typename T>
  bool is_basis(const set3<T> & v1, const set3<T> & v2, const set3<T> & v3)
  {
    if (v1.is_normalized(static_cast<T>(0.0001)) == false ||
        v2.is_normalized(static_cast<T>(0.0001)) == false ||
        v3.is_normalized(static_cast<T>(0.0001)) == false)
    {
      return false;
    }

    if (std::abs(ggo::dot(v1, v2)) > static_cast<T>(0.0001) ||
        std::abs(ggo::dot(v1, v3)) > static_cast<T>(0.0001) ||
        std::abs(ggo::dot(v2, v3)) > static_cast<T>(0.0001))
    {
      return false;
    }

    return true;
  }
}

//////////////////////////////////////////////////////////////////
// Global operators.
namespace ggo
{
  template <typename T> set3<T> operator+(const set3<T> & set1, const set3<T> & set3);
  template <typename T> set3<T> operator-(const set3<T> & set1, const set3<T> & set3);

  template <typename T> set3<T> operator*(T val, const set3<T> & param);
  template <typename T> set3<T> operator*(const set3<T> & param, T val);
  template <typename T> set3<T> operator/(const set3<T> & param, T val);
}

//////////////////////////////////////////////////////////////////
// I/O
namespace ggo
{
  template <typename T>
  std::ostream & operator<<(std::ostream &os, const set3<T> & param)
  {
    return os << '(' << param.x() << ", " << param.y() << ", " << param.z() << ')';
  }
}

//////////////////////////////////////////////////////////////////
// Type aliases.
namespace ggo
{
  using point3d_double    = set3<double>;
  using point3d_float     = set3<float>;
  using point3d_int       = set3<int>;
  using vector3d_double   = set3<double>;
  using vector3d_float    = set3<float>;
  using vector3d_int      = set3<int>;

  template <typename data_t> using vector3d = set3<data_t>;
  template <typename data_t> using point3d  = set3<data_t>;
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  set3<T>::set3(T x, T y, T z)
  {
    _x = x;
    _y = y;
    _z = z;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T>::set3(const set3 & param)
  {
    _x = param._x;
    _y = param._y;
    _z = param._z;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  const set3<T> & set3<T>::operator=(const set3 & param)
  {
    _x = param._x;
    _y = param._y;
    _z = param._z;

    return ( *this );
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool set3<T>::operator==(const set3 & param) const
  {
    return _x == param._x && _y == param._y && _z == param._z;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool set3<T>::operator!=(const set3 & param) const
  {
    return _x != param._x || _y != param._y || _z != param._z;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> & set3<T>::operator+=(const set3 & param)
  {
    x() += param.x();
    y() += param.y();
    z() += param.z();
    return *(this);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> & set3<T>::operator-=(const set3 & param)
  {
    x() -= param.x();
    y() -= param.y();
    z() -= param.z();
    return *(this);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> set3<T>::operator-() const
  {
    return set3<T>(-_x, -_y, -_z);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> operator+(const set3<T> & param1, const set3<T> & param2)
  {
    return set3<T>(param1.x() + param2.x(), param1.y() + param2.y(), param1.z() + param2.z());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> operator-(const set3<T> & param1, const set3<T> & param2)
  {
    return set3<T>(param1.x() - param2.x(), param1.y() - param2.y(), param1.z() - param2.z() );
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> operator*(T val, const set3<T> & param)
  {
    return set3<T>(val * param.x(), val * param.y(), val * param.z());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> operator*(const set3<T> & param, T val)
  {
    return set3<T>(val * param.x(), val * param.y(), val * param.z());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  set3<T> operator/(const set3<T> & param, T val)
  {
    return set3<T>(param.x() / val, param.y() / val, param.z() / val);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void set3<T>::set_length(T length)
  {
    GGO_ASSERT(get_hypot() > T(0));

    T k = length / get_length();

    _x *= k;
    _y *= k;
    _z *= k;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void set3<T>::normalize()
  {
    set_length(T(1));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool set3<T>::is_normalized(T epsilon) const
  {
    return std::abs(_x * _x + _y * _y + _z * _z - T(1)) < epsilon;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void set3<T>::build_basis(set3 & b1, set3 & b2) const
  {
    GGO_ASSERT(this->is_normalized(T(0.001)) == true);
    
    // Get 2 orthogonal vectors.
    float x = std::abs(_x);
    float y = std::abs(_y);
    float z = std::abs(_z);

    if ((x <= y) && (x <= z)) // Mininum is X.
    {
      T inv_norm = 1 / std::sqrt(_z * _z + _y * _y);
      b1.set(0, _z * inv_norm, -_y * inv_norm);
      b2.set(_y * b1._z - _z * b1._y, -_x * b1._z, _x * b1._y - _y * b1._x);
    }
    else if ((y <= x) && (y <= z)) // Mininum is Y.
    {
      T inv_norm = 1 / std::sqrt(_z * _z + _x * _x);
      b1.set(_z * inv_norm, 0, -_x * inv_norm);
      b2.set(_y * b1._z, _z * b1._x - _x * b1._z, -_y * b1._x);
    }
    else // Mininum is Z.
    {
      T inv_norm = 1 / std::sqrt(_y * _y + _x * _x);
      b1.set(_y * inv_norm, -_x * inv_norm, 0);
      b2.set(-_z * b1._y, _z * b1._x, _x * b1._y - _y * b1._x);
    }

    GGO_ASSERT(ggo::is_basis(*this, b1, b2));
  }
}

#endif
