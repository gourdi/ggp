#include "ggo_smoke_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_smoke_animation_artist::ggo_loop_array2d::ggo_loop_array2d(int size_x, int size_y)
:
ggo::array2d<double>(size_x, size_y)
{
}

//////////////////////////////////////////////////////////////
// Here this(0, 0) is really located at (0, 0), meaning that
// the array covers a (-0.5, size_x+0.5) x (-0.5, size_y+0.5)
// region.
double ggo_smoke_animation_artist::ggo_loop_array2d::interpolate(double x, double y) const
{
	// Bilinear interpolation.
	int src_left = x < 0 ? int(x) - 1 : int(x);
	int src_bottom = y < 0 ? int(y) - 1 : int(y);
	
	double alpha_x = x - src_left; 
	double alpha_y = y - src_bottom;
	GGO_ASSERT(alpha_x >= -0.0001 && alpha_x <= 1.0001);
	GGO_ASSERT(alpha_y >= -0.0001 && alpha_y <= 1.0001);
	
	src_left = ggo::pos_mod(src_left, get_size_x());
	src_bottom = ggo::pos_mod(src_bottom, get_size_y());
	
	int src_right = (src_left + 1) % get_size_x();
	int src_top = (src_bottom + 1) % get_size_y();

	double bottom = (1. - alpha_x) * this->operator()(src_left, src_bottom) + alpha_x * this->operator()(src_right, src_bottom);
	double top = (1. - alpha_x) * this->operator()(src_left, src_top) + alpha_x * this->operator()(src_right, src_top);

	return (1. - alpha_y) * bottom + alpha_y * top;
}

//////////////////////////////////////////////////////////////
double ggo_smoke_animation_artist::ggo_loop_array2d::loop_value(int x, int y) const
{
	x = ggo::pos_mod(x, get_size_x());
	y = ggo::pos_mod(y, get_size_y());
	
	return this->operator()(x, y);
}

//////////////////////////////////////////////////////////////
ggo_smoke_animation_artist::ggo_smoke_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_velocity_x1(render_width + 1, render_height),
_velocity_x2(render_width + 1, render_height),
_velocity_y1(render_width, render_height + 1),
_velocity_y2(render_width, render_height + 1),
_density1(render_width, render_height),
_density2(render_width, render_height),
_bkgd_buffer(3 * render_width * render_height),
_sources(4)
{
}

