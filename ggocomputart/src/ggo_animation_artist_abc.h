#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>
#include <2d/ggo_image_format.h>

namespace ggo
{
  class animation_artist_abc : public artist
  {
  public:

    static animation_artist_abc * create(animation_artist_id artist_id, int width, int height, int line_step, ggo::image_format format);

    bool  render_frame(void * buffer);

  protected:

    virtual void  render_frame(void * buffer, int frame_index, bool & finished) = 0;

                  animation_artist_abc(int width, int height, int line_step, ggo::image_format format);

  private:

    int _frame_index = 0;
  };
}

namespace ggo
{
  class fixed_frames_count_animation_artist_abc : public animation_artist_abc
  {
  protected:

    fixed_frames_count_animation_artist_abc(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, bool & finished) override;

    virtual void  render_frame(void * buffer, int frame_index) = 0;
    virtual int   frames_count() const = 0;
  };
}

#endif
