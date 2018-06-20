#include "ggo_crop.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  struct crop_functor
  {
    template <image_format format>
    static ggo::image call(const ggo::image & input, const ggo::rect_int & crop)
    {
      using format_traits = image_format_traits<format>;

      int line_byte_size = format_traits::pixel_byte_size * crop.width();

      image output(crop.size(), format);

      for (int dst_y = 0; dst_y < crop.height(); ++dst_y)
      {
        int src_y = crop.bottom() + dst_y;
        const void * src_ptr = get_pixel_ptr<format>(input.data(), crop.left(), src_y, input.height(), input.line_byte_step());
        void * dst_ptr = get_line_ptr<format_traits::lines_order>(output.data(), dst_y, output.height(), output.line_byte_step());

        memcpy(dst_ptr, src_ptr, line_byte_size);
      }

      return output;
    }
  };

  //////////////////////////////////////////////////////////////
  ggo::image crop(const ggo::image & input, const ggo::rect_int & crop)
  {
    ggo::rect_int clipped_rect(crop);
    if (clipped_rect.clip(input.width(), input.height()) == false)
    {
      throw std::runtime_error("crop rectangle does not intersect image");
    }

    return dispatch_image_format<crop_functor>(input.format(), input, clipped_rect);
  }
}
