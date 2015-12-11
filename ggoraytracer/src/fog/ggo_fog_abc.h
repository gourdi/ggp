#ifndef __GGO_FOG_ABC__
#define __GGO_FOG_ABC__

#include <ggo_set3.h>
#include <ggo_shapes3d.h>
#include <ggo_color.h>

namespace ggo
{
  class fog_abc
  {
  public:

    virtual ggo::color  process_segment(const ggo::point3d_float & p1, const ggo::point3d_float & p2, const ggo::color & color) const = 0;
    virtual ggo::color  process_background_ray(const ggo::ray3d_float & ray, const ggo::color & color) const = 0;
  };
}

#endif
