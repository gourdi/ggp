#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <sstream>
#include <string>
#include <ostream>
#include <kernel/ggo_kernel.h>

// Structs.
namespace ggo
{
  // YA
  template <typename data_t>
  struct ya
  {
    ya() {}
    ya(data_t k) : _y(k), _a(k) {}
    ya(data_t y, data_t a) : _y(y), _a(a) {}

    ya<data_t>  operator-() const { return { -this->_y, -this->_a }; }

    void        operator+=(ggo::ya<data_t> rhs) { this->_y += rhs._y; this->_a += rhs._a; }
    void        operator-=(ggo::ya<data_t> rhs) { this->_y -= rhs._y; this->_a -= rhs._a; }
    void        operator*=(data_t k) { this->_y *= k; this->_a *= k; }
    void        operator/=(data_t k) { this->_y /= k; this->_a /= k; }

    bool        operator==(ggo::ya<data_t> rhs) const { return this->_y == rhs._y && this->_a == rhs._a; }
    bool        operator!=(ggo::ya<data_t> rhs) const { return this->_y != rhs._y || this->_a != rhs._a; }

    data_t _y;
    data_t _a;
  };

  template <typename data_t>
  ya<data_t> operator+(ya<data_t> c1, ya<data_t> c2) { return { data_t(c1._y + c2._y), data_t(c1._a + c2._a) }; }

  template <typename data_t>
  ya<data_t> operator-(ya<data_t> c1, ya<data_t> c2) { return { data_t(c1._y - c2._y), data_t(c1._a - c2._a) }; }

  template <typename data_t>
  ya<data_t> operator*(data_t k, ya<data_t> c) { return { data_t(k * c._y), data_t(k * c._a) }; }

  template <typename data_t>
  ya<data_t> operator*(ya<data_t> c, data_t k) { return { data_t(k * c._y), data_t(k * c._a) }; }

