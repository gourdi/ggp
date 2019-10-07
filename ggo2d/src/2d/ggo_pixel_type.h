#ifndef __GGO_PIXEL_TYPE__
#define __GGO_PIXEL_TYPE__

#include <stdint.h>
#include <2d/ggo_color.h>

namespace ggo
{
  enum class pixel_type
  {
    y_8u,
    y_16u,
    y_32f,
    y_64f,
    ya_8u,
    rgb_8u,
    rgba_8u,
    bgr_8u,
    bgrx_8u,
    bgra_8u,
    rgb_16u,
    rgb_32f
  };

  inline std::ostream & operator<<(std::ostream & os, pixel_type pt)
  {
    switch (pt)
    {
    case pixel_type::y_8u:
      os << "pixel_type::y_8u"; break;
    case pixel_type::y_16u:
      os << "pixel_type::y_16u"; break;
    case pixel_type::y_32f:
      os << "pixel_type::y_32f"; break;
    case pixel_type::y_64f:
      os << "pixel_type::y_64f"; break;
    case pixel_type::ya_8u:
      os << "pixel_type::ya_8u"; break;
    case pixel_type::rgb_8u:
      os << "pixel_type::rgb_8u"; break;
    case pixel_type::rgba_8u:
      os << "pixel_type::rgba_8u"; break;
    case pixel_type::bgr_8u:
      os << "pixel_type::bgr_8u"; break;
    case pixel_type::bgrx_8u:
      os << "pixel_type::bgrx_8u"; break;
    case pixel_type::bgra_8u:
      os << "pixel_type::bgra_8u"; break;
    case pixel_type::rgb_16u:
      os << "pixel_type::rgb_16u"; break;
    case pixel_type::rgb_32f:
      os << "pixel_type::rgb_32f"; break;
    }
    return os;
  }

  template <pixel_type pt>
  struct pixel_type_traits {};

  template <>
  struct pixel_type_traits<pixel_type::y_8u>
  {
    static constexpr int pixel_byte_size = 1;

    using color_t = uint8_t;

    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct pixel_type_traits<pixel_type::y_16u>
  {
    static constexpr int pixel_byte_size = 2;

    using color_t = uint16_t;

    static uint16_t read(const void * ptr) { return *static_cast<const uint16_t *>(ptr); }
    static void write(void * ptr, uint16_t c) { *static_cast<uint16_t *>(ptr) = c; }
  };

  template <>
  struct pixel_type_traits<pixel_type::y_32f>
  {
    static_assert(sizeof(float) == 4);

    static constexpr int pixel_byte_size = 4;

    using color_t = float;

    static float read(const void * ptr) { return *static_cast<const float *>(ptr); }
    static void write(void * ptr, float c) { *static_cast<float *>(ptr) = c; }
  };

  template <>
  struct pixel_type_traits<pixel_type::y_64f>
  {
    static_assert(sizeof(double) == 8);

    static constexpr int pixel_byte_size = 8;

    using color_t = double;

    static double read(const void * ptr) { return *static_cast<const double *>(ptr); }
    static void write(void * ptr, double c) { *static_cast<double *>(ptr) = c; }
  };

  template <>
  struct pixel_type_traits<pixel_type::ya_8u>
  {
    static constexpr int pixel_byte_size = 2;

    using color_t = ya_8u;

    static ggo::ya_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::ya_8u(ptr_8u[0], ptr_8u[1]);
    }

