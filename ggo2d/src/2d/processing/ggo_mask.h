#ifndef __GGO_MASK__
#define __GGO_MASK__

#include <kernel/ggo_size.h>

namespace ggo
{
  // Apply a mask on a brush.
  template <typename input_image_type, typename mask_image_type, typename brush_t>
  void apply_mask(input_image_type & image, const mask_image_type & mask, ggo::size size, brush_t && brush)
  {
    static_assert(color_traits<mask_image_type::color_t>::samples_count == 1);

    if (image.size() != mask.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < size.height(); ++y)
    {
      for (int x = 0; x < size.width(); ++x)
      {
        auto alpha = mask.read_pixel(x, y);
        auto c1 = brush(x, y);
        auto c2 = image.read_pixel(x, y);
        auto c3 = ggo::linerp(c1, c2, alpha);
        image.write_pixel(x, y, c3);
      }
    }
  }

  // Apply a mask on an image.
  template <typename input_image_type, typename mask_image_type, typename layer_image_type>
  void apply_mask(input_image_type & image, const mask_image_type & mask, const layer_image_type & layer)
  {
    static_assert(color_traits<mask_image_type::color_t>::samples_count == 1);

    if (image.size() != mask.size() || image.size() != layer.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < size.height(); ++y)
    {
      for (int x = 0; x < size.width(); ++x)
      {
        auto alpha = mask.read_pixel(x, y);
        auto c1 = layer.read_pixel(x, y);
        auto c2 = image.read_pixel(x, y);
        auto c3 = ggo::linerp(c1, c2, alpha);
        image.write_pixel(x, y, c3);
      }
    }
  }
}

#endif
