#ifndef __GGO_BADABOUM_ANIMATION_ARTIST__
#define __GGO_BADABOUM_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <badaboum/ggo_badaboum_artist.h>

namespace ggo
{
  class badaboum_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

    badaboum_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    ggo::badaboum_artist _artist;
  };
}

#endif