  template <typename data_t>
  ya<data_t> operator/(ya<data_t> c, data_t k) { return { data_t(c._y / k), data_t(c._a / k) }; }

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, ya<data_t> c) { os << "(y:" << c._y << "; a:" << c._a << ")"; return os; }

  using ya_8u = ggo::ya<uint8_t>;
  using ya_32f = ggo::ya<float>;

  // RGB
  template <typename data_t> 
  struct rgb
  {
    rgb() {}
    rgb(data_t k) : _r(k), _g(k), _b(k) {}
    rgb(data_t r, data_t g, data_t b) : _r(r), _g(g), _b(b) {}

    rgb<data_t> operator-() const { return { -this->_r, -this->_g, -this->_b }; }

    void        operator+=(ggo::rgb<data_t> rhs) { this->_r += rhs._r; this->_g += rhs._g; this->_b += rhs._b; }
    void        operator-=(ggo::rgb<data_t> rhs) { this->_r -= rhs._r; this->_g -= rhs._g; this->_b -= rhs._b; }
    void        operator*=(ggo::rgb<data_t> rhs) { this->_r *= rhs._r; this->_g *= rhs._g; this->_b *= rhs._b; }
    void        operator*=(data_t k) { this->_r *= k; this->_g *= k; this->_b *= k;}
    void        operator/=(data_t k) { this->_r /= k; this->_g /= k; this->_b /= k;}

    bool        operator==(ggo::rgb<data_t> rhs) const { return this->_r == rhs._r && this->_g == rhs._g && this->_b == rhs._b; }
    bool        operator!=(ggo::rgb<data_t> rhs) const { return this->_r != rhs._r || this->_g != rhs._g || this->_b != rhs._b; }

    data_t _r;
    data_t _g;
    data_t _b;
  };

  template <typename data_t>
  rgb<data_t> operator+(rgb<data_t> c1, rgb<data_t> c2) { return { data_t(c1._r + c2._r), data_t(c1._g + c2._g), data_t(c1._b + c2._b) }; }

  template <typename data_t>
  rgb<data_t> operator-(rgb<data_t> c1, rgb<data_t> c2) { return { data_t(c1._r - c2._r), data_t(c1._g - c2._g), data_t(c1._b - c2._b) }; }

  template <typename data_t>
  rgb<data_t> operator*(rgb<data_t> c1, rgb<data_t> c2) { return { data_t(c1._r * c2._r), data_t(c1._g * c2._g), data_t(c1._b * c2._b) }; }

  template <typename data_t>
  rgb<data_t> operator*(data_t k, rgb<data_t> c) { return { data_t(k * c._r), data_t(k * c._g), data_t(k * c._b) }; }

  template <typename data_t>
  rgb<data_t> operator*(rgb<data_t> c, data_t k) { return { data_t(k * c._r), data_t(k * c._g), data_t(k * c._b) }; }

  template <typename data_t>
  rgb<data_t> operator/(rgb<data_t> c, data_t k) { return { data_t(c._r / k), data_t(c._g / k), data_t(c._b / k) }; }

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, rgb<data_t> c) { os << "(r:" << c._r << "; g:" << c._g << "; b:" << c._b << ")"; return os; }

  using rgb_8u = ggo::rgb<uint8_t>;
  using rgb_16u = ggo::rgb<uint16_t>;
  using rgb_32u = ggo::rgb<uint32_t>;
  using rgb_32f = ggo::rgb<float>;

  // RGBA
  template <typename data_t>
  struct rgba
  {
    rgba() {}
    rgba(data_t k) : _r(k), _g(k), _b(k), _a(k) {}
    rgba(const ggo::rgb<data_t> & rgb, data_t a) : _r(rgb._r), _g(rgb._g), _b(rgb._b), _a(a) {}
    rgba(data_t r, data_t g, data_t b, data_t a) : _r(r), _g(g), _b(b), _a(a) {}

    rgb<data_t>   rgb() const { return { this->_r, this->_g, this->_b }; }

    rgba<data_t>  operator-() const { return { -this->_r, -this->_g, -this->_b, -this->_a }; }

    void          operator+=(ggo::rgba<data_t> rhs) { this->_r += rhs._r; this->_g += rhs._g; this->_b += rhs._b; this->_a += rhs._a; }
    void          operator-=(ggo::rgba<data_t> rhs) { this->_r -= rhs._r; this->_g -= rhs._g; this->_b -= rhs._b; this->_a -= rhs._a;}
    void          operator*=(data_t k) { this->_r *= k; this->_g *= k; this->_b *= k; this->_a *= k; }
    void          operator/=(data_t k) { this->_r /= k; this->_g /= k; this->_b /= k; this->_a /= k; }

    bool          operator==(ggo::rgba<data_t> rhs) const { return this->_r == rhs._r && this->_g == rhs._g && this->_b == rhs._b && this->_a == rhs._a; }
    bool          operator!=(ggo::rgba<data_t> rhs) const { return this->_r != rhs._r || this->_g != rhs._g || this->_b != rhs._b || this->_a != rhs._a; }

    data_t _r;
    data_t _g;
    data_t _b;
    data_t _a;
  };

  template <typename data_t>
  rgba<data_t> operator+(rgba<data_t> c1, rgba<data_t> c2) { return { data_t(c1._r + c2._r), data_t(c1._g + c2._g), data_t(c1._b + c2._b), data_t(c1._a + c2._a) }; }

  template <typename data_t>
  rgba<data_t> operator-(rgba<data_t> c1, rgba<data_t> c2) { return { data_t(c1._r - c2._r), data_t(c1._g - c2._g), data_t(c1._b + c2._b), data_t(c1._a - c2._a) }; }

  template <typename data_t>
  rgba<data_t> operator*(data_t k, rgba<data_t> c) { return { data_t(k * c._r), data_t(k * c._g), data_t(k * c._b), data_t(k * c._a) }; }

  template <typename data_t>
  rgba<data_t> operator*(rgba<data_t> c, data_t k) { return { data_t(k * c._r), data_t(k * c._g), data_t(k * c._b), data_t(k * c._a) }; }
  
  template <typename data_t>
  rgba<data_t> operator/(rgba<data_t> c, data_t k) { return { data_t(c._r / k), data_t(c._g / k), data_t(c._b / k), data_t(c._a / k) }; }

  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, rgba<data_t> c) { os << "(r:" << c._r << "; g:" << c._g << "; b:" << c._b << "; a:" << c._a << ")"; return os; }

  using rgba_8u = ggo::rgba<uint8_t>;
  using rgba_16u = ggo::rgba<uint16_t>;
  using rgba_32u = ggo::rgba<uint32_t>;
  using rgba_32f = ggo::rgba<float>;
}

