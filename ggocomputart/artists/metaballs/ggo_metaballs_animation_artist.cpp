#include "ggo_metaballs_animation_artist.h"
#include <ggo_antialiasing_renderer.h>

#define GGO_BALL_SIZE 2

namespace 
{
  const int FRAMES_COUNT = 300;
}

//////////////////////////////////////////////////////////////
ggo_metaballs_animation_artist::ggo_metaballs_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_camera(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_metaballs_animation_artist::init_sub()
{
	for (int i = 0; i < 200; ++i)
	{
		ggo_moving_center center;
		
		center._center = ggo::pos3f(ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE), ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE), ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE));
		center._radius = ggo::rand_float(0, GGO_BALL_SIZE);
		center._start_angle1 = ggo::rand_float(0, 2 * ggo::pi<float>());
		center._start_angle2 = ggo::rand_float(0, 2 * ggo::pi<float>());
		center._end_angle1 = ggo::rand_float(0, 2 * ggo::pi<float>());
		center._end_angle2 = ggo::rand_float(0, 2 * ggo::pi<float>());
		
		_centers.push_back(center);
	}
	
	float angle = ggo::rand_float(0, 2 * ggo::pi<float>());
	_params._light2 = ggo::pos3f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
	_camera.basis().set_pos(0, 0, 25);
	_camera.set_aperture(0.1f);
}

//////////////////////////////////////////////////////////////
bool ggo_metaballs_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index <= FRAMES_COUNT)
	{
		std::vector<ggo::pos3f> centers;
		
		float t = ggo::ease_inout_to<float>(frame_index, FRAMES_COUNT);
		
		_params._centers.clear();
		for (std::vector<ggo_moving_center>::iterator it = _centers.begin(); it != _centers.end(); ++it)
		{
			float angle1 = ggo::map<float>(t, 0, 1, it->_start_angle1, it->_end_angle1);
			float angle2 = ggo::map<float>(t, 0, 1, it->_start_angle2, it->_end_angle2);
			
			float x1 = it->_radius;
			float y1 = 0;
			float z1 = 0;

			float x2 = std::cos(angle1) * x1 - std::sin(angle1) * y1;
			float y2 = std::sin(angle1) * x1 + std::cos(angle1) * y1;
			float z2 = z1;
			
			float x3 = std::cos(angle2) * x2 - std::sin(angle2) * z2;
			float y3 = y2;
			float z3 = std::sin(angle2) * x2 + std::cos(angle2) * z2;
			
			x3 += it->_center.get<0>();
			y3 += it->_center.get<1>();
			z3 += it->_center.get<2>();
			
			_params._centers.push_back(ggo::pos3f(x3, y3, z3));
		}

    ggo::antialiasing_renderer renderer(_camera);
		ggo_metaballs_artist artist(get_render_width(), get_render_height());
		artist.render_bitmap(buffer, renderer, _params);
		
		return true;
	}
	else
	{
		return false;
	}
}
