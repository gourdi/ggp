#ifndef __GGO_BLIT__
#define __GGO_BLIT__

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_blend.h>

namespace ggo
{
  template <typename input_image_t, typename output_image_t>
  void blit(const input_image_t & input_image, output_image_t & output_image, int left, int bottom);

  template <pixel_type input_pixel_type, lines_order input_lines_order, pixel_type output_pixel_type, lines_order output_lines_order>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom);

  template <pixel_type input_pixel_type, lines_order input_lines_order, pixel_type output_pixel_type, lines_order output_lines_order>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step);
}

namespace ggo
{
  template <typename input_image_t, typename output_image_t>
  void blit(const input_image_t & input_image, output_image_t & output_image, int left, int bottom)
  {
    using input_color_t = input_image_t::color_t;
    using output_color_t = output_image_t::color_t;
    using input_color_traits = color_traits<input_image_t::color_t>;
    using output_color_traits = color_traits<output_image_t::color_t>;

    // Create image views.
    auto views = create_intersection_views(input_image, output_image, left, bottom);
    if (!views)
    {
      return;
    }

    auto & input_view = views->first;
    auto & output_view = views->second;
    GGO_ASSERT_EQ(input_view.size(), output_view.size());

    // Copy pixels.
    for (int y = 0; y < input_view.height(); ++y)
    {
      for (int x = 0; x < input_view.width(); ++x)
      {
        if constexpr (input_color_traits::has_alpha == true)
        {
          output_view.write_pixel(x, y, alpha_blend<output_color_t>(output_view.read_pixel(x, y), input_view.read_pixel(x, y)));
        }
        else
        {
          output_view.write_pixel(x, y, convert_color_to<output_color_t>(input_view.read_pixel(x, y)));
        }
      }
    }
  }

  template <pixel_type input_pixel_type, lines_order input_lines_order, pixel_type output_pixel_type, lines_order output_lines_order>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step,
    int left, int bottom)
  {
    const_image_view_t<input_pixel_type, input_lines_order> input_image(input, { input_width, input_height }, input_line_step);
    image_view_t<output_pixel_type, output_lines_order> output_image(output, { output_width, output_height }, output_line_step);

    blit(input_image, output_image, left, bottom);
  }

  template <pixel_type input_pixel_type, lines_order input_lines_order, pixel_type output_pixel_type, lines_order output_lines_order>
  void blit(const void * input, int input_width, int input_height, int input_line_step,
    void * output, int output_width, int output_height, int output_line_step)
  {
    blit<input_pixel_type, input_lines_order, output_pixel_type, output_lines_order>(
      input, input_width, input_height, input_line_step,
      output, output_width, output_height, output_line_step,
      0, 0);
  }
}

#endif
