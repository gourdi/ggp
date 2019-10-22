#ifndef __GGO_STOA_ANIMATION_ARTIST__
#define __GGO_STOA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_stoa_artist.h"

namespace ggo
{
  class stoa_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          stoa_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

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