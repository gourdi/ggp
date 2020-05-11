#ifndef __GGO_2D_CROP__
#define __GGO_2D_CROP__

#include <2d/ggo_image.h>
#include <kernel/ggo_rect_int.h>

// Details.
namespace ggo
{
  namespace crop_details
  {
    template <typename input_image_t, typename make_output_image_func>
    image crop(const input_image_t & input, const ggo::rect_int & crop_rect, make_output_image_func make_output_image)
    {
      ggo::rect_int clipped_rect(crop_rect);
      if (clipped_rect.clip(input.size()) == false)
      {
        throw std::runtime_error("crop rectangle does not intersect image");
      }

      const int line_byte_size = input.pixel_byte_size() * clipped_rect.width();

      auto output = make_output_image(clipped_rect.size());

      for (int dst_y = 0; dst_y < clipped_rect.height(); ++dst_y)
      {
        int src_y = clipped_rect.bottom() + dst_y;
        const void * src_ptr = input.pixel_ptr(clipped_rect.left(), src_y);
        void * dst_ptr = output.line_ptr(dst_y);

        memcpy(dst_ptr, src_ptr, line_byte_size);
      }

      return output;
    }
  }
}

// Static images.
namespace ggo
{
  template <pixel_type pixel_type, lines_order memory_lines_order, typename void_ptr_t>
  auto crop(const image_base_t<pixel_type, memory_lines_order, void_ptr_t> & input, const ggo::rect_int & crop_rect)
  {
    auto make_output_image = [](const ggo::size & crop_size)
    {
      return ggo::image_t<pixel_type, memory_lines_order>(crop_size);
    };

    return crop_details::crop(input, crop_rect, make_output_image);
  }
}

// Dynamic images.
namespace ggo
{
  template <typename void_ptr_t>
  auto crop(const image_base<void_ptr_t> & input, const ggo::rect_int & crop_rect)
  {
    auto make_output_image = [&](const ggo::size & crop_size)
    {
      return ggo::image(crop_size, input.pixel_type(), input.memory_lines_order());
    };

    return crop_details::crop(input, crop_rect, make_output_image);
  }
}

#endif
