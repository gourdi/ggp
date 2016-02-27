#ifndef __GGO_SET2__
#define __GGO_SET2__

#include <cmath>
#include <iostream>

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class set2
  {
  public:

                    set2() {}
                    set2(T x, T y) : _x(x), _y(y) {}
                    set2(const set2 & p) : _x(p._x), _y(p._y) {}

    const set2<T> &	operator=(const set2 & param);

    bool				    operator==(const set2 & param) const;
    bool				    operator!=(const set2 & param) const;

    set2<T>&		    operator+=(const set2<T> & param);
    set2<T>&		    operator-=(const set2<T> & param);
      
    set2<T>         operator-() { return set2<T>(-_x, -_y); }

    set2<T>&		    operator*=(T param) { _x *= param, _y *= param; return *this; }
    set2<T>&		    operator/=(T param) { _x /= param, _y /= param; return *this; }

    // Set interface.
    template <typename Func>
    void				    map(Func func) { _x = func(_x); _y = func(_y); }

    // Vector/position interface.
    T					      x() const { return _x; }
    T					      y() const { return _y; }

    T &					    x() { return _x; }
    T &					    y() { return _y; }

    T					      get_hypot() const { return _x * _x + _y * _y; }
    T					      get_length() const { return std::sqrt(_x * _x + _y * _y); }
    T					      get_angle() const { return std::atan2(_y, _x); }
    
    // Creation from polar coordinates.
    static set2<T>	from_polar(T angle, T length) { return set2(length * std::cos(angle), length * std::sin(angle)); }

    // Size interface.
    T					      width() const  { return _x; }
    T					      height() const { return _y; }
  
    T &					    width()  { return _x; }
    T &					    height() { return _y; }
  
    T					      area() const { return _x * _y; }
  
    void				    move(T dx, T dy) { _x += dx; _y += dy; }
    void				    rotate(T angle);
    void				    rotate(T angle, const set2<T> & center);
        
    void				    normalize() { set_length(T(1)); }
    void				    set_length(T length) { T ratio = length / get_length(); _x *= ratio; _y *= ratio; }
    ggo::set2<T>    get_normalized() const { ggo::set2<T> v(*this); v.normalize(); return v; }
    bool            is_normalized(T epsilon = T(0.001)) const { return std::abs(get_length() - 1) < epsilon; }

  private:

    T	_x;
    T	_y;
  };
}

//////////////////////////////////////////////////////////////////
// Dot product.
namespace ggo
{
  template <typename T>
  T dot(const set2<T> & v1, const set2<T> & v2)
  {
    return v1.x() * v2.x() + v1.y() * v2.y();
  }
}

//////////////////////////////////////////////////////////////////
// Aithmetic global operators.
namespace ggo
{
  template <typename T>
  set2<T> operator+(const set2<T> & param1, const set2<T> & param2)
  {
    return set2<T>(param1.x()+param2.x(), param1.y()+param2.y());
  }

  template <typename T>
  set2<T> operator-(const set2<T> & param1, const set2<T> & param2)
  {
    return set2<T>(param1.x()-param2.x(), param1.y()-param2.y());
  }

  template <typename T>
  set2<T> operator*(T val, const set2<T> & param)
  {
    return set2<T>(val * param.x(), val * param.y());
  }

  template <typename T>
  set2<T> operator*(const set2<T> & param, T val)
  {
    return set2<T>(val * param.x(), val * param.y());
  }
  
  template <typename T>
  set2<T> operator/(const set2<T> & param, T val)
  {
    return set2<T>(param.x() / val, param.y() / val);
  }
}
  
//////////////////////////////////////////////////////////////////
// Type aliases.
namespace ggo
{
  typedef set2<double>  point2d_double;
  typedef set2<float>   point2d_float;
  typedef set2<int>     point2d_int;
  typedef set2<double>  vector2d_double;
  typedef set2<float>   vector2d_float;
  typedef set2<int>     vector2d_int;
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void set2<T>::rotate(T angle)
  {
    T cos_tmp = std::cos(angle);
    T sin_tmp = std::sin(angle);
    
    T x_tmp = _x * cos_tmp - _y * sin_tmp;
    T y_tmp = _x * sin_tmp + _y * cos_tmp;

    _x = x_tmp;
    _y = y_tmp;		
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void set2<T>::rotate(T angle, const set2<T> & center)
  {
    set2<T> rotated(*this); // We have to use a temporary in case &center == this.

    rotated -= center;
    rotated.rotate(angle);

    *this = center + rotated;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  const set2<T> & set2<T>::operator=(const set2 & param)
  {
    _x = param._x;
    _y = param._y;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  // Boolean operators.
  template <typename T>
  bool set2<T>::operator==(const set2 & param) const
  {
    return (_x == param._x) && (_y == param._y);
  }

  template <typename T>
  bool set2<T>::operator!=(const set2 & param) const
  {
    return (_x != param._x) || (_y != param._y);
  }

  /////////////////////////////////////////////////////////////////////
  // In place operators.
  template <typename T>
  set2<T> & set2<T>::operator+=(const set2 & param)
  {
    x() += param.x();
    y() += param.y();
    return *this;
  }

  template <typename T>
  set2<T> & set2<T>::operator-=(const set2 & param)
  {
    x() -= param.x();
    y() -= param.y();
    return *this;
  }
}

/////////////////////////////////////////////////////////////////////
// I/O operators.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::ostream & operator<<(std::ostream & os, const set2<T> & s)
  {
    os << s.x() << ' ' << s.y();
    return os;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::istream & operator>>(std::istream & is, set2<T> & s)
  {
    is >> s.x() >> s.y();
    return is;
  }
}

#endif
