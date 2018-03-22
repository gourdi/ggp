#ifndef __GGO_PIXEL_BUFFER_FORMAT__
#define __GGO_PIXEL_BUFFER_FORMAT__

#include <stdint.h>
#include <ggo_memory_layouts.h>
#include <ggo_color.h>

/////////////////////////////////////////////////////////////////////
// The sampling is rule is that the pixel (0, 0) represents 
// the square (-0.5, 0.5) x (-0.5, 0.5).

// Pixel buffer formats.
namespace ggo
{
  enum pixel_buffer_format
  {
    y_8u_yu,
    y_8u_yd,
    y_16u_yd,
    y_32f_yu,
    y_64f_yd,
    rgb_8u_yu,
    rgb_8u_yd,
    bgra_8u_yd,
    rgb_16u_yd,
    rgb_32f_yu
  };

  template <pixel_buffer_format pbf>
  struct pixel_buffer_format_info {};

  template <>
  struct pixel_buffer_format_info<y_8u_yu>
  {
    static const int pixel_byte_size = 1;

    using color_t = uint8_t;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 1>;

    // Accessor interface.
    using type = uint8_t;
    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct pixel_buffer_format_info<y_8u_yd>
  {
    static const int pixel_byte_size = 1;

    using color_t = uint8_t;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 1>;

    // Accessor interface.
    using type = uint8_t;
    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct pixel_buffer_format_info<y_16u_yd>
  {
    static const int pixel_byte_size = 2;

    using color_t = uint16_t;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 2>;

    // Accessor interface.
    using type = uint16_t;
    static uint16_t read(const void * ptr) { return *static_cast<const uint16_t *>(ptr); }
    static void write(void * ptr, uint16_t c) { *static_cast<uint16_t *>(ptr) = c; }
  };

  template <>
  struct pixel_buffer_format_info<y_32f_yu>
  {
    static_assert(sizeof(float) == 4);

    static const int pixel_byte_size = 4;

    using color_t = float;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 4>;

    // Accessor interface.
    using type = float;
    static float read(const void * ptr) { return *static_cast<const float *>(ptr); }
    static void write(void * ptr, float c) { *static_cast<float *>(ptr) = c; }
  };

  template <>
  struct pixel_buffer_format_info<y_64f_yd>
  {
    static_assert(sizeof(double) == 8);

    static const int pixel_byte_size = 8;

    using color_t = double;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 8>;

    // Accessor interface.
    using type = double;
    static double read(const void * ptr) { return *static_cast<const double *>(ptr); }
    static void write(void * ptr, double c) { *static_cast<double *>(ptr) = c; }
  };

  template <>
  struct pixel_buffer_format_info<rgb_8u_yu>
  {
    static const int pixel_byte_size = 3;
    static const pixel_buffer_format gray_pbf = y_8u_yu;

    using color_t = ggo::color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 3>;

    // Accessor interface.
    using type = ggo::color_8u;

    static ggo::color_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::color_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2]);
    }

    static void write(void * ptr, const ggo::color_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.r();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.b();
    }
  };

  template <>
  struct pixel_buffer_format_info<rgb_8u_yd>
  {
    static const int pixel_byte_size = 3;
    static const pixel_buffer_format gray_pbf = y_8u_yd;

    using color_t = ggo::color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 3>;

    // Accessor interface.
    using type = ggo::color_8u;

    static ggo::color_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::color_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2]);
    }

    static void write(void * ptr, const ggo::color_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.r();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.b();
    }
  };

  template <>
  struct pixel_buffer_format_info<rgb_32f_yu>
  {
    static_assert(sizeof(float) == 4, "sizeof(float) must be 4");

    static const int pixel_byte_size = 12;

    using color_t = ggo::color_32f;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 12>;

    // Accessor interface.
    using type = ggo::color_32f;

    static ggo::color_32f read(const void * ptr)
    {
      const float * ptr_32f = static_cast<const float *>(ptr);
      return ggo::color_32f(ptr_32f[0], ptr_32f[1], ptr_32f[2]);
    }

    static void write(void * ptr, const ggo::color_32f & c)
    {
      float * ptr_32f = static_cast<float *>(ptr);
      ptr_32f[0] = c.r();
      ptr_32f[1] = c.g();
      ptr_32f[2] = c.b();
    }
  };

  template <>
  struct pixel_buffer_format_info<bgra_8u_yd>
  {
    static const int pixel_byte_size = 4;
    static const pixel_buffer_format gray_pbf = y_8u_yd;

    using color_t = ggo::color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 4>;

    // Accessor interface.
    using type = ggo::color_8u;

    static ggo::color_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::color_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0]);
    }

    static void write(void * ptr, const ggo::color_8u & c)
    {
      uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
      ptr_8u[0] = c.b();
      ptr_8u[1] = c.g();
      ptr_8u[2] = c.r();
    }
  };

  template <>
  struct pixel_buffer_format_info<rgb_16u_yd>
  {
    static const int pixel_byte_size = 6;
    static const pixel_buffer_format gray_pbf = y_16u_yd;

    using color_t = ggo::color_16u;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 6>;

    // Accessor interface.
    using type = ggo::color_16u;

    static ggo::color_16u read(const void * ptr)
    {
      const uint16_t * ptr_16u = static_cast<const uint16_t *>(ptr);
      return ggo::color_16u(ptr_16u[0], ptr_16u[1], ptr_16u[2]);
    }

    static void write(void * ptr, const ggo::color_16u & c)
    {
      uint16_t * ptr_16u = static_cast<uint16_t *>(ptr);
      ptr_16u[0] = c.r();
      ptr_16u[1] = c.g();
      ptr_16u[2] = c.b();
    }
  };
}

