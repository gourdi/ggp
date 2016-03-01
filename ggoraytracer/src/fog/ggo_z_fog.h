#ifndef __GGO_Z_FOG__
#define __GGO_Z_FOG__

#include <ggo_fog_abc.h>

namespace ggo
{
  class z_fog : public fog_abc
  {
  public:

                  z_fog(const ggo::color & color, float z1, float z2, float far);

      ggo::color  process_segment(const ggo::point3d_float & p1, const ggo::point3d_float & p2, const ggo::color & color) const override;
      ggo::color  process_background_ray(const ggo::ray3d_float & ray, const ggo::color & color) const override;
      
  private:

      ggo::color  map_color(const ggo::color & color, const ggo::point3d_float & p1, const ggo::point3d_float & p2) const;
      
  private:

      ggo::color   _color;
      float       _z_inf;
      float       _z_sup;
      float       _far;
  };
}

#endif