//////////////////////////////////////////////////////////////
void ggo_smoke_animation_artist::init_sub()
{
	_velocity_x_cur = &_velocity_x1;
	_velocity_x_tmp = &_velocity_x2;
	_velocity_y_cur = &_velocity_y1;
	_velocity_y_tmp = &_velocity_y2;
	_density_cur = &_density1;
	_density_tmp = &_density2;
	
	_velocity_x_cur->fill(0);
	_velocity_y_cur->fill(0);
	_velocity_x_tmp->fill(0);
	_velocity_y_tmp->fill(0);
	_density_cur->fill(0);
	_density_tmp->fill(0);
	
	double center_x = get_render_width() / 2;
	double center_y = get_render_height() / 2;

	for (int i = 0; i < _sources.get_size(); ++i)
	{
		ggo_fluid_source & source = _sources[i];
		
		switch (i)
		{
		case 0:
			// Top.
			source._circle.center().x() = center_x;
			source._circle.center().y() = center_y + 0.25 * get_render_min_size();
			source._angle = -ggo::PI<float>() / 2;
			break;
		case 1:
			// Right.
			source._circle.center().x() = center_x + 0.25 * get_render_min_size();
			source._circle.center().y() = center_y;
			source._angle = ggo::PI<float>();
			break;
		case 2:
			// Bottom.
			source._circle.center().x() = center_x;
			source._circle.center().y() = center_y - 0.25 * get_render_min_size();
			source._angle = ggo::PI<float>() / 2;
			break;
		case 3:
			// Left.
			source._circle.center().x() = center_x - 0.25 * get_render_min_size();
			source._circle.center().y() = center_y;
			source._angle = 0;
			break;
		}
	
		source._circle.radius() = ggo::rand_float(0.03f * get_render_min_size(), 0.04f * get_render_min_size());
		source._speed = ggo::rand_float(0.007f * get_render_min_size(), 0.009f * get_render_min_size());
		source._timer1 = ggo::rand_int(50, 100);
		source._timer2 = ggo::rand_int(50, 150);
		source._angle_amplitude = ggo::rand_float(ggo::PI<float>() / 4, ggo::PI<float>());
		source._angle_offset = ggo::rand_float(0, 2 * ggo::PI<float>());
		source._wave_length = ggo::rand_float(0.05f, 0.2f);
		source._density = 0;
	}
	
	ggo_fluid_source & source = _sources[ggo::rand_int(0, 3)];
	source._timer1 = 0;

	float hue = ggo::rand_float();
	_smoke_color = ggo::color::from_hsv(hue, 1, ggo::rand_float(0.2f, 0.8f));
	ggo::color color1 = ggo::color::from_hsv(hue, ggo::rand_float(0.f, 0.2f), ggo::rand_float(0.8f, 1.f));
	ggo::color color2 = ggo::color::from_hsv(hue, ggo::rand_float(0.f, 0.2f), ggo::rand_float(0.8f, 1.f));
	ggo::color color3 = ggo::color::from_hsv(hue, ggo::rand_float(0.f, 0.2f), ggo::rand_float(0.8f, 1.f));
	ggo::color color4 = ggo::color::from_hsv(hue, ggo::rand_float(0.f, 0.2f), ggo::rand_float(0.8f, 1.f));
  
  ggo::rgb_image_data_uint8 image_data(_bkgd_buffer, get_render_width(), get_render_height());
	ggo::fill_4_colors(image_data, color1, color2, color3, color4);
}