namespace ggo
{
  template <typename functor, typename... args>
  auto dispatch_pbf(pixel_buffer_format pbf, args&&... a)
  {
    switch (pbf)
    {
    default: GGO_FAIL(); // Don't break to fallback on the below pixel buffer format.
    case y_8u_yu: return functor::call<y_8u_yu>(std::forward<args>(a)...);
    case y_8u_yd: return functor::call<y_8u_yd>(std::forward<args>(a)...);
    case y_16u_yd: return functor::call<y_16u_yd>(std::forward<args>(a)...);
    case y_32f_yu: return functor::call<y_32f_yu>(std::forward<args>(a)...);
    case rgb_8u_yu: return functor::call<rgb_8u_yu>(std::forward<args>(a)...);
    case rgb_8u_yd: return functor::call<rgb_8u_yd>(std::forward<args>(a)...);
    case bgra_8u_yd: return functor::call<bgra_8u_yd>(std::forward<args>(a)...);
    case rgb_16u_yd: return functor::call<rgb_16u_yd>(std::forward<args>(a)...);
    case rgb_32f_yu: return functor::call<rgb_32f_yu>(std::forward<args>(a)...);
    }
  }
}

namespace ggo
{
  struct get_pixel_byte_size_t
  {
    template <pixel_buffer_format pbf>
    static int call()
    {
      return pixel_buffer_format_info<pbf>::pixel_byte_size;
    }
  };

  inline int get_pixel_byte_size(pixel_buffer_format pbf)
  {
    return dispatch_pbf<get_pixel_byte_size_t>(pbf);
  }

  inline int get_line_byte_size(pixel_buffer_format pbf, int width)
  {
    return get_pixel_byte_size(pbf) * width;
  }
}

namespace ggo
{
  // Pointer to line.
  template <pixel_buffer_format pbf>
  void * get_line_ptr(void * ptr, const int y, const int height, const int line_step)
  {
    using memory_layout = pixel_buffer_format_info<pbf>::memory_layout_t;

    return memory_layout::get_y_ptr(ptr, y, height, line_step);
  }

  template <pixel_buffer_format pbf>
  const void * get_line_ptr(const void * ptr, const int y, const int height, const int line_step)
  {
    using memory_layout = pixel_buffer_format_info<pbf>::memory_layout_t;

    return memory_layout::get_y_ptr(ptr, y, height, line_step);
  }

  // Pointer to pixel.
  template <pixel_buffer_format pbf>
  void * get_pixel_ptr(void * ptr, const int x, const int y, const int height, const int line_step)
  {
    using memory_layout = pixel_buffer_format_info<pbf>::memory_layout_t;

    return memory_layout::get_xy_ptr(ptr, x, y, height, line_step);
  }

  template <pixel_buffer_format pbf>
  const void * get_pixel_ptr(const void * ptr, const int x, const int y, const int height, const int line_step)
  {
    using memory_layout = pixel_buffer_format_info<pbf>::memory_layout_t;

    return memory_layout::get_xy_ptr(ptr, x, y, height, line_step);
  }

  // Set pixel to pointer.
  template <pixel_buffer_format pbf>
  void write_pixel(void * ptr, const typename pixel_buffer_format_info<pbf>::color_t & c)
  {
    pixel_buffer_format_info<pbf>::write(ptr, c);
  }

  // Set pixel to pointer with coordinates.
  template <pixel_buffer_format pbf>
  void write_pixel(void * ptr, const int x, const int y, const int height, const int line_step, const typename pixel_buffer_format_info<pbf>::color_t & c)
  {
    ptr = get_pixel_ptr<pbf>(ptr, x, y, height, line_step);

    write_pixel<pbf>(ptr, c);
  }

  // Get pixel from pointer.
  template <pixel_buffer_format pbf>
  typename pixel_buffer_format_info<pbf>::color_t read_pixel(const void * ptr)
  {
    return pixel_buffer_format_info<pbf>::read(ptr);
  }

  // Get pixel from pointer and coordinates.
  template <pixel_buffer_format pbf>
  typename pixel_buffer_format_info<pbf>::color_t read_pixel(const void * ptr, const int x, const int y, const int height, const int line_step)
  {
    ptr = get_pixel_ptr<pbf>(ptr, x, y, height, line_step);

    return read_pixel<pbf>(ptr);
  }
}

#endif