/////////////////////////////////////////////////////////////////////
// Color spaces
namespace ggo
{
  enum class color_space
  {
    y,
    ya,
    rgb,
    rgba
  };
}

/////////////////////////////////////////////////////////////////////
// Color traits
namespace ggo
{
  template <typename color_t>
  struct color_traits {};

  template <>
  struct color_traits<uint8_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint8_t;
    using sample_t = uint8_t;
    static constexpr uint8_t max = 0xff;
    static constexpr color_space color_space = color_space::y;
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<uint16_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint16_t;
    using sample_t = uint16_t;
    static constexpr uint16_t max = 0xffff;
    static constexpr color_space color_space = color_space::y;
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<uint32_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint32_t;
    using sample_t = uint32_t;
    static constexpr uint32_t max = 0xffffffff;
    static constexpr color_space color_space = color_space::y;
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<float>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = float;
    using sample_t = float;
    static constexpr float max = 1.f;
    static constexpr color_space color_space = color_space::y;
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ya_8u>
  {
    using floating_point_color_t = ya_32f;
    using no_alpha_color_t = uint8_t;
    using sample_t = uint8_t;
    static constexpr float max = 0xff;
    static constexpr color_space color_space = color_space::ya;
    static constexpr int samples_count = 2;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ya_32f>
  {
    using floating_point_color_t = ya_32f;
    using no_alpha_color_t = float;
    using sample_t = float;
    static constexpr float max = 1.f;
    static constexpr color_space color_space = color_space::ya;
    static constexpr int samples_count = 2;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ggo::rgb_8u>
  {
    using floating_point_color_t = ggo::rgb_32f;
    using no_alpha_color_t = ggo::rgb_8u;
    using alpha_color_t = ggo::rgba_8u;
    using sample_t = uint8_t;
    static constexpr uint8_t max = 0xff;
    static constexpr color_space color_space = color_space::rgb;
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::rgb_16u>
  {
    using floating_point_color_t = ggo::rgb_32f;
    using no_alpha_color_t = ggo::rgb_16u;
    using alpha_color_t = ggo::rgba_16u;
    using sample_t = uint16_t;
    static constexpr uint16_t max = 0xffff;
    static constexpr color_space color_space = color_space::rgb;
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::rgb_32u>
  {
    using floating_point_color_t = ggo::rgb_32f;
    using no_alpha_color_t = ggo::rgb_32u;
    using alpha_color_t = ggo::rgba_32u;
    using sample_t = uint32_t;
    static constexpr uint32_t max = 0xffffffff;
    static constexpr color_space color_space = color_space::rgb;
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::rgb_32f>
  {
    using floating_point_color_t = ggo::rgb_32f;
    using no_alpha_color_t = ggo::rgb_32f;
    using alpha_color_t = ggo::rgba_32f;
    using sample_t = float;
    static constexpr float max = 1.f;
    static constexpr color_space color_space = color_space::rgb;
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::rgba_8u>
  {
    using floating_point_color_t = ggo::rgba_32f;
    using no_alpha_color_t = ggo::rgb_8u;
    using alpha_color_t = ggo::rgba_8u;
    using sample_t = uint8_t;
    static constexpr uint8_t max = 0xff;
    static constexpr color_space color_space = color_space::rgba;
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ggo::rgba_32u>
  {
    using floating_point_color_t = ggo::rgba_32f;
    using no_alpha_color_t = ggo::rgb_32u;
    using alpha_color_t = ggo::rgba_32u;
    using sample_t = uint32_t;
    static constexpr uint32_t max() { return 0xffffffff; }
    static constexpr color_space color_space = color_space::rgba;
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ggo::rgba_32f>
  {
    using floating_point_color_t = ggo::rgba_32f;
    using no_alpha_color_t = ggo::rgb_32f;
    using alpha_color_t = ggo::rgba_32f;
    using sample_t = float;
    static constexpr float max = 1.f;
    static constexpr color_space color_space = color_space::rgba;
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };
}

// The following traits makes it possible to have a floating point color type of a given precision.
namespace ggo
{
  template <typename color_t, typename floating_point_t>
  struct floating_point_color_traits {};

  template <>
  struct floating_point_color_traits<uint8_t, float>
  {
    using floating_point_color_t = float;
  };

  template <>
  struct floating_point_color_traits<float, float>
  {
    using floating_point_color_t = float;
  };

  template <>
  struct floating_point_color_traits<ggo::rgb_8u, float>
  {
    using floating_point_color_t = ggo::rgb_32f;
  };

  template <>
  struct floating_point_color_traits<ggo::rgb_32f, float>
  {
    using floating_point_color_t = ggo::rgb_32f;
  };
}

/////////////////////////////////////////////////////////////////////
// Color conversion
namespace ggo
{
  template <typename sample_out_t, typename sample_in_t>
  sample_out_t convert_sample_to(sample_in_t s)
  {
    static_assert(std::is_same<sample_in_t, sample_out_t>::value == true);
    return s;
  }

  // 8u => 16u
  template <> inline uint16_t convert_sample_to(uint8_t s) { return (uint16_t(s) << 8) | s; }
  // 8u => 32u
  template <> inline uint32_t convert_sample_to(uint8_t s) { return (uint32_t(s) << 24) | (uint32_t(s) << 16) | (uint32_t(s) << 8) | s; }
  // 8u => 32f
  template <> inline float convert_sample_to(uint8_t s) { return s / float(std::numeric_limits<uint8_t>::max()); }

  // 16u => 8u
  template <> inline uint8_t convert_sample_to(uint16_t s) { return static_cast<uint8_t>(s >> 8); }
  // 16u => 32u
  template <> inline uint32_t convert_sample_to(uint16_t s) { return (uint32_t(s) << 16) | s; }
  // 16u => 32f
  template <> inline float convert_sample_to(uint16_t s) { return s / float(std::numeric_limits<uint16_t>::max()); }

  // 32u => 8u
  template <> inline uint8_t convert_sample_to(uint32_t s) { return static_cast<uint8_t>(s >> 24); }
  // 32u => 16u
  template <> inline uint16_t convert_sample_to(uint32_t s) { return static_cast<uint16_t>(s >> 16); }
  // 32u => 32f
  template <> inline float convert_sample_to(uint32_t s) { return s / float(std::numeric_limits<uint32_t>::max()); }
  
  // 32f => 8u
  template <> inline uint8_t convert_sample_to(float s) { return static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * ggo::clamp(s, 0.f, 1.f) + 0.5f); }
  // 32f => 16u
  template <> inline uint16_t convert_sample_to(float s) { return static_cast<uint16_t>(std::numeric_limits<uint16_t>::max() * ggo::clamp(s, 0.f, 1.f) + 0.5f); }
  // 32f => 32u
  template <> inline uint32_t convert_sample_to(float s) { return static_cast<uint32_t>(std::numeric_limits<uint32_t>::max() * ggo::clamp(s, 0.f, 1.f) + 0.5f); }

  template <typename sample_t>
  sample_t rgb_to_y(sample_t r, sample_t g, sample_t b)
  {
    return (r + g + b) / 3;
  }

  template <typename color_out_t, typename color_in_t>
  color_out_t convert_color_to(const color_in_t & c)
  {
    using color_traits_in = color_traits<color_in_t>;
    using color_traits_out = color_traits<color_out_t>;

    if constexpr(color_traits_in::color_space == color_space::y)
    {
      if constexpr(color_traits_out::color_space == color_space::y) // y => y
      {
        return convert_sample_to<color_traits_out::sample_t>(c);
      }
      else if constexpr(color_traits_out::color_space == color_space::ya) // y => ya
      {
        return { convert_sample_to<color_traits_out::sample_t>(c), color_traits_out::max };
      }
      else if constexpr(color_traits_out::color_space == color_space::rgb) // y => rgb
      {
        auto y_out = convert_sample_to<color_traits_out::sample_t>(c);
        return { y_out, y_out, y_out };
      }
      else // y => rgba
      {
        static_assert(color_traits_out::color_space == color_space::rgba);
        auto y_out = convert_sample_to<color_traits_out::sample_t>(c);
        return { y_out, y_out, y_out, color_traits_out::max };
      }
    }
    else if constexpr(color_traits_in::color_space == color_space::ya)
    {
      if constexpr(color_traits_out::color_space == color_space::y) // ya => y
      {
        return convert_sample_to<color_traits_out::sample_t>(c._y);
      }
      else if constexpr(color_traits_out::color_space == color_space::ya) // ya => ya
      {
        return { convert_sample_to<color_traits_out::sample_t>(c._y), convert_sample_to<color_traits_out::sample_t>(c._a) };
      }
      else if constexpr(color_traits_out::color_space == color_space::rgb) // ya => rgb
      {
        auto y_out = convert_sample_to<color_traits_out::sample_t>(c._y);
        return { y_out, y_out, y_out };
      }
      else // ya => rgb
      {
        static_assert(color_traits_out::color_space == color_space::rgba);
        auto y_out = convert_sample_to<color_traits_out::sample_t>(c._y);
        return { y_out, y_out, y_out, convert_sample_to<color_traits_out::sample_t>(c._a) };
      }
    }
    else if constexpr(color_traits_in::color_space == color_space::rgb)
    {
      if constexpr(color_traits_out::color_space == color_space::y) // rgb => y
      {
        return convert_sample_to<color_traits_out::sample_t>(rgb_to_y(c._r, c._g, c._b));
      }
      else if constexpr(color_traits_out::color_space == color_space::ya) // rgb => ya
      {
        return { convert_sample_to<color_traits_out::sample_t>(rgb_to_y(c._r, c._g, c._b)), color_traits_out::max };
      }
      else if constexpr(color_traits_out::color_space == color_space::rgb) // rgb => rgb
      {
        return {
          convert_sample_to<color_traits_out::sample_t>(c._r),
          convert_sample_to<color_traits_out::sample_t>(c._g),
          convert_sample_to<color_traits_out::sample_t>(c._b) };
      }
      else // rgb => rgb
      {
        static_asssert(color_traits_out::color_space == color_space::rgba);
        return {
          convert_sample_to<color_traits_out::sample_t>(c._r),
          convert_sample_to<color_traits_out::sample_t>(c._g),
          convert_sample_to<color_traits_out::sample_t>(c._b),
          color_traits_out::max };
      }
    }
    else 
    {
      static_assert(color_traits_in::color_space == color_space::rgba);
      if constexpr(color_traits_out::color_space == color_space::y) // rgba => y
      {
        return convert_sample_to<color_traits_out::sample_t>(rgb_to_y(c._t, c._g, c._b));
      }
      else if constexpr(color_traits_out::color_space == color_space::ya) // rgba => y
      {
        return { convert_sample_to<color_traits_out::sample_t>(rgb_to_y(c._t, c._g, c._b)), color_traits_out::max };
      }
      else if constexpr(color_traits_out::color_space == color_space::rgb) // rgba => rgb
      {
        return {
          convert_sample_to<color_traits_out::sample_t>(c._r),
          convert_sample_to<color_traits_out::sample_t>(c._g),
          convert_sample_to<color_traits_out::sample_t>(c._b) };
      }
      else // rgba => rgba
      {
        static_assert(color_traits_out::color_space == color_space::rgba);
        return {
          convert_sample_to<color_traits_out::sample_t>(c._r),
          convert_sample_to<color_traits_out::sample_t>(c._g),
          convert_sample_to<color_traits_out::sample_t>(c._b),
          convert_sample_to<color_traits_out::sample_t>(c._a) };
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////
// Static variables.
namespace ggo
{
  template <typename color_t> color_t white()
  { 
    const auto v = color_traits<color_t>::max;
    return color_t(v, v, v);
  }

  template <typename color_t> color_t gray()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const sample_t v = color_traits<color_t>::max / 2;
    return color_t(v, v, v);
  }

  template <typename color_t> color_t black()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    return color_t(sample_t(0), sample_t(0), sample_t(0));
  }

  template <typename color_t> color_t red()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(v, sample_t(0), sample_t(0));
  }

  template <typename color_t> color_t green()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(sample_t(0), v, sample_t(0));
  }

  template <typename color_t> color_t blue()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(sample_t(0), sample_t(0), v);
  }

  template <typename color_t> color_t cyan()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(sample_t(0), v, v);
  }

  template <typename color_t> color_t magenta()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(v, sample_t(0), v);
  }

  template <typename color_t> color_t yellow()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(v, v, sample_t(0));
  }

  template <typename color_t> color_t orange()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max;
    return color_t(v, sample_t(v / 2), sample_t(0));
  }

  inline ggo::rgb_8u white_8u()     { return ggo::white<ggo::rgb_8u>(); }
  inline ggo::rgb_8u gray_8u()      { return ggo::gray<ggo::rgb_8u>(); }
  inline ggo::rgb_8u black_8u()     { return ggo::black<ggo::rgb_8u>(); }
  inline ggo::rgb_8u red_8u()       { return ggo::red<ggo::rgb_8u>(); }
  inline ggo::rgb_8u green_8u()     { return ggo::green<ggo::rgb_8u>(); }
  inline ggo::rgb_8u blue_8u()      { return ggo::blue<ggo::rgb_8u>(); }
  inline ggo::rgb_8u cyan_8u()      { return ggo::cyan<ggo::rgb_8u>(); }
  inline ggo::rgb_8u magenta_8u()   { return ggo::magenta<ggo::rgb_8u>(); }
  inline ggo::rgb_8u yellow_8u()    { return ggo::yellow<ggo::rgb_8u>(); }
  inline ggo::rgb_8u orange_8u()    { return ggo::orange<ggo::rgb_8u>(); }

  inline ggo::rgb_32f white_32f()   { return ggo::white<ggo::rgb_32f>(); }
  inline ggo::rgb_32f gray_32f()    { return ggo::gray<ggo::rgb_32f>(); }
  inline ggo::rgb_32f black_32f()   { return ggo::black<ggo::rgb_32f>(); }
  inline ggo::rgb_32f red_32f()     { return ggo::red<ggo::rgb_32f>(); }
  inline ggo::rgb_32f green_32f()   { return ggo::green<ggo::rgb_32f>(); }
  inline ggo::rgb_32f blue_32f()    { return ggo::blue<ggo::rgb_32f>(); }
  inline ggo::rgb_32f cyan_32f()    { return ggo::cyan<ggo::rgb_32f>(); }
  inline ggo::rgb_32f magenta_32f() { return ggo::magenta<ggo::rgb_32f>(); }
  inline ggo::rgb_32f yellow_32f()  { return ggo::yellow<ggo::rgb_32f>(); }
  inline ggo::rgb_32f orange_32f()  { return ggo::orange<ggo::rgb_32f>(); }
}

/////////////////////////////////////////////////////////////////////
// Color accumulator.
namespace ggo
{
  template <typename color_t>
  struct accumulator
  {
  };

  template <>
  struct accumulator<uint8_t>
  {
    int _acc = 0;
    void add(uint8_t c) { _acc += c; }
    template <int count> uint8_t div() const { return (_acc + count / 2) / count; }
  };

  template <>
  struct accumulator<rgb_8u>
  {
    uint32_t _r = 0; uint32_t _g = 0; uint32_t _b = 0;
    void add(const rgb_8u & c) { _r += c._r; _g += c._g; _b += c._b; }
    template <uint32_t count> rgb_8u div() const {
      return rgb_8u(
        static_cast<uint8_t>((_r + count / 2) / count),
        static_cast<uint8_t>((_g + count / 2) / count),
        static_cast<uint8_t>((_b + count / 2) / count));
    }
  };

  template <>
  struct accumulator<rgba_8u>
  {
    uint32_t _r = 0; uint32_t _g = 0; uint32_t _b = 0; uint32_t _a = 0;
    void add(const rgba_8u & c) { _r += c._a * c._r; _g += c._a * c._g; _b += c._a * c._b; _a += c._a; }
    template <uint32_t count> rgba_8u div() const {
      if (_a == 0)
      {
        return { 0, 0, 0, 0 };
      }
      else
      {
        return {
          static_cast<uint8_t>(ggo::round_div(_r, _a)),
          static_cast<uint8_t>(ggo::round_div(_g, _a)),
          static_cast<uint8_t>(ggo::round_div(_b, _a)),
          static_cast<uint8_t>(ggo::round_div(_a, count)) };
      }
    }
  };

  template <>
  struct accumulator<float>
  {
    float _acc = 0.f;
    void add(float c) { _acc += c; }
    template <int count> float div() const { return _acc / count; }
  };

  template <>
  struct accumulator<rgb_32f>
  {
    float _r = 0.f; float _g = 0.f; float _b = 0.f;
    void add(const rgb_32f & c) { _r += c._r; _g += c._g; _b += c._b; }
    template <int count> rgb_32f div() const { return rgb_32f(_r / count, _g / count, _b / count); }
  };
}

/////////////////////////////////////////////////////////////////////
// Fixed point division for color.
namespace ggo
{
  template <int bit_shift, typename data_t>
  ggo::rgb<data_t> fixed_point_div(const ggo::rgb<data_t> & c)
  {
    using sample_t = typename ggo::color_traits<color<data_t>>::sample_t;

    static_assert(bit_shift > 1, "invalid bit shift");
    static_assert(std::is_integral<sample_t>::value && std::is_unsigned<sample_t>::value, "expected unsigned integral sample type");

    return ggo::color<data_t>(
      ggo::fixed_point_div<bit_shift>(c._r),
      ggo::fixed_point_div<bit_shift>(c._g),
      ggo::fixed_point_div<bit_shift>(c._b));
  }
}

/////////////////////////////////////////////////////////////////////
// Linear interpolation.
namespace ggo
{
  template <typename fract_t>
  uint8_t linerp(uint8_t color_a, uint8_t color_b, const fract_t & weight_a)
  {
    constexpr uint32_t one = 1 << fract_t::_log2_den;

    const uint32_t weight_b = one - weight_a._num;
    
    return ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a + weight_b * color_b);
  }

  template <typename fract_t>
  rgb_8u linerp(rgb_8u color_a, rgb_8u color_b, const fract_t & weight_a)
  {
    constexpr uint32_t one = 1 << fract_t::_log2_den;

    const uint32_t weight_b = one - weight_a._num;

    return {
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a._r + weight_b * color_b._r)),
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a._g + weight_b * color_b._g)),
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a._b + weight_b * color_b._b)) };
  }

  template <typename fract_t>
  rgba_8u linerp(rgba_8u color_a, rgba_8u color_b, const fract_t & weight_a)
  {
    uint32_t w_a = weight_a * color_a._a;
    uint32_t w_b = ggo::round_div<uint32_t>((0xff - w_a) * color_b._a, 0xff);

    uint32_t a = w_a + w_b;
    if (a == 0)
    {
      return { 0, 0, 0, 0 };
    }

    uint32_t r = ggo::round_div(w_a * color_a._r + w_b * color_b._r, a);
    uint32_t g = ggo::round_div(w_a * color_a._g + w_b * color_b._g, a);
    uint32_t b = ggo::round_div(w_a * color_a._b + w_b * color_b._b, a);

    return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a) };
  }

  template <typename fract_t>
  float linerp(float color_a, float color_b, const fract_t & weight_a)
  {
    const float weight_a_32f = weight_a.to<float>();
    const float weight_b_32f = 1.f - weight_a_32f;

    return weight_a_32f * color_a + weight_b_32f * color_b;
  }

  template <typename fract_t>
  rgb_32f linerp(const rgb_32f & color_a, const rgb_32f & color_b, const fract_t & weight_a)
  {
    const float weight_a_32f = weight_a.to<float>();
    const float weight_b_32f = 1.f - weight_a_32f;

    return {
      weight_a_32f * color_a._r + weight_b_32f * color_b._r,
      weight_a_32f * color_a._g + weight_b_32f * color_b._g,
      weight_a_32f * color_a._b + weight_b_32f * color_b._b };
  }
}