//////////////////////////////////////////////////////////////
bool ggo_smoke_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index > GGO_SMOKE_FRAMES_COUNT)
	{
		return false;
	}
	
	// Process the sources.
	for (int i = 0; i < _sources.get_size(); ++i)
	{
		ggo_fluid_source & source = _sources[i];
		
		if (source._timer1 > 0)
		{
			source._timer1 -= 1;
		}
		else if (source._timer2 > 0)
		{
			--source._timer2;
			source._density = std::min(source._density + 0.1, 1.0);
		}
		else if (source._density > 0)
		{
			source._density = std::max(source._density - 0.01, 0.0);
		}
	}
	
	for (int y = 0; y < get_render_height(); ++y) 
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			for (int i = 0; i < _sources.get_size(); ++i)
			{
				ggo_fluid_source & source = _sources[i];
				
				if (source._density > 0)
				{
					double dist = ggo::distance(double(x), double(y), source._circle.center().x(), source._circle.center().y());
					if (dist < source._circle.radius())
					{
						_density_cur->operator()(x, y) = std::min(_density_cur->operator()(x, y) + source._density, 1.0);
					}
					
					if (dist < 0.8 * source._circle.radius())
					{
						double angle = source._angle + source._angle_amplitude * std::cos(source._wave_length * frame_index + source._angle_offset);
					
						_velocity_x_cur->operator()(x, y) = source._speed * std::cos(angle);
						_velocity_y_cur->operator()(x, y) = source._speed * std::sin(angle);
					}
				}
			}
		}
	}
	
	// Horizontal self advection.
	for (int y = 0; y < get_render_height(); ++y) 
	{
		for (int x = 0; x < get_render_width() + 1; ++x)
		{
			double velocity_x = _velocity_x_cur->loop_value(x, y);
			double velocity_y = 0.25 * (_velocity_y_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1) + _velocity_y_cur->loop_value(x - 1, y) + _velocity_y_cur->loop_value(x - 1, y + 1));

			_velocity_x_tmp->operator()(x, y) = _velocity_x_cur->interpolate(x - velocity_x, y - velocity_y);
		}
	}
	
	std::swap(_velocity_x_tmp, _velocity_x_cur);
	
	// Vertical self advection.
	for (int y = 0; y < get_render_height() + 1; ++y) 
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			double velocity_x = 0.25 * (_velocity_x_cur->loop_value(x, y) + _velocity_x_cur->loop_value(x, y - 1) + _velocity_x_cur->loop_value(x + 1, y) + _velocity_x_cur->loop_value(x + 1, y - 1));
			double velocity_y = _velocity_y_cur->loop_value(x, y);

			_velocity_y_tmp->operator()(x, y) = _velocity_y_cur->interpolate(x - velocity_x, y - velocity_y);
		}
	}
	
	std::swap(_velocity_y_tmp, _velocity_y_cur);
	
	// Compute pressure.
	ggo_loop_array2d divergence(get_render_width(), get_render_height());
	ggo_loop_array2d pressure(get_render_width(), get_render_height());
	for (int y = 0; y < get_render_height(); ++y)
	{
		for (int x = 0; x < get_render_width(); ++x) 
		{
			divergence(x, y) = -(_velocity_x_cur->loop_value(x + 1, y) - _velocity_x_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1) - _velocity_y_cur->loop_value(x, y));
			divergence(x, y) *= get_render_min_size();
			pressure(x, y) = 0;
		}
	}
	
	// Solve pressure with Gauss-Seidel.
	for (int k = 0; k < 50; ++k) 
	{
		for (int y = 0; y < get_render_height(); ++y)
		{
			for (int x = 0; x < get_render_width(); ++x) 
			{
				pressure(x, y) = (divergence(x, y) + pressure.loop_value(x - 1, y) + pressure.loop_value(x + 1, y) + pressure.loop_value(x, y - 1) + pressure.loop_value(x, y + 1)) / 4;
			}
		}
	}

	// Apply pressure.
	for (int y = 0; y < get_render_height(); ++y) 
	{
		for (int x = 0; x < get_render_width() + 1; ++x)
		{
			_velocity_x_cur->operator()(x, y) -= (pressure.loop_value(x, y) - pressure.loop_value(x - 1, y)) / get_render_min_size();
		}
	}
	
	for (int y = 0; y < get_render_height() + 1; ++y) 
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			_velocity_y_cur->operator()(x, y) -= (pressure.loop_value(x, y) - pressure.loop_value(x, y - 1)) / get_render_min_size();
		}
	}

	// Density advection.
	for (int y = 0; y < get_render_height(); ++y) 
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			// Backtracking.
			double velocity_x = 0.5 * (_velocity_x_cur->loop_value(x, y) + _velocity_x_cur->loop_value(x + 1, y));
			double velocity_y = 0.5 * (_velocity_y_cur->loop_value(x, y) + _velocity_y_cur->loop_value(x, y + 1));
			
			double density = _density_cur->interpolate(x - velocity_x, y - velocity_y);

			_density_tmp->operator()(x, y) = density;
		}
	}
	
	std::swap(_density_cur, _density_tmp);
	
	// Draw the density.
	uint8_t * bkgd_it = _bkgd_buffer;
	for (int y = 0; y < get_render_height(); ++y) 
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			double density = _density_cur->operator()(x, y);

			buffer[0] = ggo::to<uint8_t>(density * 255 * _smoke_color.r() + (1 - density) * bkgd_it[0]);
			buffer[1] = ggo::to<uint8_t>(density * 255 * _smoke_color.g() + (1 - density) * bkgd_it[1]);
			buffer[2] = ggo::to<uint8_t>(density * 255 * _smoke_color.b() + (1 - density) * bkgd_it[2]);
			
			buffer += 3;
			bkgd_it += 3;
		}
	}

	return true;
}
