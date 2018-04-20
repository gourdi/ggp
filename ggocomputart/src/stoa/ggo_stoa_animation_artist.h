#ifndef __GGO_STOA_ANIMATION_ARTIST__
#define __GGO_STOA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_stoa_artist.h"

namespace ggo
{
  class stoa_animation_artist : public animation_artist_abc
  {
  public:

          stoa_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int                               _frame_index;
    std::unique_ptr<ggo::stoa_artist> _artist;
    float                             _hue;
    float                             _light1_angle_start;
    float                             _light1_angle_end;
    float                             _light2_angle_start;
    float                             _light2_angle_end;
  };
}

#endif