/////////////////////////////////////////////////////////////////////
// HSV
namespace ggo
{
  // Reference: http://en.wikipedia.org/wiki/HSL_color_space
  template <typename data_t>
  void hsv2rgb(data_t h, data_t s, data_t v, data_t & r, data_t & g, data_t & b)
  {
    static_assert(std::is_floating_point<data_t>::value);

    h = std::fmod(h, data_t(1));
    if (h < 0)
    {
      h += 1;
    }
    s = ggo::clamp<data_t>(s, 0, 1);
    v = ggo::clamp<data_t>(v, 0, 1);

    int		  h_i = ggo::clamp(int(h * 6), 0, 5);
    data_t	f = h * 6 - std::floor(h * 6);
    data_t	p = v * (1 - s);
    data_t	q = v * (1 - f * s);
    data_t	t = v * (1 - (1 - f) * s);

    switch (h_i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  template <typename color_t>
  color_t from_hsv(
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t hue,
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t sat,
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t val)
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using sample_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<decltype(hue)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(sat)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(val)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<sample_t>::value, "expecting floating point type");

    sample_t r, g, b;
    ggo::hsv2rgb(hue, sat, val, r, g, b);
    floating_point_color_t c(r, g, b);

    return convert_color_to<color_t>(c);
  }
}

