#ifndef _GGO_FLIES_ARTIST__
#define _GGO_FLIES_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_array.h>

namespace ggo
{
  class flies_artist : public static_background_animation_artist_abc
  {
  public:

    flies_artist(int render_width, int render_height);

    void	init_sub() override;
    void	init_bkgd_buffer(void * bkgd_buffer) const override;
    bool	render_next_frame_bkgd(void * buffer, int frame_index) override;

  private:

    struct fly
    {
      ggo::pos2f  _cur_pos;
      float				_velocity;
      float				_current_angle;
      float				_target_angle;
      float				_hue;
      float 			_timer;
    };

    float             _hue;
    std::vector<fly>  _flies;
  };
}

#endif
