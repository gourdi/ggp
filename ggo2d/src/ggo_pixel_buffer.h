#ifndef __GGO_PIXEL_BUFFER__
#define __GGO_PIXEL_BUFFER__

#include <stdint.h>
#include <ggo_buffer_access.h>
#include <ggo_color.h>

/////////////////////////////////////////////////////////////////////
// The sampling is rule is that the pixel (0, 0) represents 
// the square (-0.5, 0.5) x (-0.5, 0.5).

/////////////////////////////////////////////////////////////////////
// Coordinates are expressed the following way:
//
// ^ y 
// |
// |
// +------> x

// Pixel buffer formats.
namespace ggo
{
  enum pixel_buffer_format
  {
    y_8u_yu,
    y_32f_yu,
    rgb_8u_yu,
    rgb_32f_yu,
    bgra_8u_yd
  };

  template <pixel_buffer_format pbf>
  struct pixel_buffer_format_info {};

  template <>
  struct pixel_buffer_format_info<y_8u_yu>
  {
    static const y_direction y_dir = y_up;
    static const int pixel_byte_size = 1;
    static const int samples_count = 1;

    using color_t = uint8_t;
    using sample_t = uint8_t;
  };

  template <>
  struct pixel_buffer_format_info<y_32f_yu>
  {
    static_assert(sizeof(float) == 4 , "sizeof(float) must be 4");

    static const y_direction y_dir = y_up;
    static const int pixel_byte_size = 4;
    static const int samples_count = 1;

    using color_t = float;
    using sample_t = float;
  };

  template <>
  struct pixel_buffer_format_info<rgb_8u_yu>
  {
    static const y_direction y_dir = y_up;
    static const int pixel_byte_size = 3;
    static const int samples_count = 3;

    using color_t = ggo::color_8u;
    using sample_t = uint8_t;
  };

  template <>
  struct pixel_buffer_format_info<rgb_32f_yu>
  {
    static_assert(sizeof(float) == 4, "sizeof(float) must be 4");

    static const y_direction y_dir = y_up;
    static const int pixel_byte_size = 12;
    static const int samples_count = 3;

    using color_t = ggo::color_32f;
    using sample_t = float;
  };

  template <>
  struct pixel_buffer_format_info<bgra_8u_yd>
  {
    static const y_direction y_dir = y_down;
    static const int pixel_byte_size = 4;
    static const int samples_count = 3;
    static const int sample_byte_size = 1;

    using color_t = ggo::color_8u;
  };

  // Get line pointer.
  template <pixel_buffer_format pbf>
  void * get_line_ptr(void * buffer, const int y, const int height, const int line_step)
  {
    return ptr_offset(buffer, line_step * (pixel_buffer_format_info<pbf>::y_dir == y_down ? height - y - 1 : y));
  }
  
  template <pixel_buffer_format pbf>
  const void * get_line_ptr(const void * buffer, const int y, const int height, const int line_step)
  { 
    return ptr_offset(buffer, line_step * (pixel_buffer_format_info<pbf>::y_dir == y_down ? height - y - 1 : y));
  }

  // Get pixel pointer.
  template <pixel_buffer_format pbf>
  void * get_pixel_ptr(void * buffer, const int x, const int y, const int height, const int line_step)
  {
    return static_cast<uint8_t *>(get_line_ptr<pbf>(buffer, y, height, line_step)) + pixel_buffer_format_info<pbf>::pixel_byte_size * x;
  }
  
  template <pixel_buffer_format pbf>
  const void * get_pixel_ptr(const void * buffer, const int x, const int y, const int height, const int line_step)
  {
    return static_cast<const uint8_t *>(get_line_ptr<pbf>(buffer, y, height, line_step)) + pixel_buffer_format_info<pbf>::pixel_byte_size * x;
  }

  // Set pixel to pointer.
  template <pixel_buffer_format pbf, typename color_t> void set_pixel(void * ptr, const color_t & c) { static_assert(false, "missing specialization"); }

  template <> inline void set_pixel<y_8u_yu, uint8_t>(void * ptr, const uint8_t & c)
  {
    uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
    ptr_8u[0] = c;
  }

  template <> inline void set_pixel<y_32f_yu, float>(void * ptr, const float & c)
  {
    float * ptr_32f = static_cast<float *>(ptr);
    ptr_32f[0] = c;
  }
 
  template <> inline void set_pixel<rgb_8u_yu, ggo::color_8u>(void * ptr, const ggo::color_8u & c)
  {
    uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
    ptr_8u[0] = c._r;
    ptr_8u[1] = c._g;
    ptr_8u[2] = c._b;
  }

  template <> inline void set_pixel<rgb_32f_yu, ggo::color_32f>(void * ptr, const ggo::color_32f & c)
  {
    float * ptr_32f = static_cast<float *>(ptr);
    ptr_32f[0] = c._r;
    ptr_32f[1] = c._g;
    ptr_32f[2] = c._b;
  }

