#ifndef __GGO_HEXA_ANIMATION_ARTIST__
#define __GGO_HEXA_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include <kernel/math/scalar_fields_2d/ggo_scalar_field_2d_abc.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include "ggo_hexa_artist.h"

namespace ggo
{
  class object3d;

  class hexa_animation_artist : public progress_animation_artist
  {
  public:

          hexa_animation_artist(int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    std::unique_ptr<ggo::hexa_artist> _artist;
  };
}

#endif

