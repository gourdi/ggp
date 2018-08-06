#ifndef __GGO_Z_FOG__
#define __GGO_Z_FOG__

#include <raytracer/objects3d/volumetric_objects/ggo_volumetric_object_abc.h>

namespace ggo
{
  class z_fog : public volumetric_object_abc
  {
  public:

                                      z_fog(const ggo::rgb_32f & color, float z1, float z2, float far);

      ggo::rgb_32f                    process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::rgb_32f & color) const override;
      ggo::rgb_32f                    process_background_ray(const ggo::ray3d_float & ray, const ggo::rgb_32f & color) const override;
      std::optional<box3d_data_float> get_bounding_box() const override { return {}; }

  private:

      ggo::rgb_32f                    map_color(const ggo::rgb_32f & color, const ggo::pos3f & p1, const ggo::pos3f & p2) const;
      
  private:

      ggo::rgb_32f  _color;
      float         _z_inf;
      float         _z_sup;
      float         _far;
  };
}

#endif