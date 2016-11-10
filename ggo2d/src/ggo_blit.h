#ifndef __GGO_BLIT__
#define __GGO_BLIT__

#include <ggo_pixel_buffer.h>
#include <ggo_buffer_access.h>

namespace ggo
{
  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom);
}

namespace ggo
{
  template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom)
  {
    using format_in = pixel_buffer_format_info<pbf_in>;
    using format_out = pixel_buffer_format_info<pbf_out>;

    if (left >= output_height || bottom >= output_height)
    {
      return;
    }

    if (left < 0)
    {
      input = ggo::ptr_offset(input, -left * pixel_buffer_format_info<pbf_in>::pixel_byte_size);
      input_width -= -left;
      left = 0;
    }

    if (bottom < 0)
    {
      GGO_FAIL();
    }

    if (left + input_width > output_width)
    {
      GGO_FAIL();
    }

    if (bottom + input_height > output_height)
    {
      GGO_FAIL();
    }

    for (int y = 0; y < input_height; ++y)
    {
      const void * input_line_ptr = ggo::get_line_ptr<format_in::y_dir>(input, y, input_height, input_line_step);
      void * output_line_ptr = ggo::get_line_ptr<format_out::y_dir>(output, y, output_height, output_line_step);

      for (int x = 0; x < input_width; ++x)
      {
        auto color_in = ggo::read_pixel<pbf_in>(input_line_ptr);
        auto color_out = ggo::convert_color_to<ggo::pixel_buffer_format_info<pbf_out>::color_t>(color_in);

        ggo::write_pixel<pbf_out>(output_line_ptr, color_out);

        input_line_ptr = ggo::ptr_offset(input_line_ptr, ggo::pixel_buffer_format_info<pbf_in>::pixel_byte_size);
        output_line_ptr = ggo::ptr_offset(output_line_ptr, ggo::pixel_buffer_format_info<pbf_out>::pixel_byte_size);
      }
    }
  }
}

#endif
