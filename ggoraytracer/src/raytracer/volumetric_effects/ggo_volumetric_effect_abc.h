#ifndef __GGO_FOG_ABC__
#define __GGO_FOG_ABC__

#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class volumetric_effect_abc
  {
  public:

    virtual ggo::color_32f                  process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const = 0;
    virtual ggo::color_32f                  process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const = 0;
    virtual std::optional<box3d_data_float> get_bounding_box() const = 0;
  };
}

#endif
