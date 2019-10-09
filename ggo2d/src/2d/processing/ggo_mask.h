#ifndef __GGO_MASK__
#define __GGO_MASK__

#include <kernel/ggo_size.h>

namespace ggo
{
  template <typename image_t, typename mask_image_t, typename brush_t>
  void apply_mask(image_t & image, const mask_image_t & mask, brush_t && brush)
  {
    static_assert(color_traits<mask_image_t::color_t>::samples_count == 1);

    if (image.size() != mask.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < image.height(); ++y)
    {
      for (int x = 0; x < image.width(); ++x)
      {
        auto alpha = mask.read_pixel(x, y);
        auto c1 = brush(x, y);
        auto c2 = image.read_pixel(x, y);
        auto c3 = ggo::linerp(c1, c2, alpha);
        image.write_pixel(x, y, c3);
      }
    }
  }
}

#endif