  template <> inline void set_pixel<bgra_8u_yd, ggo::color_8u>(void * ptr, const ggo::color_8u & c)
  {
    uint8_t * ptr_8u = static_cast<uint8_t *>(ptr);
    ptr_8u[0] = c._b;
    ptr_8u[1] = c._g;
    ptr_8u[2] = c._r;
  }

  // Set pixel to pointer with coordinates.
  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void set_pixel(void * ptr, const int x, const int y, const int height, const int line_step, const color_t & c)
  {
    set_pixel<pbf, color_t>(get_pixel_ptr<pbf>(ptr, x, y, height, line_step), c);
  }

  // Get pixel from pointer.
  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  color_t get_pixel(const void * ptr) { static_assert(false, "missing specialization"); }
  
  template<> inline uint8_t get_pixel<y_8u_yu, uint8_t>(const void * ptr)
  {
    const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
    return ptr_8u[0];
  }

  template<> inline float get_pixel<y_32f_yu, float>(const void * ptr)
  {
    const float * ptr_32f = static_cast<const float *>(ptr);
    return ptr_32f[0];
  }

  template<> inline ggo::color_8u get_pixel<rgb_8u_yu, ggo::color_8u>(const void * ptr)
  {
    const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
    return ggo::color_8u(ptr_8u[0], ptr_8u[1], ptr_8u[2]);
  }

  template<> inline ggo::color_32f get_pixel<rgb_32f_yu, ggo::color_32f>(const void * ptr)
  {
    const float * ptr_32f = static_cast<const float *>(ptr);
    return ggo::color_32f(ptr_32f[0], ptr_32f[1], ptr_32f[2]);
  }
  
  template<> inline ggo::color_8u get_pixel<pixel_buffer_format::bgra_8u_yd, ggo::color_8u>(const void * ptr)
  {
    const uint8_t * ptr_8u = static_cast<const uint8_t *>(ptr);
    return ggo::color_8u(ptr_8u[2], ptr_8u[1], ptr_8u[0]);
  }

  // Get pixel from pointer and coordinates.
  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  color_t get_pixel(const void * ptr, const int x, const int y, const int height, const int line_step)
  {
    return get_pixel<pbf, color_t>(get_pixel_ptr<pbf>(ptr, x, y, height, line_step));
  }

  // Block processing.
  template <pixel_buffer_format pbf, typename func_t>
  void process_buffer(void * buffer, const int width, const int height, const int line_step, func_t func)
  {
    GGO_ASSERT_PTR(buffer);
    GGO_ASSERT_GE(line_step, width * pixel_buffer_format_info<pbf>::pixel_byte_size);

    const int line_byte_size = width * pixel_buffer_format_info<pbf>::pixel_byte_size;
    uint8_t * ptr = static_cast<uint8_t *>(buffer);

    for (int y = 0; y < height; ++y)
    {
      const uint8_t * line_end = ptr + line_byte_size;
      for (uint8_t * line = ptr; line != line_end; line += pixel_buffer_format_info<pbf>::pixel_byte_size)
      {
        func(static_cast<void*>(line));
      }

      ptr += line_step;
    }
  }

  // Fast rectangle processing: no check on arguments.
  template <pixel_buffer_format pbf, typename func_t>
  void process_rect_fast(void * buffer, const int height, const int line_step, const int left, const int right, const int bottom, const int top, func_t func)
  {
    if (pixel_buffer_format_info<pbf>::y_dir == y_down)
    {
      void * ptr = get_pixel_ptr<pbf>(buffer, left, top, height, line_step);
      ggo::process_buffer<pbf>(ptr, right - left + 1, top - bottom + 1, line_step, func);
    }
    else
    {
      void * ptr = get_pixel_ptr<pbf>(buffer, left, bottom, height, line_step);
      ggo::process_buffer<pbf>(ptr, right - left + 1, top - bottom + 1, line_step, func);
    }
  }

  // Safe rectangle processing: rectangle is normalized and clamped.
  template <pixel_buffer_format pbf, typename func_t>
  void process_rect_safe(void * buffer, const int width, const int height, const int line_step, int left, int right, int bottom, int top, func_t func)
  {
    if (left > right) std::swap(left, right);
    if (bottom > top) std::swap(bottom, top);

    if (left >= width || right < 0 || bottom >= height || top < 0)
    {
      return;
    }

    left = std::max(left, 0);
    right = std::min(right, width - 1);
    bottom = std::max(bottom, 0);
    top = std::min(top, height - 1);

    process_rect_fast<pbf, func_t>(buffer, height, line_step, left, right, bottom, top, func);
  }
}


#endif

