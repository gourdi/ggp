#ifndef __GGO_LINEAR_FOG__
#define __GGO_LINEAR_FOG__

#include <ggo_fog_abc.h>

namespace ggo
{
  class linear_fog : public fog_abc
  {
  public:

    enum flags
    {
      discard_light_segments = 1 << 0
    };

                      linear_fog(const ggo::color_32f & color, float near, float far, uint32_t flags = 0);

      ggo::color_32f  process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const override;
      ggo::color_32f  process_light_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const override;
      ggo::color_32f  process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const override;
      
  private:

      ggo::color_32f  _color;
      float           _near;
      float           _far;
      uint32_t        _flags;
  };
}

#endif

