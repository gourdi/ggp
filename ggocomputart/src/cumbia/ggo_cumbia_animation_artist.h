#ifndef __GGO_CUMBIA_ANIMATION_ARTIST__
#define __GGO_CUMBIA_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include "ggo_cumbia_artist.h"

namespace ggo
{
  class cumbia_animation_artist : public progress_animation_artist
  {
  public:

          cumbia_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    ggo::basis3d_f      _camera_basis;
    float               _camera_aperture;
    ggo::cumbia_artist  _artist;
  };
}

#endif
