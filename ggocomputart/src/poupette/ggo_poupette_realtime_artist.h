#ifndef __GGO_POUPETTE_REALTIME_ARTIST__
#define __GGO_POUPETTE_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include "ggo_poupette_artist.h"

namespace ggo
{
  class poupette_realtime_artist : public realtime_artist_abc
  {
  public:

    poupette_realtime_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos) override;
    void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    bool finished(int frame_index) const override;

  private:

    ggo::poupette_artist _artist;
  };
}

#endif