#ifndef __GGO_2D_MEAN_BOX_BLUR__
#define __GGO_2D_MEAN_BOX_BLUR__

#include <kernel/math/signal_proc/ggo_convolution2d.h>
#include <kernel/math/signal_proc/ggo_mean_box_filter2d.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  template <typename color_t>
  struct mean_box_blur2d_helper {};

  template <>
  struct mean_box_blur2d_helper<uint8_t>
  {
    using color_t = uint16_t;

    static uint16_t divide(const uint16_t & pixel, int div)
    {
      return ggo::round_div(pixel, static_cast<uint16_t>(div));
    }

    static uint16_t convert(const uint8_t & pixel)
    {
      return static_cast<uint16_t>(pixel);
    }

    static uint8_t convert(const uint16_t & pixel)
    {
      return static_cast<uint8_t>(pixel);
    }
  };

  template <>
  struct mean_box_blur2d_helper<ggo::rgb_8u>
  {
    using color_t = ggo::rgb_16u;

    static ggo::rgb_16u divide(const rgb_16u c, const int div)
    {
      return ggo::rgb_16u(
        ggo::round_div(c._r, static_cast<uint16_t>(div)),
        ggo::round_div(c._g, static_cast<uint16_t>(div)),
        ggo::round_div(c._b, static_cast<uint16_t>(div)));
    }

    static ggo::rgb_16u convert(const ggo::rgb_8u & c)
    {
      return ggo::rgb_16u(
        static_cast<uint16_t>(c._r),
        static_cast<uint16_t>(c._g),
        static_cast<uint16_t>(c._b));
    }

    static ggo::rgb_8u convert(const ggo::rgb_16u & c)
    {
      return ggo::rgb_8u(
        static_cast<uint8_t>(c._r),
        static_cast<uint8_t>(c._g),
        static_cast<uint8_t>(c._b));
    }
  };

  ////////////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void mean_box_blur2d(void * buffer, int line_byte_step, const ggo::size & size, int box_radius)
  {
    using format_traits = image_format_traits<format>;
    using mean_box_helper = mean_box_blur2d_helper<typename format_traits::color_t>;

    constexpr int item_byte_step = format_traits::pixel_byte_size;

    const int div = 2 * box_radius + 1;

    auto read = [](const void * ptr) {
      return mean_box_helper::convert(format_traits::read(ptr));
    };

    auto write = [](void * ptr, const typename mean_box_helper::color_t & pixel) {
      format_traits::write(ptr, mean_box_helper::convert(pixel));
    };

    auto divide = [&](const typename mean_box_helper::color_t & pixel) {
      return mean_box_helper::divide(pixel, div);
    };

    ggo::mean_box_filter_2d_horz<border_mode::duplicate_edge>(buffer, item_byte_step, line_byte_step, read, 
      buffer, item_byte_step, line_byte_step, write,
      size, rect_int::from_size(size), divide, box_radius);

    ggo::mean_box_filter_2d_vert<border_mode::duplicate_edge>(buffer, item_byte_step, line_byte_step, read,
      buffer, item_byte_step, line_byte_step, write,
      size, rect_int::from_size(size), divide, box_radius);
  }
}

#endif
