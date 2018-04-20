#ifndef __GGO_IMAGE_FORMAT__
#define __GGO_IMAGE_FORMAT__

#include <stdint.h>
#include <kernel/memory/ggo_memory_layouts.h>
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
    static const int pixel_byte_size = 1;

    using color_t = uint8_t;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 1>;

    // Accessor interface.
    using type = uint8_t;
    static uint8_t read(const void * ptr) { return *static_cast<const uint8_t *>(ptr); }
    static void write(void * ptr, uint8_t c) { *static_cast<uint8_t *>(ptr) = c; }
  };

  template <>
  struct image_format_traits<y_8u_yd>
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
  struct image_format_traits<y_16u_yd>
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
  struct image_format_traits<y_32f_yu>
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
  struct image_format_traits<y_64f_yd>
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
  struct image_format_traits<rgb_8u_yu>
  {
    static const int pixel_byte_size = 3;
    static const image_format gray_format = y_8u_yu;

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
  struct image_format_traits<rgb_8u_yd>
  {
    static const int pixel_byte_size = 3;
    static const image_format gray_format = y_8u_yd;

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
  struct image_format_traits<rgba_8u_yd>
  {
    static const int pixel_byte_size = 4;
    static const image_format gray_format = y_8u_yd;

    using color_t = ggo::alpha_color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 4>;

    // Accessor interface.
    using type = ggo::alpha_color_8u;

    static ggo::alpha_color_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::alpha_color_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2], ptr_8u[3]);
    }

    static void write(void * ptr, const ggo::alpha_color_8u & c)
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
  struct image_format_traits<bgr_8u_yd>
  {
    static const int pixel_byte_size = 3;
    static const image_format gray_format = y_8u_yd;

    using color_t = ggo::color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 3>;

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
  struct image_format_traits<bgr_8u_yu>
  {
    static const int pixel_byte_size = 3;
    static const image_format gray_format = y_8u_yu;

    using color_t = ggo::color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::up, 3>;

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
  struct image_format_traits<bgrx_8u_yd>
  {
    static const int pixel_byte_size = 4;
    static const image_format gray_format = y_8u_yd;

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
  struct image_format_traits<bgra_8u_yd>
  {
    static const int pixel_byte_size = 4;
    static const image_format gray_format = y_8u_yd;

    using color_t = ggo::alpha_color_8u;
    using memory_layout_t = lines_memory_layout<ggo::direction::down, 4>;

    // Accessor interface.
    using type = ggo::alpha_color_8u;

    static ggo::alpha_color_8u read(const void * ptr)
    {
      const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
      return ggo::alpha_color_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0], ptr_8u[3]);
    }

    static void write(void * ptr, const ggo::alpha_color_8u & c)
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
  auto dispatch_image_format(image_format format, args&&... a)
  {
    switch (format)
    {
    default: GGO_FAIL(); // Don't break to fallback on the below image format.
    case y_8u_yu: return functor::call<y_8u_yu>(std::forward<args>(a)...);
    case y_8u_yd: return functor::call<y_8u_yd>(std::forward<args>(a)...);
    case y_16u_yd: return functor::call<y_16u_yd>(std::forward<args>(a)...);
    case y_32f_yu: return functor::call<y_32f_yu>(std::forward<args>(a)...);
    case rgb_8u_yu: return functor::call<rgb_8u_yu>(std::forward<args>(a)...);
    case rgb_8u_yd: return functor::call<rgb_8u_yd>(std::forward<args>(a)...);
    case bgr_8u_yu: return functor::call<bgr_8u_yu>(std::forward<args>(a)...);
    case bgr_8u_yd: return functor::call<bgr_8u_yd>(std::forward<args>(a)...);
    case bgrx_8u_yd: return functor::call<bgrx_8u_yd>(std::forward<args>(a)...);
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
  // Pointer to line.
  template <image_format format>
  void * get_line_ptr(void * ptr, const int y, const int height, const int line_step)
  {
    using memory_layout = image_format_traits<format>::memory_layout_t;

    return memory_layout::get_y_ptr(ptr, y, height, line_step);
  }

  template <image_format format>
  const void * get_line_ptr(const void * ptr, const int y, const int height, const int line_step)
  {
    using memory_layout = image_format_traits<format>::memory_layout_t;

    return memory_layout::get_y_ptr(ptr, y, height, line_step);
  }

  // Pointer to pixel.
  template <image_format format>
  void * get_pixel_ptr(void * ptr, const int x, const int y, const int height, const int line_step)
  {
    using memory_layout = image_format_traits<format>::memory_layout_t;

    return memory_layout::get_xy_ptr(ptr, x, y, height, line_step);
  }

  template <image_format format>
  const void * get_pixel_ptr(const void * ptr, const int x, const int y, const int height, const int line_step)
  {
    using memory_layout = image_format_traits<format>::memory_layout_t;

    return memory_layout::get_xy_ptr(ptr, x, y, height, line_step);
  }

  // Set pixel to pointer.
  template <image_format format>
  void write_pixel(void * ptr, const typename image_format_traits<format>::color_t & c)
  {
    image_format_traits<format>::write(ptr, c);
  }

  // Set pixel to pointer with coordinates.
  template <image_format format>
  void write_pixel(void * ptr, const int x, const int y, const int height, const int line_step, const typename image_format_traits<format>::color_t & c)
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
  typename image_format_traits<format>::color_t read_pixel(const void * ptr, const int x, const int y, const int height, const int line_step)
  {
    ptr = get_pixel_ptr<format>(ptr, x, y, height, line_step);

    return read_pixel<format>(ptr);
  }
}

#endif
