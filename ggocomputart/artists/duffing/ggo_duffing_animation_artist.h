#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_array.h>
#include <ggo_curve.h>

namespace ggo
{
  class duffing_animation_artist : public animation_artist_abc
  {
  public:

                duffing_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void				init_sub() override;
    bool				render_next_frame_sub(void * buffer, int frame_index) override;

    void				apply_shadow(float * buffer, const float * shadow_buffer) const;

    ggo::pos2f  apply_duffing(float t, float dt, float angle_offset, ggo::pos2f & point) const;

  private:

    ggo::cubic_curve_float	_hue_curve;
    ggo::cubic_curve_float	_sat_curve;
    ggo::cubic_curve_float	_val_curve1;
    ggo::cubic_curve_float	_val_curve2;
    ggo::cubic_curve_float	_val_curve3;
    ggo::cubic_curve_float	_val_curve4;
    std::vector<ggo::pos2f> _points;
  };
}

#endif
