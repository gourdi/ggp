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

          duffing_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) override;

  private:

    ggo::cubic_curve_float    _hue_curve;
    ggo::cubic_curve_float    _sat_curve;
    ggo::cubic_curve_float    _val_curve1;
    ggo::cubic_curve_float    _val_curve2;
    ggo::cubic_curve_float    _val_curve3;
    ggo::cubic_curve_float    _val_curve4;
    std::vector<ggo::pos2_f>  _points;
  };
}

#endif
