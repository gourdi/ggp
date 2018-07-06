#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_curve.h>

namespace ggo
{
  class duffing_animation_artist : public animation_artist_abc
  {
  public:

                duffing_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void        render_frame(void * buffer, int frame_index, bool & finished) override;

    void				apply_shadow(float * buffer, const float * shadow_buffer) const;

  private:

    ggo::cubic_curve_float  _hue_curve;
    ggo::cubic_curve_float  _sat_curve;
    ggo::cubic_curve_float  _val_curve1;
    ggo::cubic_curve_float  _val_curve2;
    ggo::cubic_curve_float  _val_curve3;
    ggo::cubic_curve_float  _val_curve4;
    std::vector<ggo::pos2f> _points;
  };
}

#endif
