#include "ggo_basic_space_partitionning_2d.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void basic_space_partitionning_2d::process_shapes(const std::vector<ggo::paintable_shape_ptr> & shapes,   
                                                    const ggo::pixel_rect & pixel_rect,
                                                    std::function<void(const ggo::pixel_rect &)> block_fct,
                                                    std::function<void(int, int, bool)> pixel_fct) const
  {
    GGO_ASSERT(pixel_rect.left() <= pixel_rect.right() && pixel_rect.left() >= 0);
    GGO_ASSERT(pixel_rect.bottom() <= pixel_rect.top() && pixel_rect.bottom() >= 0);

    for (int y = pixel_rect.bottom(); y <= pixel_rect.top(); ++y)
    {
      for (int x = pixel_rect.left(); x <= pixel_rect.right(); ++x)
      {
        pixel_fct(x, y, true);
      }
    }
  }
}
