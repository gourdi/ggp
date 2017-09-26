#ifndef __GGO_SIMPLE_COLOR_OBJECT3D__
#define __GGO_SIMPLE_COLOR_OBJECT3D__

#include <ggo_object3d_abc.h>
#include <ggo_material_abc.h>

#if 0

namespace ggo
{
  template <uint32_t flags, typename shape_t, typename material_t>
  class simple_color_object3d : public shape_object3d<discard_basis>
  {
  private:

    ggo::color_32f  process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const std::function<ggo::color_32f(ggo::ray3d_float)> & raytrace) const override;

  private:

    material_t  _material;
  };
}

#include <ggo_simple_color_object3d.imp.h>

#endif

#endif