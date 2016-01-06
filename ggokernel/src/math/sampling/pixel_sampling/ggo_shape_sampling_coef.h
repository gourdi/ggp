#ifndef __GGO_SHAPE_SAMPLING__
#define __GGO_SHAPE_SAMPLING__

#include <ggo_shapes2d.h>

namespace ggo
{
  uint16_t get_sampling_coef4X4(const ggo::samplable_shape2d_abc<float> & shape, int x, int y);
  uint64_t get_sampling_coef8X8(const ggo::samplable_shape2d_abc<float> & shape, int x, int y);
}

#endif

