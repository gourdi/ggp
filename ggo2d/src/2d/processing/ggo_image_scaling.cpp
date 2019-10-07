#include "ggo_image_scaling.h"
#include <kernel/math/signal_processing/ggo_scale2d.h>

namespace ggo
{
  namespace
  {
    //////////////////////////////////////////////////////////////
    struct scaler
    {
      template <lines_order memory_lines_order, pixel_type pixel_type>
      static image call(const image & input, ggo::size output_size)
      {
        using color_t = ggo::pixel_type_traits<pixel_type>::color_t;
        using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

        image output(output_size, pixel_type, memory_lines_order);

        auto in = [&](int x, int y)
        {
          auto c = ggo::pixel_type_traits<pixel_type>::read(input.pixel_ptr(x, y));

          return ggo::convert_color_to<floating_point_color_t>(c);
        };

        auto out = [&](int x, int y, const floating_point_color_t & c)
        {
          ggo::pixel_type_traits<pixel_type>::write(output.pixel_ptr(x, y), ggo::convert_color_to<color_t>(c));
        };

        ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration, floating_point_color_t, float>(
          in, input.size().width(), input.size().height(), out, output_size.width(), output_size.height());

        return output;
      }
    };
  }

  //////////////////////////////////////////////////////////////
  image scale(const image & input, ggo::size output_size)
  {
    if (output_si ze.width() <= 0 || output_size.height() <= 0)
    {
      throw std::runtime_error("negative or null scaling output dimension");
    }

    return dispatch_image_format<scaler>(input.pixel_type(), input.memory_lines_order(), input, output_size);
  }
}