    static void write(void * ptr, ggo::ya_8u c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.y();
      ptr_8u[1] = c.a();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::rgb_8u>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr pixel_type gray_format = pixel_type::y_8u;

    using color_t = ggo::rgb_8u;

    static ggo::rgb_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::rgb_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2]);
    }

    static void write(void * ptr, const ggo::rgb_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.r();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.b();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::rgba_8u>
  {
    static constexpr int pixel_byte_size = 4;
    static constexpr pixel_type gray_format = pixel_type::y_8u;

    using color_t = ggo::rgba_8u;

    static ggo::rgba_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::rgba_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2], ptr_8u[3]);
    }

    static void write(void * ptr, const ggo::rgba_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.r();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.b();
      ptr_8u[3] = c.a();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::rgb_32f>
  {
    static_assert(sizeof(float) == 4);

    static constexpr int pixel_byte_size = 12;

    using color_t = ggo::rgb_32f;

    static ggo::rgb_32f read(const void * ptr)
    {
      const float * ptr_32f = static_cast<const float *>(ptr);
      return ggo::rgb_32f(ptr_32f[0], ptr_32f[1], ptr_32f[2]);
    }

    static void write(void * ptr, const ggo::rgb_32f & c)
    {
      float * ptr_32f = static_cast<float *>(ptr);
      ptr_32f[0] = c.r();
      ptr_32f[1] = c.g();
      ptr_32f[2] = c.b();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::bgr_8u>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr pixel_type gray_format = pixel_type::y_8u;

    using color_t = ggo::rgb_8u;

    static ggo::rgb_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::rgb_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0]);
    }

    static void write(void * ptr, const ggo::rgb_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.b();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.r();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::bgrx_8u>
  {
    static const int pixel_byte_size = 4;
    static const pixel_type gray_format = pixel_type::y_8u;

    using color_t = ggo::rgb_8u;

    static ggo::rgb_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::rgb_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0]);
    }

    static void write(void * ptr, const ggo::rgb_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.b();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.r();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::bgra_8u>
  {
    static constexpr int pixel_byte_size = 4;
    static const pixel_type gray_format = pixel_type::y_8u;

    using color_t = ggo::rgba_8u;

    static ggo::rgba_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::rgba_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0], ptr_8u[3]);
    }

    static void write(void * ptr, const ggo::rgba_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.b();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.r();
      ptr_8u[3] = c.a();
    }
  };

  template <>
  struct pixel_type_traits<pixel_type::rgb_16u>
  {
    static const int pixel_byte_size = 6;
    static const pixel_type gray_format = pixel_type::y_16u;

    using color_t = ggo::rgb_16u;

    static ggo::rgb_16u read(const void * ptr)
    {
      const uint16_t * ptr_16u = static_cast<const uint16_t *>(ptr);
      return ggo::rgb_16u(ptr_16u[0], ptr_16u[1], ptr_16u[2]);
    }

    static void write(void * ptr, const ggo::rgb_16u & c)
    {
      uint16_t * ptr_16u = static_cast<uint16_t *>(ptr);
      ptr_16u[0] = c.r();
      ptr_16u[1] = c.g();
      ptr_16u[2] = c.b();
    }
  };
}

// Dispatch
namespace ggo
{
  template <typename functor, typename... args>
  auto dispatch_pixel_type(pixel_type pt, args&&... a)
  {
    switch (pt)
    {
    default: throw std::runtime_error("invalid image format"); break;
    case pixel_type::y_8u: return functor::call<pixel_type::y_8u>(std::forward<args>(a)...);
    case pixel_type::y_16u: return functor::call<pixel_type::y_16u>(std::forward<args>(a)...);
    case pixel_type::y_32f: return functor::call<pixel_type::y_32f>(std::forward<args>(a)...);
    case pixel_type::ya_8u: return functor::call<pixel_type::ya_8u>(std::forward<args>(a)...);
    case pixel_type::rgb_8u: return functor::call<pixel_type::rgb_8u>(std::forward<args>(a)...);
    case pixel_type::bgr_8u: return functor::call<pixel_type::bgr_8u>(std::forward<args>(a)...);
    case pixel_type::bgrx_8u: return functor::call<pixel_type::bgrx_8u>(std::forward<args>(a)...);
    case pixel_type::rgba_8u: return functor::call<pixel_type::rgba_8u>(std::forward<args>(a)...);
    case pixel_type::rgb_16u: return functor::call<pixel_type::rgb_16u>(std::forward<args>(a)...);
    case pixel_type::rgb_32f: return functor::call<pixel_type::rgb_32f>(std::forward<args>(a)...);
    }
  }
}

// Pixel byte size.
namespace ggo
{
  struct get_pixel_byte_size_t
  {
    template <pixel_type pt>
    static constexpr int call()
    {
      return pixel_type_traits<pt>::pixel_byte_size;
    }
  };

  inline int get_pixel_byte_size(pixel_type pt)
  {
    return dispatch_pixel_type<get_pixel_byte_size_t>(pt);
  }

  inline int get_line_byte_size(pixel_type pt, int width)
  {
    return get_pixel_byte_size(pt) * width;
  }
}

