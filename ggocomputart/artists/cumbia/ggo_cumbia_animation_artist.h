#ifndef __GGO_CUMBIA_ANIMATION_ARTIST__
#define __GGO_CUMBIA_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_cumbia_artist.h"
#include <ggo_point_camera.h>

namespace ggo
{
  class cumbia_animation_artist : public animation_artist_abc
  {
  public:

          cumbia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    int                             _frame_index;
    ggo::cumbia_artist	            _artist;
    ggo::antialiasing_point_camera	_camera;
  };
}

#endif
