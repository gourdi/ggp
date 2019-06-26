#ifndef __GGO_MASK__
#define __GGO_MASK__

#include <kernel/ggo_size.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  // Apply a mask on a brush.
  template <ggo::image_format buffer_format, ggo::image_format mask_format, typename brush_t>
  void apply_mask(void * buffer, int buffer_line_byte_step, const void * mask, int mask_line_byte_step,
    ggo::size size, brush_t brush)
  {
    using mask_color_t = image_format_traits<mask_format>::color_t;
    static_assert(color_traits<mask_color_t>::samples_count == 1);

    if constexpr (ggo::image_format_traits<buffer_format>::lines_order == ggo::image_format_traits<mask_format>::lines_order)
    {
      for (int y = 0; y < size.height(); ++y)
      {
        void * buffer_it = buffer;
        const void * mask_it = mask;

        for (int x = 0; x < size.width(); ++x)
        {
          auto alpha = ggo::read_pixel<mask_format>(mask_it);
          mask_it = ggo::move_ptr(mask_it, ggo::image_format_traits<mask_format>::pixel_byte_size);

          auto c1 = brush(x, y);
          auto c2 = ggo::read_pixel<buffer_format>(buffer_it);
          auto c3 = ggo::linerp(c1, c2, alpha);
          ggo::write_pixel<buffer_format>(buffer_it, c3);
          buffer_it = ggo::move_ptr(buffer_it, ggo::image_format_traits<buffer_format>::pixel_byte_size);
        }

        buffer = ggo::move_ptr(buffer, buffer_line_byte_step);
        mask = ggo::move_ptr(mask, mask_line_byte_step);
      }
    }
    else
    {
      GGO_FAIL();
    }
  }

  // Apply a mask on an image.
  template <ggo::image_format buffer_format, ggo::image_format mask_format, ggo::image_format layer_format = buffer_format>
  void apply_mask(void * buffer, int buffer_line_byte_step, const void * mask, int mask_line_byte_step,
    const void * layer, int layer_line_syte_step, ggo::size size)
  {
    using mask_color_t = image_format_traits<mask_format>::color_t;
    static_assert(color_traits<mask_color_t>::samples_count == 1);

    if constexpr (
      ggo::image_format_traits<buffer_format>::lines_order == ggo::image_format_traits<mask_format>::lines_order &&
      ggo::image_format_traits<layer_format>::lines_order  == ggo::image_format_traits<mask_format>::lines_order)
    {
      for (int y = 0; y < size.height(); ++y)
      {
        void * buffer_it = buffer;
        const void * mask_it = mask;
        const void * layer_it = layer;

        for (int x = 0; x < size.width(); ++x)
        {
          auto alpha = ggo::read_pixel<mask_format>(mask_it);
          mask_it = ggo::move_ptr(mask_it, ggo::image_format_traits<mask_format>::pixel_byte_size);

          auto c1 = ggo::read_pixel<layer_format>(layer_it);
          layer_it = ggo::move_ptr(layer_it, ggo::image_format_traits<buffer_format>::pixel_byte_size);

          auto c2 = ggo::read_pixel<buffer_format>(buffer_it);
          auto c3 = ggo::linerp(c1, c2, alpha);
          ggo::write_pixel<buffer_format>(buffer_it, c3);
          buffer_it = ggo::move_ptr(buffer_it, ggo::image_format_traits<buffer_format>::pixel_byte_size);
        }

        buffer = ggo::move_ptr(buffer, buffer_line_byte_step);
        mask = ggo::move_ptr(mask, mask_line_byte_step);
        layer = ggo::move_ptr(layer, layer_line_syte_step);
      }
    }
    else
    {
      GGO_FAIL();
    }
  }

}

#endif
