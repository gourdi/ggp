#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>

namespace ggo
{
  class animation_artist_abc : public artist
  {
  public:

    static animation_artist_abc * create(animation_artist_id artist_id, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    bool  render_frame(void * buffer, float time_step);

  protected:

    virtual void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) = 0;

                  animation_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    int _frame_index = 0;
  };
}

namespace ggo
{
  class fixed_frames_count_animation_artist_abc : public animation_artist_abc
  {
  protected:

    fixed_frames_count_animation_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int frames_count);

    int frames_count() const { return _frames_count; }

  private:

    void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) override;

    virtual void  render_frame(void * buffer, int frame_index, float time_step) = 0;
    
  private:

    const int _frames_count;
  };
}

#endif
