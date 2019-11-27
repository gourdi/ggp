#ifndef __GGO_CRYSTAL_ANIMATION_ARTIST__
#define __GGO_CRYSTAL_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include "ggo_crystal_artist.h"

namespace ggo
{
  class crystal_animation_artist : public progress_animation_artist
  {
  public:

          crystal_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    ggo::rgb_8u                 _bkgd_colors[4];
    ggo::crystal_artist::params _start_params;
    ggo::crystal_artist::params _end_params;
  };
}

#endif
