#include "ggo_metaballs_animation_artist.h"
#include <ggo_antialiasing_renderer.h>

//////////////////////////////////////////////////////////////
ggo::metaballs_animation_artist::metaballs_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height),
_camera(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_animation_artist::init_sub()
{
  const float ball_size = 2;


	for (int i = 0; i < 200; ++i)
	{
		moving_center center;
		
		center._center = ggo::pos3f(ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size));
		center._radius = ggo::rand<float>(0, ball_size);
		center._start_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
		center._start_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());
		center._end_angle1 = ggo::rand<float>(0, 2 * ggo::pi<float>());
		center._end_angle2 = ggo::rand<float>(0, 2 * ggo::pi<float>());
		
		_centers.push_back(center);
	}
	
	float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_params._light2 = ggo::pos3f(1000 * std::cos(angle), 1000 * std::sin(angle), 1000.f);
	_camera.basis().set_pos(0, 0, 25);
	_camera.set_aperture(0.1f);
}

//////////////////////////////////////////////////////////////
bool ggo::metaballs_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 300;

  if (frame_index >= frames_count)
  {
    return false;
  }
	
  std::vector<ggo::pos3f> centers;
		
		float t = ggo::ease_inout_to<float>(frame_index, frames_count);
		
		_params._centers.clear();
		for (const auto & center : _centers)
		{
			float angle1 = ggo::map<float>(t, 0, 1, center._start_angle1, center._end_angle1);
			float angle2 = ggo::map<float>(t, 0, 1, center._start_angle2, center._end_angle2);
			
			float x1 = center._radius;
			float y1 = 0;
			float z1 = 0;

			float x2 = std::cos(angle1) * x1 - std::sin(angle1) * y1;
			float y2 = std::sin(angle1) * x1 + std::cos(angle1) * y1;
			float z2 = z1;
			
			float x3 = std::cos(angle2) * x2 - std::sin(angle2) * z2;
			float y3 = y2;
			float z3 = std::sin(angle2) * x2 + std::cos(angle2) * z2;
			
			x3 += center._center.x();
			y3 += center._center.y();
			z3 += center._center.z();
			
			_params._centers.push_back(ggo::pos3f(x3, y3, z3));
		}

    ggo::antialiasing_renderer renderer(_camera);
		ggo::metaballs_artist artist(get_render_width(), get_render_height());
		artist.render_bitmap(buffer, renderer, _params);
		
		return true;
}
