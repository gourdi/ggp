#ifndef _GGO_FLIES_ARTIST__
#define _GGO_FLIES_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_array.h>

class ggo_flies_artist : public ggo_static_background_animation_artist_abc
{
public:
	
        ggo_flies_artist(int render_width, int render_height);
	
	void	init_sub() override;
  void	init_bkgd_buffer(uint8_t * bkgd_buffer) override;
  bool	render_next_frame_bkgd(uint8_t * buffer, int frame_index) override;
    
private:
	
	struct ggo_fly
	{
		ggo::pos2f  _cur_pos;
		float				_velocity;
		float				_current_angle;
		float				_target_angle;
		float				_hue;
		float 			_timer;
	};
	
  float                 _hue;
	std::vector<ggo_fly>	_flies;
};

#endif
