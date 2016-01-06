#ifndef __GGO_2D__
#define __GGO_2D__

#include <memory>
#include <ggo_shapes2d.h>

namespace ggo
{
  using paintable_shape_ptr = std::shared_ptr<const ggo::paintable_shape2d_abc<float>>;
}

#endif
