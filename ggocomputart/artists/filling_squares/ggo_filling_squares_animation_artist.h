#ifndef __GGO_FILLING_SQUARES_ANIMATION_ARTIST__
#define __GGO_FILLING_SQUARES_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_filling_squares_artist.h"
#include <ggo_animate_abc.h>
#include <ggo_animator.h>

class ggo_filling_squares_animation_artist : public ggo_dynamic_background_animation_artist_abc
{
public:

					ggo_filling_squares_animation_artist(int render_width, int render_height);

private:
	
	class ggo_animated_square : public ggo_animate_abc
	{
    public:
    
            ggo_animated_square(int start_offset) : ggo_animate_abc(start_offset) {};
    
		bool    update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter) override;

		ggo::point2d_float 								              _pos;
		float											                      _angle;
		ggo_filling_squares_artist::ggo_colored_square 	_colored_square;
        
    static  bool bkgd_rendering_allowed;
	};

  void	init_sub() override;
	void	init_bkgd_buffer(uint8_t * bkgd_buffer) override;
	bool	render_next_frame_bkgd(uint8_t * output_buffer, uint8_t * bkgd_buffer, int frame_index) override;
	
private:
	
	ggo_animator  _animator;
  float         _hue;
};

#endif
