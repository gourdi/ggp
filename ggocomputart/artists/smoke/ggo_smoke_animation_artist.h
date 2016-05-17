#ifndef _GGO_SMOKE_ANIMATION_ARTIST__
#define _GGO_SMOKE_ANIMATION_ARTIST__

#include <ggo_array.h>
#include "ggo_animation_artist_abc.h"

#define GGO_SMOKE_FRAMES_COUNT 600

class ggo_smoke_animation_artist : public ggo_animation_artist_abc
{
public:
	
        ggo_smoke_animation_artist(int render_width, int render_height);
	
	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:
	
	class ggo_loop_array2d : public ggo::array<double, 2>
	{
	public:
		
            ggo_loop_array2d(int size_x, int size_y);
		
		double 	interpolate(double x, double y) const;
		double 	loop_value(int x, int y) const;
	};
	
	struct ggo_fluid_source
	{
		ggo::circle_double	_circle;
		double				      _speed;
		double				      _angle;
		double				      _angle_amplitude;
		double				      _angle_offset;
		double				      _wave_length;
		int					        _timer1;
		int					        _timer2;
		double				      _density;
	};

	ggo_loop_array2d	              _velocity_x1;
	ggo_loop_array2d	              _velocity_x2;
	ggo_loop_array2d	              _velocity_y1;
	ggo_loop_array2d	              _velocity_y2;
	ggo_loop_array2d *              _velocity_x_cur;
	ggo_loop_array2d *              _velocity_x_tmp;
	ggo_loop_array2d *              _velocity_y_cur;
	ggo_loop_array2d *              _velocity_y_tmp;
	ggo_loop_array2d	              _density1;
	ggo_loop_array2d	              _density2;
	ggo_loop_array2d *              _density_cur;
	ggo_loop_array2d *              _density_tmp;
	ggo::array<ggo_fluid_source, 1> _sources;
	ggo::array<uint8_t, 1>			    _bkgd_buffer;
	ggo::color					            _smoke_color;
};

#endif
