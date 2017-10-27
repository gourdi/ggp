#ifndef __GGO_HEXA_ANIMATION_ARTIST__
#define __GGO_HEXA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_scalar_field_2d_abc.h>
#include <ggo_point_camera.h>
#include "ggo_hexa_artist.h"

namespace ggo
{
  class object3d;

  class hexa_animation_artist : public animation_artist_abc
  {
  public:

          hexa_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int                               _frame_index;
    std::unique_ptr<ggo::hexa_artist> _artist;
  };
}

#endif

