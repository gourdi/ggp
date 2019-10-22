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

  class realtime_artist_abc : public paint_artist
  {
  public:

    realtime_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    static realtime_artist_abc * create(realtime_artist_id artist_id, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    void preprocess_frame(uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step);
    void render_tile(void * buffer, const ggo::rect_int & clipping);
    bool finished();

  private:

    virtual void preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) = 0;
    virtual void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) = 0;
    virtual bool finished(int frame_index) const = 0;

  private:

    int _frame_index = 0;
  };
}

namespace ggo
{
  class fixed_frames_count_realtime_artist_abc : public realtime_artist_abc
  {
  public:

    fixed_frames_count_realtime_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    bool finished(int frame_index) const override;

    virtual int frames_count() const = 0;
  };
}


#endif
