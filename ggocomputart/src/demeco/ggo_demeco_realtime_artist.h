#ifndef __GGO_DEMECO_REALTIME_ARTIST__
#define __GGO_DEMECO_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include "ggo_demeco_artist.h"

namespace ggo
{
  class demeco_realtime_artist : public realtime_artist_abc
  {
  public:

    demeco_realtime_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool finished(int frame_index) const override { return _finished; }

  private:

    demeco_artist<ggo::bgrx_8u_yd, ggo::sampling_8x8> _artist;
    bool _finished = false;
  };
}

#endif