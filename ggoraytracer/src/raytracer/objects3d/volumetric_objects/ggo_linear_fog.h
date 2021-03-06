#ifndef __GGO_LINEAR_FOG__
#define __GGO_LINEAR_FOG__

#include <raytracer/objects3d/volumetric_objects/ggo_volumetric_object_abc.h>

namespace ggo
{
  class linear_fog : public volumetric_object_abc
  {
  public:

                                      linear_fog(const ggo::rgb_32f & color, float near, float far);

      ggo::rgb_32f                    process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const override;
      ggo::rgb_32f                    process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const override;
      std::optional<box3d_data_float> get_bounding_box() const override { return {}; }

  private:

      ggo::rgb_32f  _color;
      float         _near;
      float         _far;
  };
}

#endif

