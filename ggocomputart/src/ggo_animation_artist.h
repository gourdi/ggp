#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_paint_artist.h>
#include <ggo_artist_ids.h>
#include <kernel/ggo_ratio.h>

namespace ggo
{
  class animation_artist_abc : public paint_artist
  {
  public:

    static animation_artist_abc * create(
      animation_artist_id artist_id, int width, int height, int line_byte_step,
      ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps);

    virtual void  render_frame(void * buffer, bool & finished) = 0;

  protected:

                  animation_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
  };
}

namespace ggo
{
  class progress_animation_artist_abc : public animation_artist_abc
  {
  protected:

    progress_animation_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio duration, ggo::ratio fps);

  private:

            void  render_frame(void * buffer, bool & finished) override;
    virtual void  render_frame(void * buffer, float progress) = 0;
    
  private:

    const ggo::ratio _duration;
    const ggo::ratio _fps;
    ggo::ratio _elapsed_time = { 0, 1 };
  };
}

#endif
