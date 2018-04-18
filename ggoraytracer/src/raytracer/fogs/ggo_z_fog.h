#ifndef __GGO_Z_FOG__
#define __GGO_Z_FOG__

#include <raytracer/fogs/ggo_fog_abc.h>

namespace ggo
{
  class z_fog : public fog_abc
  {
  public:

                      z_fog(const ggo::color_32f & color, float z1, float z2, float far);

      ggo::color_32f  process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const override;
      ggo::color_32f  process_light_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const override;
      ggo::color_32f  process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const override;
      
  private:

      ggo::color_32f  map_color(const ggo::color_32f & color, const ggo::pos3f & p1, const ggo::pos3f & p2) const;
      
  private:

      ggo::color_32f  _color;
      float           _z_inf;
      float           _z_sup;
      float           _far;
  };
}

#endif