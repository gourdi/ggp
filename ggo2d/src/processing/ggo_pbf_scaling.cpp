#include "ggo_pbf_scaling.h"
#include <ggo_scale2d.h>

namespace ggo
{
  namespace
  {
    //////////////////////////////////////////////////////////////
    struct scaler
    {
      template <pixel_buffer_format pbf>
      static pixel_buffer call(const pixel_buffer & image, int output_width, int output_height)
      {
        using color_t = ggo::pixel_buffer_format_info<pbf>::color_t;
        using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

        pixel_buffer output(output_width, output_height, image.pbf());

        auto in = [&](int x, int y)
        {
          auto c = ggo::read_pixel<pbf>(image.data(), x, y, image.height(), image.line_byte_step());

          return ggo::convert_color_to<floating_point_color_t>(c);
        };

        auto out = [&](int x, int y, const floating_point_color_t & c)
        {
          ggo::write_pixel<pbf>(output.data(), x, y, output_height, output.line_byte_step(), ggo::convert_color_to<color_t>(c));
        };

        ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration, floating_point_color_t, float>(
          in, image.width(), image.height(), out, output_width, output_height);

        return output;
      }
    };
  }

  //////////////////////////////////////////////////////////////
  pixel_buffer ggo_scale_2d(const pixel_buffer & image, int output_width, int output_height)
  {
    if (output_width <= 0 || output_height <= 0)
    {
      throw std::runtime_error("negative or null scaling output dimension");
    }

    return ggo::dispatch_pbf<scaler>(image.pbf(), image, output_width, output_height);
  }
}


