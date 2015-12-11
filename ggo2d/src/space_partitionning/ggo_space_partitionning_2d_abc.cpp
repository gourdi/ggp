#include "ggo_space_partitionning_2d_abc.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void space_partitionning_2d_abc::process_shape(ggo::paintable_shape_ptr shape,
                                                 const ggo::pixel_rect & pixel_rect,
                                                 std::function<void(const ggo::pixel_rect &)> block_fct,
                                                 std::function<void(int, int, bool)> pixel_fct) const
  {
    std::vector<ggo::paintable_shape_ptr> shapes;
      
    shapes.push_back(shape);

    process_shapes(shapes, pixel_rect, block_fct, pixel_fct);
  }
}