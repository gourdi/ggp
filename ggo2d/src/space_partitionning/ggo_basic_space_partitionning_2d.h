#ifndef GGO_2D_BASIC_SPACE_PARTITIONNING__
#define GGO_2D_BASIC_SPACE_PARTITIONNING__

#include <ggo_space_partitionning_2d_abc.h>

namespace ggo
{
  class basic_space_partitionning_2d : public space_partitionning_2d_abc
  {
  public:

    void process_shapes(const std::vector<ggo::paintable_shape_ptr> & shapes,
                        const ggo::pixel_rect & pixel_rect,
                        std::function<void(const ggo::pixel_rect &)> block_fct,
                        std::function<void(int, int, bool)> pixel_fct) const override;
  };
}

#endif
