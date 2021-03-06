#ifndef __GGO_DISC_GLOW__
#define __GGO_DISC_GLOW__

#include <raytracer/objects3d/volumetric_objects/ggo_volumetric_object_abc.h>

namespace ggo
{
  class disc_glow : public volumetric_object_abc
  {
  public:

                                    disc_glow(const ggo::pos3_f & pos, float opacity, const ggo::rgb_32f & color, float inner_radius, float outter_radius);

    ggo::rgb_32f                    process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const override;
    ggo::rgb_32f                    process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const override;
    std::optional<box3d_data_float> get_bounding_box() const override;

  private:

    ggo::pos3_f      _pos;
    float           _opacity;
    ggo::rgb_32f    _color;
    float           _inner_radius;
    float           _outter_radius;
  };
}

#endif
