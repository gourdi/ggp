#include "ggo_scaling.h"
#include <kernel/math/shapes_2d/ggo_scale2d.h>

namespace ggo
{
  namespace
  {
    //////////////////////////////////////////////////////////////
    struct scaler
    {
      template <image_format format>
      static image call(const image & input, int output_width, int output_height)
      {
        using color_t = ggo::image_format_traits<format>::color_t;
        using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

        image output(output_width, output_height, input.format());

        auto in = [&](int x, int y)
        {
          auto c = ggo::read_pixel<format>(input.data(), x, y, input.height(), input.line_byte_step());

          return ggo::convert_color_to<floating_point_color_t>(c);
        };

        auto out = [&](int x, int y, const floating_point_color_t & c)
        {
          ggo::write_pixel<format>(output.data(), x, y, output_height, output.line_byte_step(), ggo::convert_color_to<color_t>(c));
        };

        ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration, floating_point_color_t, float>(
          in, input.width(), input.height(), out, output_width, output_height);

        return output;
      }
    };
  }

  //////////////////////////////////////////////////////////////
  image scale(const image & input, int output_width, int output_height)
  {
    if (output_width <= 0 || output_height <= 0)
    {
      throw std::runtime_error("negative or null scaling output dimension");
    }

    return ggo::dispatch_image_format<scaler>(input.format(), input, output_width, output_height);
  }
}


