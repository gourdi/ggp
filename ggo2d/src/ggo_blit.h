#ifndef __GGO_BLIT__
#define __GGO_BLIT__

#include <ggo_pixel_buffer.h>

namespace ggo
{
  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom);

  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out, typename blend_func_t>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom, blend_func_t blend_func);
}

namespace ggo
{
  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom)
  {
    using color_t_in = pixel_buffer_format_info<pbf_in>::color_t;
    using color_t_out = pixel_buffer_format_info<pbf_out>::color_t;

    auto blend_func = [](const color_t_out & c_out, const color_t_in & c_in)
    {
      return ggo::convert_color_to<color_t_out>(c_in);
    };

    blit<pbf_in, pbf_out>(
      input, input_width, input_height, input_line_step,
      output, output_width, output_height, output_line_step, left, bottom, blend_func);
  }

  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out, typename blend_func_t>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom, blend_func_t blend_func)
  {
    using format_in = pixel_buffer_format_info<pbf_in>;
    using format_out = pixel_buffer_format_info<pbf_out>;
    using memory_layout_in = format_in::memory_layout_t;
    using memory_layout_out = format_in::memory_layout_t;

    // Blit is implemented for lines memory layout only.
    static_assert(memory_layout_in::lines_direction == ggo::direction::up || memory_layout_in::lines_direction == ggo::direction::down);
    static_assert(memory_layout_out::lines_direction == ggo::direction::up || memory_layout_out::lines_direction == ggo::direction::down);

    // Nothing to do.
    if (left >= output_width || left + input_width <= 0)
    {
      return;
    }

    if (bottom >= output_height || bottom + input_height <= 0)
    {
      return;
    }

    // Clip.
    if (bottom < 0)
    {
      if (memory_layout_in::lines_direction == ggo::direction::up)
      {
        input = ggo::ptr_offset(input, -bottom * input_line_step);
      }
      input_height = input_height + bottom;
      bottom = 0;
    }

    const int top = bottom + input_height - 1; // Inclusive, output coordinates.
    if (top >= output_height)
    {
      if (memory_layout_in::lines_direction == ggo::direction::down)
      {
        input = ggo::ptr_offset(input, (top - output_height + 1) * input_line_step);
      }
      input_height = output_height - bottom;
    }

    if (left < 0)
    {
      input = ggo::ptr_offset(input, -left * format_in::pixel_byte_size);
      input_width = input_width + left;
      left = 0;
    }

    const int right = left + input_width - 1; // Inclusive, output coordinates.
    if (right >= output_width)
    {
      input_width = output_width - left;
    }

    // Copy pixels.
    for (int y = 0; y < input_height; ++y)
    {
      const void * input_ptr = ggo::get_line_ptr<pbf_in>(input, y, input_height, input_line_step);
      const void * input_end = ggo::get_pixel_ptr<pbf_in>(input, input_width, y, input_height, input_line_step);
      void * output_ptr = ggo::get_pixel_ptr<pbf_out>(output, left, bottom + y, output_height, output_line_step);

      while (true)
      {
        const auto color_in = ggo::read_pixel<pbf_in>(input_ptr);
        const auto color_out = ggo::read_pixel<pbf_out>(output_ptr);
        const auto color_blended = blend_func(color_out, color_in);

        ggo::write_pixel<pbf_out>(output_ptr, color_blended);

        input_ptr = ggo::ptr_offset<format_in::pixel_byte_size>(input_ptr);

        if (input_ptr == input_end)
        {
          break;
        }

        output_ptr = ggo::ptr_offset<format_out::pixel_byte_size>(output_ptr);
      }
    }
  }
}

#endif
