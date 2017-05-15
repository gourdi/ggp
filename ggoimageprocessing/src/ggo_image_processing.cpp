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

/////////////////////////////////////////////////////////////////////
ggo::rect_data<float> ggo::get_view_pixel_rect(const ggo::basis2d<float> & view_basis, const ggo::pos2i & pixel)
{
  if (std::abs(ggo::dot(view_basis.x(), view_basis.y())) > 0.0001f)
  {
    throw std::runtime_error("basis is not orthogonal");
  }

  ggo::pos2f center = view_basis.pos() + float(pixel.x()) * view_basis.x() + float(pixel.y()) * view_basis.y();
  float width = view_basis.x().get_length();
  float height = view_basis.y().get_length();
  float left = center.x() - width / 2.f;
  float bottom = center.y() - height / 2.f;
  return ggo::rect_data_from_left_right_bottom_top(left, left + width, bottom, bottom + height);
}

/////////////////////////////////////////////////////////////////////
ggo::basis2d<float> ggo::compute_fit_view_basis(int image_width, int image_height, int view_width, int view_height)
{
  if (view_width * image_height > image_width * view_height)
  {
    // View is proportionaly larger than the image:
    // **VVVV**
    // **VVVV**
    // **VVVV**
    const float size = float(image_height) / float(view_height);

    float offset = (image_width - size * view_width) / 2.f;
    GGO_ASSERT(offset <= 0.f);

    float left = -0.5f + offset;
    float bottom = -0.5f;

    ggo::pos2f p{ left + size / 2.0f, bottom + size / 2.0f };
    ggo::pos2f x{ size, 0.0f };
    ggo::pos2f y{ 0.0f, size };

    return ggo::basis2d<float>(p, x, y);
  }
  else
  {
    // View is proportionaly larger than the image:
    // ****
    // VVVV
    // VVVV
    // VVVV
    // ****
    const float size = float(image_width) / float(view_width);

    float offset = (image_height - size * view_height) / 2.f;
    GGO_ASSERT(offset <= 0.f);

    float left = -0.5f;
    float bottom = -0.5f + offset;

    ggo::pos2f p{ left + size / 2.0f, bottom + size / 2.0f };
    ggo::pos2f x{ size, 0.0f };
    ggo::pos2f y{ 0.0f, size };

    return ggo::basis2d<float>(p, x, y);
  }
}

