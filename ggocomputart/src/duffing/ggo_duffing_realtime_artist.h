#ifndef __GGO_DUFFING_REALTIME_ARTIST__
#define __GGO_DUFFING_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include "ggo_duffing.h"

namespace ggo
{
  class duffing_realtime_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

          duffing_realtime_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override { return 1500; }

  private:

    ggo::duffing                _duffing;
    float                       _radius;
    std::array<ggo::pos2_f, 16> _points;
    ggo::rgb_8u                 _bkgd_color;
    float                       _hue;
    ggo::rgb_8u                 _paint_color;
    float                       _angle_offset;
  };
}

#endif
