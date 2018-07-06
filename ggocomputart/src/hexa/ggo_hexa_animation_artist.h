#ifndef __GGO_HEXA_ANIMATION_ARTIST__
#define __GGO_HEXA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include "ggo_hexa_artist.h"

namespace ggo
{
  class object3d;

  class hexa_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          hexa_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index) override;
    int   frames_count() const override { return 300; }

  private:

    std::unique_ptr<ggo::hexa_artist> _artist;
  };
}

#endif

