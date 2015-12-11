#ifndef __GGO_2D_SPACE_PARTITIONNING_ABC__
#define __GGO_2D_SPACE_PARTITIONNING_ABC__

#include <ggo_2d.h>
#include <ggo_pixel_rect.h>

namespace ggo
{
  class space_partitionning_2d_abc
  {
  public:

            void process_shape(ggo::paintable_shape_ptr shape,
                               const ggo::pixel_rect & pixel_rect,
                               std::function<void(const ggo::pixel_rect &)> block_fct,
                               std::function<void(int, int, bool)> pixel_fct) const;

    virtual void process_shapes(const std::vector<ggo::paintable_shape_ptr> & shapes,
                                const ggo::pixel_rect & pixel_rect,
                                std::function<void(const ggo::pixel_rect &)> block_fct,
                                std::function<void(int, int, bool)> pixel_fct) const = 0;
  };
}

#endif