// Static iterators.
namespace ggo
{
  template <typename subtype_t, pixel_type pt, typename void_ptr_t> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct pixel_type_iterator_t
  {
    pixel_type_iterator_t(void_ptr_t ptr) : _ptr(ptr) {}

    void_ptr_t _ptr;

    static constexpr std::ptrdiff_t byte_offset = pixel_type_traits<pt>::pixel_byte_size;

    void operator++() { _ptr = ggo::move_ptr(_ptr,  byte_offset); }
    void operator--() { _ptr = ggo::move_ptr(_ptr, -byte_offset); }
    bool operator==(const pixel_type_iterator_t & it) { return _ptr == it._ptr; }
    bool operator!=(const pixel_type_iterator_t & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr,  byte_offset * delta)); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr, -byte_offset * delta)); }
  };

  template <typename subtype_t, pixel_type pt, typename void_ptr_t>
  auto distance(const pixel_type_iterator_t<subtype_t, pt, void_ptr_t> & it1, const pixel_type_iterator_t<subtype_t, pt, void_ptr_t> & it2)
  {
    return (static_cast<const char *>(it2._ptr) - static_cast<const char *>(it1._ptr)) / pixel_type_traits<pt>::pixel_byte_size;
  }

  template <pixel_type pt>
  struct input_pixel_type_iterator_t final : public ggo::pixel_type_iterator_t<input_pixel_type_iterator_t<pt>, pt, const void *>
  {
    using data_t = typename pixel_type_traits<pt>::color_t;

    input_pixel_type_iterator_t(const void * ptr) : pixel_type_iterator_t(ptr) {}

    data_t get(std::ptrdiff_t delta) const { return pixel_type_traits<pt>::read(ggo::move_ptr(_ptr, byte_offset * delta)); }
  };

  template <pixel_type pt>
  struct output_pixel_type_iterator_t final : public ggo::pixel_type_iterator_t<output_pixel_type_iterator_t<pt>, pt, void *>
  {
    using data_t = typename pixel_type_traits<pt>::color_t;

    output_pixel_type_iterator_t(void * ptr) : pixel_type_iterator_t(ptr) {}

    void set(const data_t & color) { pixel_type_traits<pt>::write(_ptr, color); }
  };
}

// Dynamic iterators.
namespace ggo
{
  template <typename subtype_t, typename void_ptr_t> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct pixel_type_iterator
  {
    pixel_type_iterator(void_ptr_t ptr, std::ptrdiff_t byte_offset) : _ptr(ptr), _byte_offset(byte_offset) {}

    void_ptr_t _ptr;
    const std::ptrdiff_t _byte_offset;

    void operator++() { _ptr = ggo::move_ptr(_ptr,  _byte_offset); }
    void operator--() { _ptr = ggo::move_ptr(_ptr, -_byte_offset); }
    bool operator==(const pixel_type_iterator & it) { return _ptr == it._ptr; }
    bool operator!=(const pixel_type_iterator & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr,  _byte_offset * delta), _byte_offset); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr, -_byte_offset * delta), _byte_offset); }
  };

  template <typename subtype_t, typename void_ptr_t>
  auto distance(const pixel_type_iterator<subtype_t, void_ptr_t> & it1, const pixel_type_iterator<subtype_t, void_ptr_t> & it2)
  {
    GGO_ASSERT_EQ(it1._byte_offset, it1._byte_offset);
    return (static_cast<const char *>(it2._ptr) - static_cast<const char *>(it1._ptr)) / it1._byte_offset;
  }

  template <pixel_type pt>
  struct input_pixel_type_iterator final : public ggo::pixel_type_iterator<input_pixel_type_iterator<pt>, const void *>
  {
    using data_t = typename pixel_type_traits<pt>::color_t;

    input_pixel_type_iterator(const void * ptr, std::ptrdiff_t byte_offset) : pixel_type_iterator(ptr, byte_offset) {}

    data_t get(std::ptrdiff_t delta) const { return pixel_type_traits<pt>::read(ggo::move_ptr(_ptr, _byte_offset * delta)); }
  };

  template <pixel_type pt>
  struct output_pixel_type_iterator final : public ggo::pixel_type_iterator<output_pixel_type_iterator<pt>, void *>
  {
    using data_t = typename pixel_type_traits<pt>::color_t;

    output_pixel_type_iterator(void * ptr, std::ptrdiff_t byte_offset) : pixel_type_iterator(ptr, byte_offset) {}

    void set(const data_t & color) { pixel_type_traits<pt>::write(_ptr, color); }
  };
}

#endif

