#ifndef __GGO_GLOW__
#define __GGO_GLOW__

#include <ggo_object3d_abc.h>

#if 0
namespace ggo
{
  class glow : public object3d_abc
  {
  private:

    ggo::color_32f get_emissive_color() const override { return ggo::black_32f(); }
    ggo::color_32f process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, float random_variable1, float random_variable2) const override;

  private:

    ggo::pos3f  _pos;
    float       _radius;
  };
}

#endif

#endif
