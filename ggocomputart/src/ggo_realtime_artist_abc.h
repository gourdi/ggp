#ifndef __GGO_REALTIME_ARTIST_ABC__
#define __GGO_REALTIME_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>

namespace ggo
{
  class realtime_artist_abc : public artist
  {
  public:

    realtime_artist_abc(int width, int height, int line_step, ggo::image_format format);

    static realtime_artist_abc * create(realtime_artist_id artist_id, int width, int height, int line_step, ggo::image_format format);

    void preprocess_frame();
    void render_tile(void * buffer, const ggo::rect_int & clipping);
    bool finished();

  private:

    virtual void preprocess_frame(int frame_index) = 0;
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

    fixed_frames_count_realtime_artist_abc(int width, int height, int line_step, ggo::image_format format);

  private:

    bool finished(int frame_index) const override;

    virtual int frames_count() const = 0;
  };
}


#endif