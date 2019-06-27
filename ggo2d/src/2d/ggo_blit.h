#ifndef __GGO_BLIT__
#define __GGO_BLIT__

#include <2d/ggo_image_format.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_blend.h>

namespace ggo
{
  template <image_format format_in, image_format format_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom);

  template <image_format format_in, image_format format_out, typename blend_func_t>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom, blend_func_t blend_func);
}

namespace ggo
{
  template <image_format format_in, image_format format_out>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom)
  {
    using color_t_in = image_format_traits<format_in>::color_t;
    using color_t_out = image_format_traits<format_out>::color_t;

    auto blend_func = [](const color_t_out & c_out, const color_t_in & c_in)
    {
      return ggo::alpha_blend(c_out, c_in);
    };

    blit<format_in, format_out>(
      input, input_width, input_height, input_line_step,
      output, output_width, output_height, output_line_step, left, bottom, blend_func);
  }

  template <image_format format_in, image_format format_out, typename blend_func_t>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom, blend_func_t blend_func)
  {
    using format_traits_in = image_format_traits<format_in>;
    using format_traits_out = image_format_traits<format_out>;

    // Create image views.
    ggo::const_image_wrapper_t<format_in> input_img(input, { input_width, input_height }, input_line_step);
    auto input_view = input_img.create_view(rect_int::from_left_width_bottom_height(-left, output_width, -bottom, output_height));
    if (!input_view)
    {
      return;
    }

    ggo::image_wrapper_t<format_out> output_img(output, { output_width, output_height }, output_line_step);
    auto output_view = output_img.create_view(ggo::rect_int::from_left_width_bottom_height(left, input_width, bottom, input_height));
    if (!output_view)
    {
      return;
    }

    GGO_ASSERT_EQ(input_view->size(), output_view->size());

    // Copy pixels.
    int w = input_view->size().width();
    int h = input_view->size().height();
    int end_offset = w * format_traits_in::pixel_byte_size;
    for (int y = 0; y < h; ++y)
    {
      const void * in = input_view->line_ptr(y);
      const void * end = move_ptr(in, end_offset);
      void * out = output_view->line_ptr(y);

      for (; in != end; in = move_ptr(in, format_traits_in::pixel_byte_size), out = move_ptr(out, format_traits_out::pixel_byte_size))
      {
        const auto color_in = ggo::read_pixel<format_in>(in);
        const auto color_out = ggo::read_pixel<format_out>(out);
        const auto color_blended = blend_func(color_out, color_in);

        ggo::write_pixel<format_out>(out, color_blended);
      }
    }
  }
}

#endif
