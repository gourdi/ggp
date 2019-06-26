#ifndef __GGO_IMAGE_FORMAT__
#define __GGO_IMAGE_FORMAT__

#include <stdint.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <2d/ggo_color.h>

// Image formats.
namespace ggo
{
  enum image_format
  {
    y_8u_yu,
    y_8u_yd,
    y_16u_yd,
    y_32f_yu,
    y_64f_yd,
    ya_8u_yd,
    rgb_8u_yu,
    rgb_8u_yd,
    rgba_8u_yd,
    bgr_8u_yd,
    bgr_8u_yu,
    bgrx_8u_yd,
    bgra_8u_yd,
    rgb_16u_yd,
    rgb_32f_yu
  };

  template <image_format format>
  struct image_format_traits {};

  template <>
  struct image_format_traits<y_8u_yu>
  {
    static constexpr int pixel_byte_size = 1;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::bottom_up;

    using color_t = uint8_t;

    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<y_8u_yd>
  {
    static constexpr int pixel_byte_size = 1;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

    using color_t = uint8_t;

    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<y_16u_yd>
  {
    static constexpr int pixel_byte_size = 2;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

    using color_t = uint16_t;

    static uint16_t read(const void * ptr) { return *static_cast<const uint16_t *>(ptr); }
    static void write(void * ptr, uint16_t c) { *static_cast<uint16_t *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<y_32f_yu>
  {
    static_assert(sizeof(float) == 4);

    static constexpr int pixel_byte_size = 4;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::bottom_up;

    using color_t = float;

    static float read(const void * ptr) { return *static_cast<const float *>(ptr); }
    static void write(void * ptr, float c) { *static_cast<float *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<y_64f_yd>
  {
    static_assert(sizeof(double) == 8);

    static constexpr int pixel_byte_size = 8;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

    using color_t = double;

    static double read(const void * ptr) { return *static_cast<const double *>(ptr); }
    static void write(void * ptr, double c) { *static_cast<double *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<ya_8u_yd>
  {
    static constexpr int pixel_byte_size = 2;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<rgb_8u_yu>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr image_format gray_format = y_8u_yu;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::bottom_up;

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
  struct image_format_traits<rgb_8u_yd>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr image_format gray_format = y_8u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<rgba_8u_yd>
  {
    static constexpr int pixel_byte_size = 4;
    static constexpr image_format gray_format = y_8u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<rgb_32f_yu>
  {
    static_assert(sizeof(float) == 4, "sizeof(float) must be 4");

    static constexpr int pixel_byte_size = 12;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::bottom_up;

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
  struct image_format_traits<bgr_8u_yd>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr image_format gray_format = y_8u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<bgr_8u_yu>
  {
    static constexpr int pixel_byte_size = 3;
    static constexpr image_format gray_format = y_8u_yu;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::bottom_up;

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
  struct image_format_traits<bgrx_8u_yd>
  {
    static const int pixel_byte_size = 4;
    static const image_format gray_format = y_8u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<bgra_8u_yd>
  {
    static constexpr int pixel_byte_size = 4;
    static constexpr image_format gray_format = y_8u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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
  struct image_format_traits<rgb_16u_yd>
  {
    static const int pixel_byte_size = 6;
    static const image_format gray_format = y_16u_yd;
    static constexpr ggo::memory_lines_order lines_order = ggo::memory_lines_order::top_down;

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

namespace ggo
{
  template <typename functor, typename... args>
  auto dispatch_image_format(image_format format, args&&... a)
  {
    switch (format)
    {
    default: throw std::runtime_error("invalid image format"); break;
    case y_8u_yu: return functor::call<y_8u_yu>(std::forward<args>(a)...);
    case y_8u_yd: return functor::call<y_8u_yd>(std::forward<args>(a)...);
    case y_16u_yd: return functor::call<y_16u_yd>(std::forward<args>(a)...);
    case y_32f_yu: return functor::call<y_32f_yu>(std::forward<args>(a)...);
    case ya_8u_yd: return functor::call<ya_8u_yd>(std::forward<args>(a)...);
    case rgb_8u_yu: return functor::call<rgb_8u_yu>(std::forward<args>(a)...);
    case rgb_8u_yd: return functor::call<rgb_8u_yd>(std::forward<args>(a)...);
    case bgr_8u_yu: return functor::call<bgr_8u_yu>(std::forward<args>(a)...);
    case bgr_8u_yd: return functor::call<bgr_8u_yd>(std::forward<args>(a)...);
    case bgrx_8u_yd: return functor::call<bgrx_8u_yd>(std::forward<args>(a)...);
    case rgba_8u_yd: return functor::call<rgba_8u_yd>(std::forward<args>(a)...);
    case rgb_16u_yd: return functor::call<rgb_16u_yd>(std::forward<args>(a)...);
    case rgb_32f_yu: return functor::call<rgb_32f_yu>(std::forward<args>(a)...);
    }
  }

  template <typename functor, image_format format1>
  struct dispatch_image_format_aux
  {
    template <ggo::image_format format2, typename... args>
    static auto call(args&&... a)
    {
      return functor::call<format1, format2>(std::forward<args>(a)...);
    }
  };

  template <typename functor, typename... args>
  auto dispatch_image_format(image_format format1, image_format format2, args&&... a)
  {
    switch (format1)
    {
    default: GGO_FAIL(); // Don't break to fallback on the below image format.
    case y_8u_yu: return dispatch_image_format<dispatch_image_format_aux<functor, y_8u_yu>>(format2, std::forward<args>(a)...);
    case y_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, y_8u_yd>>(format2, std::forward<args>(a)...);
    case y_16u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, y_16u_yd>>(format2, std::forward<args>(a)...);
    case y_32f_yu: return dispatch_image_format<dispatch_image_format_aux<functor, y_32f_yu>>(format2, std::forward<args>(a)...);
    case ya_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, ya_8u_yd>>(format2, std::forward<args>(a)...);
    case rgb_8u_yu: return dispatch_image_format<dispatch_image_format_aux<functor, rgb_8u_yu>>(format2, std::forward<args>(a)...);
    case rgb_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, rgb_8u_yd>>(format2, std::forward<args>(a)...);
    case rgba_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, rgba_8u_yd>>(format2, std::forward<args>(a)...);
    case bgr_8u_yu: return dispatch_image_format<dispatch_image_format_aux<functor, bgr_8u_yu>>(format2, std::forward<args>(a)...);
    case bgr_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, bgr_8u_yd>>(format2, std::forward<args>(a)...);
    case bgrx_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, bgrx_8u_yd>>(format2, std::forward<args>(a)...);
    case bgra_8u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, bgra_8u_yd>>(format2, std::forward<args>(a)...);
    case rgb_16u_yd: return dispatch_image_format<dispatch_image_format_aux<functor, rgb_16u_yd>>(format2, std::forward<args>(a)...);
    case rgb_32f_yu: return dispatch_image_format<dispatch_image_format_aux<functor, rgb_32f_yu>>(format2, std::forward<args>(a)...);
    }
  }
}

namespace ggo
{
  struct get_pixel_byte_size_t
  {
    template <image_format format>
    static int call()
    {
      return image_format_traits<format>::pixel_byte_size;
    }
  };

  inline int get_pixel_byte_size(image_format format)
  {
    return dispatch_image_format<get_pixel_byte_size_t>(format);
  }

  inline int get_line_byte_size(image_format format, int width)
  {
    return get_pixel_byte_size(format) * width;
  }
}

namespace ggo
{
  namespace details
  {
    template <typename ptr_t>
    struct line_ptr_functor
    {
      template <ggo::image_format format>
      static ptr_t call(ptr_t ptr, int y, int height, int line_byte_step)
      {
        using format_traits = ggo::image_format_traits<format>;

        return ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
      }
    };

    template <typename ptr_t>
    struct pixel_ptr_functor
    {
      template <ggo::image_format format>
      static ptr_t call(ptr_t ptr, int x, int y, int height, int line_byte_step)
      {
        using format_traits = ggo::image_format_traits<format>;

        ptr = ggo::get_line_ptr<format_traits::lines_order>(ptr, y, height, line_byte_step);
        ptr = ggo::move_ptr(ptr, x * format_traits::pixel_byte_size);

        return ptr;
      }
    };
  }

  // Pointer to line.
  template <typename ptr_t>
  ptr_t get_line_ptr(ptr_t ptr, int y, int height, int line_byte_step, image_format format)
  {
    return dispatch_image_format<details::line_ptr_functor<ptr_t>>(format, ptr, y, height, line_byte_step);
  }

  // Pointer to pixel.
  template <image_format format, typename ptr_t>
  ptr_t get_pixel_ptr(ptr_t ptr, int x, int y, int height, int line_step)
  {
    static_assert(std::is_pointer_v<ptr_t>);

    using format_traits = ggo::image_format_traits<format>;

    return move_ptr(get_line_ptr<format_traits::lines_order>(ptr, y, height, line_step), x * format_traits::pixel_byte_size);
  }

  template <typename ptr_t>
  ptr_t get_pixel_ptr(ptr_t ptr, int x, int y, int height, int line_step, image_format format)
  {
    return dispatch_image_format<details::pixel_ptr_functor<ptr_t>>(format, ptr, x, y,height, line_step);
  }

  // Set pixel to pointer.
  template <image_format format>
  void write_pixel(void * ptr, const typename image_format_traits<format>::color_t & c)
  {
    image_format_traits<format>::write(ptr, c);
  }

  // Set pixel to pointer with coordinates.
  template <image_format format>
  void write_pixel(void * ptr, int x, int y, int height, int line_step, const typename image_format_traits<format>::color_t & c)
  {
    ptr = get_pixel_ptr<format>(ptr, x, y, height, line_step);

    write_pixel<format>(ptr, c);
  }

  // Get pixel from pointer.
  template <image_format format>
  typename image_format_traits<format>::color_t read_pixel(const void * ptr)
  {
    return image_format_traits<format>::read(ptr);
  }

  // Get pixel from pointer and coordinates.
  template <image_format format>
  typename image_format_traits<format>::color_t read_pixel(const void * ptr, int x, int y, int height, int line_step)
  {
    ptr = get_pixel_ptr<format>(ptr, x, y, height, line_step);

    return read_pixel<format>(ptr);
  }
}

#endif
