#include "ggo_image_processing.h"
#include "ggo_load_image.h"
#include <ggo_coordinates_conversions.h>

/////////////////////////////////////////////////////////////////////
ggo::image_processor::image_processor(const char * filename)
:
_pixel_buffer(ggo::load_image(filename))
{
}

namespace
{
  struct render_t
  {
    template <ggo::pixel_buffer_format pbf_render, ggo::pixel_buffer_format pbf_input>
    static void call(
      void * render_buffer, int render_width, int render_height, int render_line_byte_step,
      const void * input_buffer, int input_width, int input_height, int input_line_byte_step,
      const ggo::basis2d<float> & view_basis)
    {
      using render_format = ggo::pixel_buffer_format_info<pbf_render>;

      const ggo::rect_int input_rect = ggo::rect_int::from_width_height(input_width, input_height);

      for (int y = 0; y < render_height; ++y)
      {
        float y_f = ggo::from_pixel_to_math<float>(y);
        void * ptr = ggo::get_line_ptr<pbf_render>(render_buffer, y, render_height, render_line_byte_step);

        for (int x = 0; x < render_width; ++x)
        {
          float x_f = ggo::from_pixel_to_math<float>(x);

          const ggo::pos2f image_pos = view_basis.point_from_world_to_local({ x_f, y_f });
          const ggo::pos2i image_pixel = ggo::from_math_to_pixel(image_pos);

          if (input_rect.is_inside(image_pixel) == true)
          {
            const auto input_color = ggo::read_pixel<pbf_input>(input_buffer, image_pixel.x(), image_pixel.y(), input_height, input_line_byte_step);
            const auto render_color = ggo::convert_color_to<render_format::color_t>(input_color);
            render_format::write(ptr, render_color);
          }
          else
          {
            render_format::write(ptr, render_format::color_t(0));
          }

          ptr = ggo::ptr_offset<render_format::pixel_byte_size>(ptr);
        }
      }
    }
  };
}

/////////////////////////////////////////////////////////////////////
void ggo::image_processor::render(void * buffer, int width, int height, int line_byte_step, ggo::pixel_buffer_format pbf, const basis2d<float> & view_basis) const
{
  templatize_pbf2<render_t>(
    pbf, _pixel_buffer.get_pixel_buffer_format(),
    buffer, width, height, line_byte_step,
    _pixel_buffer.data(), _pixel_buffer.get_width(), _pixel_buffer.get_height(), _pixel_buffer.get_line_byte_step(),
    view_basis);
}
