#ifndef __GGO_CRYSTAL_ANIMATION_ARTIST__
#define __GGO_CRYSTAL_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_crystal_artist.h"

namespace ggo
{
  class crystal_animation_artist : public animation_artist_abc
  {
  public:

          crystal_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int _frame_index;
    ggo::crystal_artist _artist;
    ggo::color_8u _bkgd_colors[4];
    ggo::crystal_artist::params _start_params;
    ggo::crystal_artist::params _end_params;
  };
}

#endif
