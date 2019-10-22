#ifndef __GGO_DEMECO_ANIMATION_ARTIST__
#define __GGO_DEMECO_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_demeco_artist.h"
#include <2d/ggo_image.h>

namespace ggo
{
  class demeco_animation_artist : public animation_artist_abc
  {
  public:

    demeco_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) override;

  private:

    demeco_artist<ggo::pixel_type::rgba_8u, ggo::lines_order::down, ggo::sampling_8x8> _artist;
    ggo::image_t<ggo::pixel_type::rgba_8u, ggo::lines_order::down> _render_image;
    ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::down> _shadow_image;
  };
}

#endif