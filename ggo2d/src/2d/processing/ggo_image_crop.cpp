#include "ggo_image_crop.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  struct crop_functor
  {
    template <lines_order memory_lines_order, pixel_type pixel_type>
    static ggo::image call(const ggo::image & input, const ggo::rect_int & crop)
    {
      constexpr int pixel_byte_size = pixel_type_traits<pixel_type>::pixel_byte_size;

      int line_byte_size = pixel_byte_size * crop.width();

      image output(crop.size(), pixel_type, memory_lines_order);

      for (int dst_y = 0; dst_y < crop.height(); ++dst_y)
      {
        int src_y = crop.bottom() + dst_y;
        const void * src_ptr = get_pixel_ptr<memory_lines_order, pixel_byte_size>(input.data(), crop.left(), src_y, input.height(), input.line_byte_step());
        void * dst_ptr = get_line_ptr<memory_lines_order>(output.data(), dst_y, output.height(), output.line_byte_step());

        memcpy(dst_ptr, src_ptr, line_byte_size);
      }

      return output;
    }
  };

  //////////////////////////////////////////////////////////////
  ggo::image crop(const ggo::image & input, const ggo::rect_int & crop)
  {
    ggo::rect_int clipped_rect(crop);
    if (clipped_rect.clip(input.size()) == false)
    {
      throw std::runtime_error("crop rectangle does not intersect image");
    }

    return dispatch_image_format<crop_functor>(input.pixel_type(), input.memory_lines_order(), input, clipped_rect);
  }
}
