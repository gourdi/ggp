#ifndef GGO_BACKGROUND3D_ABC__
#define GGO_BACKGROUND3D_ABC__

#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class background3d_abc
  {
  public:

    virtual	ggo::color_32f  get_color(const ggo::ray3d_float & ray) const = 0;
  };
}

#endif