/////////////////////////////////////////////////////////////////////
// Strings
namespace ggo
{
  inline std::string to_hex(const ggo::rgb_8u & c)
  {
    auto convert = [](uint8_t v)
    {
      switch (v)
      {
      case 0x00: return '0';
      case 0x01: return '1';
      case 0x02: return '2';
      case 0x03: return '3';
      case 0x04: return '4';
      case 0x05: return '5';
      case 0x06: return '6';
      case 0x07: return '7';
      case 0x08: return '8';
      case 0x09: return '9';
      case 0x0A: return 'A';
      case 0x0B: return 'B';
      case 0x0C: return 'C';
      case 0x0D: return 'D';
      case 0x0E: return 'E';
      case 0x0F: return 'F';
      default: GGO_FAIL(); return '\0';
      }
    };

    std::ostringstream oss;
    oss << convert((c._r & 0xF0) >> 4) << convert(c._r & 0x0F);
    oss << convert((c._g & 0xF0) >> 4) << convert(c._g & 0x0F);
    oss << convert((c._b & 0xF0) >> 4) << convert(c._b & 0x0F);
    return oss.str();
  }

  inline ggo::rgb_8u from_hex(const std::string & s)
  {
    if (s.size() != 6)
    {
      throw std::runtime_error(std::string("cannot convert string '") + s + "' to color: expecting 6 characeters");
    }

    auto convert = [](char c)
    {
      switch (c)
      {
      case '0': return 0x00;
      case '1': return 0x01;
      case '2': return 0x02;
      case '3': return 0x03;
      case '4': return 0x04;
      case '5': return 0x05;
      case '6': return 0x06;
      case '7': return 0x07;
      case '8': return 0x08;
      case '9': return 0x09;
      case 'A': return 0x0A;
      case 'B': return 0x0B;
      case 'C': return 0x0C;
      case 'D': return 0x0D;
      case 'E': return 0x0E;
      case 'F': return 0x0F ;
      default: throw std::runtime_error(std::string("invalid character in string '") + c + "'");
      }
    };

    unsigned char r = (convert(s[0]) << 4) + convert(s[1]);
    unsigned char g = (convert(s[2]) << 4) + convert(s[3]);
    unsigned char b = (convert(s[4]) << 4) + convert(s[5]);

    return { r, g, b };
  }
}

#endif
