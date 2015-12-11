#ifndef __GGO_LINEAR_FOG__
#define __GGO_LINEAR_FOG__

#include <ggo_fog_abc.h>

namespace ggo
{
  class linear_fog : public fog_abc
  {
  public:

                  linear_fog(const ggo::color & color, float far);

      ggo::color  process_segment(const ggo::point3d_float & p1, const ggo::point3d_float & p2, const ggo::color & color) const override;
      ggo::color  process_background_ray(const ggo::ray3d_float & ray, const ggo::color & color) const override;
      
  private:

      ggo::color  _color;
      float       _far;
  };
}

#endif

