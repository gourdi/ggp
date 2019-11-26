#ifndef __GGO_DUFFING_REALTIME_ARTIST__
#define __GGO_DUFFING_REALTIME_ARTIST__

#include <ggo_realtime_artist.h>
#include <ggo_substeps_processing.h>
#include "ggo_duffing.h"

namespace ggo
{
  class duffing_realtime_artist : public realtime_artist
  {
  public:

          duffing_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

  private:

    void  preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void  render_tile(void * buffer, const ggo::rect_int & clipping) override;
    bool  finished() override;

  private:

    ggo::duffing              _duffing;
    float                     _radius;
    std::vector<ggo::pos2_f>  _points;
    ggo::rgb_8u               _bkgd_color;
    float                     _hue;
    ggo::rgb_8u               _paint_color;
    float                     _angle_offset;
    substeps_processing       _substeps_processing;
  };
}

#endif
