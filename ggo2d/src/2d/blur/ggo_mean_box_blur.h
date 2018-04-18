#ifndef __GGO_2D_MEAN_BOX_BLUR__
#define __GGO_2D_MEAN_BOX_BLUR__

#include <kernel/memory/ggo_buffer_iterator.h>
#include <kernel/math/signal_proc/ggo_convolution.h>
#include <kernel/math/signal_proc/ggo_mean_box_filter.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  template <typename color_t>
  struct mean_box_blur2d_helper {};

  template <>
  struct mean_box_blur2d_helper<uint8_t>
  {
    using color_t = uint16_t;

    static uint16_t divide(const uint16_t c, const int div)
    {
      return ggo::round_div(c, static_cast<uint16_t>(div));
    }

    static uint16_t convert(const uint8_t & c)
    {
      return static_cast<uint16_t>(c);
    }

    static uint8_t convert(const uint16_t & c)
    {
      return static_cast<uint8_t>(c);
    }
  };

  template <>
  struct mean_box_blur2d_helper<ggo::color_8u>
  {
    using color_t = ggo::color_16u;

    static ggo::color_16u divide(const color_16u c, const int div)
    {
      return ggo::color_16u(
        ggo::round_div(c.r(), static_cast<uint16_t>(div)),
        ggo::round_div(c.g(), static_cast<uint16_t>(div)),
        ggo::round_div(c.b(), static_cast<uint16_t>(div)));
    }

    static ggo::color_16u convert(const ggo::color_8u & c)
    {
      return ggo::color_16u(
        static_cast<uint16_t>(c.r()),
        static_cast<uint16_t>(c.g()),
        static_cast<uint16_t>(c.b()));
    }

    static ggo::color_8u convert(const ggo::color_16u & c)
    {
      return ggo::color_8u(
        static_cast<uint8_t>(c.r()),
        static_cast<uint8_t>(c.g()),
        static_cast<uint8_t>(c.b()));
    }
  };

  template <ggo::image_format format>
  struct mean_box_accessor
  {
    using format_traits = image_format_traits<format>;
    using blur_helper = mean_box_blur2d_helper<typename format_traits::color_t>;
    using input_t = typename blur_helper::color_t;
    using output_t = typename blur_helper::color_t;

    static typename blur_helper::color_t read(const void * ptr)
    {
      return blur_helper::convert(format_traits::read(ptr));
    }

    static void write(void * ptr, typename blur_helper::color_t c)
    {
      return format_traits::write(ptr, blur_helper::convert(c));
    }
  };

  ////////////////////////////////////////////////////////////////////
  template <image_format format>
  void mean_box_blur2d(void * buffer, const int width, const int height, const int line_byte_step, const int box_half_size)
  {
    using format_traits = image_format_traits<format>;
    using memory_format = format_traits::memory_layout_t;
    using mean_box_helper = mean_box_blur2d_helper<typename format_traits::color_t>;
    using data_accessor = ggo::mean_box_accessor<format>;

    const int div = 2 * box_half_size + 1;

    auto divide = [&](const typename mean_box_helper::color_t & c) {
      return mean_box_helper::divide(c, div);
    };

    // First horizontal pass.
    {
      auto line_iterator = [&](int y) {
        return memory_format::make_horizontal_read_write_iterator<data_accessor, data_accessor>(buffer, y, height, line_byte_step);
      };

      auto left = [&](int x, int y) {
        return mean_box_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buffer, 0, y, height, line_byte_step)));
      };
      auto right = [&](int x, int y) {
        return mean_box_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buffer, width - 1, y, height, line_byte_step)));
      };

      ggo::mean_box_filter_2d_horz(line_iterator, left, right, width, height, divide, box_half_size);
    }

    // Second vertical pass.
    {
      auto column_iterator = [&](int x) {
        return memory_format::make_vertical_read_write_iterator<data_accessor, data_accessor>(buffer, x, height, line_byte_step);
      };

      auto bottom = [&](int x, int y) {
        return mean_box_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buffer, x, 0, height, line_byte_step)));
      };
      auto top = [&](int x, int y) {
        return mean_box_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buffer, x, height - 1, height, line_byte_step)));
      };

      ggo::mean_box_filter_2d_vert(column_iterator, bottom, top, width, height, divide, box_half_size);
    }
  }
}

#endif
