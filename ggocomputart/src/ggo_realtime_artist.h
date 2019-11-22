#ifndef __GGO_REALTIME_ARTIST_ABC__
#define __GGO_REALTIME_ARTIST_ABC__

#include <ggo_paint_artist.h>
#include <ggo_artist_ids.h>

namespace ggo
{
  enum cursor_event
  {
    cursor_event_down = 1 << 0,
    cursor_event_up = 1 << 1
  };

  class realtime_artist : public paint_artist
  {
  public:

    realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    static realtime_artist * create(realtime_artist_id artist_id, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

    virtual void preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos) = 0;
    virtual void render_tile(void * buffer, const ggo::rect_int & clipping) = 0;
    virtual bool finished() = 0;
  };
}

#endif
