#ifndef __GGO_BADABOUM_REALTIME_ARTIST__
#define __GGO_BADABOUM_REALTIME_ARTIST__

#include <ggo_realtime_artist_abc.h>
#include <badaboum/ggo_badaboum_artist.h>

namespace ggo
{
  class badaboum_realtime_artist : public fixed_frames_count_realtime_artist_abc
  {
  public:

    badaboum_realtime_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
    int   frames_count() const override;

  private:

    ggo::badaboum_artist _artist;
  };
}

#endif
