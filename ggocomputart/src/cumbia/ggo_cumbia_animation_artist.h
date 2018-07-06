#ifndef __GGO_CUMBIA_ANIMATION_ARTIST__
#define __GGO_CUMBIA_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_cumbia_artist.h"

namespace ggo
{
  class cumbia_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          cumbia_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index) override;
    int   frames_count() const override { return 200; }

  private:

    ggo::basis3d_float  _camera_basis;
    float               _camera_aperture;
    ggo::cumbia_artist  _artist;
  };
}

#endif
