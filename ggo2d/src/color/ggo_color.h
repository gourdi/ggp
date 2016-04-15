#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <ggo_kernel.h>
#include <iostream>

namespace ggo
{
  class color
  {
  public:

              color() {}
    explicit  color(float gray) { _r = gray; _g = gray; _b = gray; }
              color(float r, float g, float b) { _r = r; _g = g; _b = b; }
              color(const color & c) { _r = c._r; _g = c._g; _b = c._b; }

    void		  operator+=(const color & c) { _r += c._r; _g += c._g; _b += c._b; }
    void		  operator-=(const color & c) { _r -= c._r; _g -= c._g; _b -= c._b; }
    void		  operator*=(const color & c) { _r *= c._r; _g *= c._g; _b *= c._b; }
    void		  operator*=(float v) { _r *= v; _g *= v; _b *= v; }
    void		  operator/=(float v) { _r /= v; _g /= v; _b /= v; }

    float &		r() { return _r; }
    float &		g() { return _g; }
    float &		b() { return _b; }

    float 		r() const { return _r; }
    float 		g() const { return _g; }
    float 		b() const { return _b; }

    uint8_t 	r8() const { return ggo::to<uint8_t>(0xFF*_r); }
    uint8_t 	g8() const { return ggo::to<uint8_t>(0xFF*_g); }
    uint8_t 	b8() const { return ggo::to<uint8_t>(0xFF*_b); }

    uint16_t 	r16() const { return ggo::to<uint16_t>(0xFFFF*_r); }
    uint16_t 	g16() const { return ggo::to<uint16_t>(0xFFFF*_g); }
    uint16_t 	b16() const { return ggo::to<uint16_t>(0xFFFF*_b); }

    uint32_t 	r32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_r); }
    uint32_t 	g32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_g); }
    uint32_t 	b32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_b); }

    void 		  randomize();

    float		  max() const { return ggo::max(_r, _g, _b); }
    float		  min() const { return ggo::min(_r, _g, _b); }
    float		  mean() const { return (_r + _g + _b) / 3; }
    
    bool      is_black(float epsilon = 0.001f) const { return _r < epsilon && _g < epsilon && _b < epsilon; }

    color &   operator=(const color & c) = default;

  public:

    static color	from_hsv(float hue, float saturation, float value);
    static color	get_random();

    static color	linear_interpolate(float x0, const color & color0, float x1, const color & c1, float x);

    static float	max_diff(const color & c1, const color & c2);

  public:

    static const color WHITE;
    static const color GRAY;
    static const color BLACK;
    static const color RED;
    static const color GREEN;
    static const color BLUE;
    static const color CYAN;
    static const color MAGENTA;
    static const color YELLOW;
    static const color ORANGE;

  private:

    float	_r;
    float	_g;
    float	_b;
  };
}

namespace ggo
{
  inline std::ostream & operator<<(std::ostream & os, const ggo::color & c)
  {
    os << c.r() << ' ' << c.g() << ' ' << c.b();
    return os;
  }

  inline std::istream & operator>>(std::istream & is, ggo::color & c)
  {
    is >> c.r() >> c.g() >> c.b();
    return is;
  }

  inline ggo::color operator-(const ggo::color & c)
  {
    return ggo::color(-c.r(), -c.g(), -c.b());
  }

  inline ggo::color operator+(const ggo::color & c1, const ggo::color & c2)
  {
    return ggo::color(c1.r() + c2.r(), c1.g() + c2.g(), c1.b() + c2.b());
  }

  inline ggo::color operator-(const ggo::color & c1, const ggo::color & c2)
  {
    return ggo::color(c1.r() - c2.r(), c1.g() - c2.g(), c1.b() - c2.b());
  }

  inline ggo::color operator*(const ggo::color & c1, const ggo::color & c2)
  {
    return ggo::color(c1.r() * c2.r(), c1.g() * c2.g(), c1.b() * c2.b());
  }

  inline ggo::color operator*(const ggo::color & c, float k)
  {
    return ggo::color(k * c.r(), k * c.g(), k * c.b());
  }

  inline ggo::color operator*(float k, const ggo::color & c)
  {
    return ggo::color(k * c.r(), k * c.g(), k * c.b());
  }

  inline ggo::color operator/(const ggo::color & c, float k)
  {
    return ggo::color(c.r() / k, c.g() / k, c.b() / k);   
  }
}

#endif
