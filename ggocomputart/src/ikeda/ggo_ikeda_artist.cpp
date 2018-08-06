#include "ggo_ikeda_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::ikeda_artist::ikeda_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format)
{
  _u0.set_harmonic(0, 0, 0);
  _u0.set_harmonic(1, ggo::rand<float>(-1, 1), 0);
  _u0.set_harmonic(2, ggo::rand<float>(-1, 1), 0);
  _u0.set_harmonic(3, ggo::rand<float>(-1, 1), 0);

  _u1.set_harmonic(0, 0, 0);
  _u1.set_harmonic(1, ggo::rand<float>(-1, 1), 0);
  _u1.set_harmonic(2, ggo::rand<float>(-1, 1), 0);
  _u1.set_harmonic(3, ggo::rand<float>(-1, 1), 0);

  _u2.set_harmonic(0, 0, 0);
  _u2.set_harmonic(1, ggo::rand<float>(-10, 10), 0);
  _u2.set_harmonic(2, ggo::rand<float>(-10, 10), 0);
  _u2.set_harmonic(3, ggo::rand<float>(-10, 10), 0);

  _hue1 = ggo::rand<float>();
  _hue2 = ggo::rand<float>();
  _range = ggo::rand<float>(3, 5);

  for (int i = 0; i < 500; ++i)
  {
    _seeds.push_back(create_seed());
  }

  _bkgd_colors[0] = ggo::from_hsv<ggo::rgb_8u>(0, 0, ggo::rand<float>(0, 0.5f));
  _bkgd_colors[1] = ggo::from_hsv<ggo::rgb_8u>(0, 0, ggo::rand<float>(0, 0.5f));
  _bkgd_colors[2] = ggo::from_hsv<ggo::rgb_8u>(0, 0, ggo::rand<float>(0, 0.5f));
  _bkgd_colors[3] = ggo::from_hsv<ggo::rgb_8u>(0, 0, ggo::rand<float>(0, 0.5f));
}

//////////////////////////////////////////////////////////////
void ggo::ikeda_artist::render_frame(void * buffer, int frame_index)
{
	ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(),
    _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3],
    ggo::rect_int::from_size(size()));
	
	std::vector<particle> particles = _seeds;
	
	float u0 = _u0.evaluate(frame_index * ggo::pi<float>() / frames_count());
	float u1 = _u1.evaluate(frame_index * ggo::pi<float>() / frames_count());
	float u2 = _u2.evaluate(frame_index * ggo::pi<float>() / frames_count());
	
	for (auto & particle : particles)
	{
    for (int i = 0; i < 10000; ++i)
		{
			float k = 1 + particle._pos.x() * particle._pos.x() + particle._pos.y() * particle._pos.y();
			if (std::abs(k) < 0.00001f)
			{
				break;
			}
				
			float t = u1 - u2 / k;
		
			ggo::pos2f next_pt;
			next_pt.x() = 1 + u0 * (particle._pos.x() * cos(t) - particle._pos.y() * sin(t));
			next_pt.y() =     u0 * (particle._pos.x() * sin(t) - particle._pos.y() * cos(t));

			float speed = ggo::distance(particle._pos, next_pt);
			particle._radius = speed;

			if (speed < 0.05f)
			{
        break;
      }
			
			// Paint the point.
			ggo::pos2f point = map_fill(particle._pos, -_range, _range);
				
			float radius = 0.0025f * particle._radius * max_size();
			radius = std::max(1.5f, radius);

      ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
        buffer, width(), height(), line_step(),
        ggo::disc_float(point, radius), ggo::make_solid_brush(particle._color), ggo::alpha_blender_rgb8u(0.1f));
				
			// Move points slowly.
			particle._pos.x() += 0.0025f * (next_pt.x() - particle._pos.x());
			particle._pos.y() += 0.0025f * (next_pt.y() - particle._pos.y());
		}
	}
}

//////////////////////////////////////////////////////////////
ggo::ikeda_artist::particle ggo::ikeda_artist::create_seed() const
{
	particle particle;

	switch (ggo::rand<int>(0, 3))
	{
		case 0:
			particle._pos.x() = ggo::rand<float>(-1.5f * _range, 1.5f * _range);
			particle._pos.y() = 1.5f * _range;
			break;
		case 1:
			particle._pos.x() = ggo::rand<float>(-1.5f * _range, 1.5f * _range);
			particle._pos.y() = -1.5f * _range;
			break;
		case 2:
			particle._pos.x() = 1.5f * _range;
			particle._pos.y() = ggo::rand<float>(-1.5f * _range, 1.5f * _range);
			break;
		case 3:
			particle._pos.x() = -1.5f * _range;
			particle._pos.y() = ggo::rand<float>(-1.5f * _range, 1.5f * _range);
			break;
	}

	if (ggo::rand<bool>())
	{
		particle._color = ggo::from_hsv<ggo::rgb_8u>(_hue1, ggo::rand<float>(0.4f, 0.6f), ggo::rand<float>(0.4f, 1.f));
	}
	else
	{
		particle._color = ggo::from_hsv<ggo::rgb_8u>(_hue2, ggo::rand<float>(0.4f, 0.6f), ggo::rand<float>(0.4f, 1.f));
	}

	return particle;
}


