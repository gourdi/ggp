#ifndef __GGO_NEWTON_ARTIST__
#define __GGO_NEWTON_ARTIST__

#include "ggo_animation_artist_abc.h"

class ggo_newton_artist : public ggo_static_background_animation_artist_abc
{
public:	
	
        ggo_newton_artist(int render_width, int render_height);
	
private:

	void	init_sub() override;
  void	init_bkgd_buffer(uint8_t * bkgd_buffer) override;
  bool	render_next_frame_bkgd(uint8_t * buffer, int frame_index) override;
	
private:
	
	bool	find_string(int index1, int index2) const;
			
	void	newton_update();
	void	newton_paint(uint8_t * buffer) const;
	
private:
	
	struct ggo_newton
	{
		ggo::point2d_float 	_cur_pos;
		ggo::point2d_float  _prv_pos;
		float				        _weight;
		ggo::vector2d_float	_forces;
		int					        _timer;
	};
	
	struct ggo_string
	{
		int		_index1;
		int		_index2;
		float	_length;
	};
	
	float					          _hue;
	std::vector<ggo_newton>	_newtons;
	std::vector<ggo_string>	_strings;
	std::vector<float>		  _max_velocities;
	ggo::vector2d_float		  _shadow_offset;
};

#endif