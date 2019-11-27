#ifndef __ggo_stoa_animation_artist__
#define __ggo_stoa_animation_artist__

#include <ggo_animation_artist.h>
#include "ggo_stoa_artist.h"

namespace ggo
{
  class stoa_animation_artist : public progress_animation_artist
  {
  public:

          stoa_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    std::unique_ptr<ggo::stoa_artist> _artist;
    float                             _hue;
    float                             _light1_angle_start;
    float                             _light1_angle_end;
    float                             _light2_angle_start;
    float                             _light2_angle_end;
  };
}

#endif