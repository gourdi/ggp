#ifndef __GGO_DUFFING_REALTIME_ANIMATION_ARTIST__
#define __GGO_DUFFING_REALTIME_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_duffing.h"

namespace ggo
{
  class duffing_realtime_animation_artist : public animation_artist_abc
  {
  public:

          duffing_realtime_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int                         _frame_index;
    ggo::duffing                _duffing;
    float                       _radius;
    std::array<ggo::pos2f, 16>  _points;
    ggo::color_8u               _bkgd_color;
    float                       _hue;
    ggo::color_8u               _paint_color;
    float                       _angle_offset;
  };
}

#endif
