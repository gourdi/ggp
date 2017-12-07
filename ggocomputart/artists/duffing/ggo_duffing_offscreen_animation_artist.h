#ifndef __GGO_DUFFING_OFFSCREEN_ANIMATION_ARTIST__
#define __GGO_DUFFING_OFFSCREEN_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_array.h>
#include <ggo_curve.h>

namespace ggo
{
  class duffing_offscreen_animation_artist : public animation_artist_abc
  {
  public:

                duffing_offscreen_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void        init() override;
    bool        prepare_frame() override;
    void        process_frame(void * buffer, const ggo::rect_int & clipping) override;

    void				apply_shadow(float * buffer, const float * shadow_buffer) const;

  private:

    int                     _frame_index;
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
