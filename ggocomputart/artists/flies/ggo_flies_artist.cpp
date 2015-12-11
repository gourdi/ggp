#include "ggo_flies_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <cmath>

//////////////////////////////////////////////////////////////
ggo_flies_artist::ggo_flies_artist(int render_width, int render_height)
:
ggo_static_background_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_flies_artist::init_sub()
{
	_flies.clear();

	_hue = ggo::rand_float();
	
	float margin = 0.1f * get_render_min_size();
	int count = ggo::rand_int(10, 20);
	for (int i = 0; i < count; ++i)
	{
		ggo_fly fly;

		fly._cur_pos.x() = ggo::rand_float(margin, get_render_width() - margin);
		fly._cur_pos.y() = ggo::rand_float(margin, get_render_height() - margin);
		fly._current_angle = ggo::rand_float(0, 2 * ggo::PI<float>());
		fly._target_angle = fly._current_angle;
		fly._velocity = ggo::rand_float(0.01f, 0.02f) * get_render_min_size();
		fly._hue = std::fmod(_hue + ggo::rand_float(0, 0.15f), 1.f);
		fly._timer = 0;

		_flies.push_back(fly);
	}
}

//////////////////////////////////////////////////////////////
void ggo_flies_artist::init_bkgd_buffer(uint8_t * bkgd_buffer)
{
  ggo::rgb_image_data_uint8 image_data(bkgd_buffer, get_render_width(), get_render_height());
	ggo::fill_4_colors(image_data, 
                    ggo::color::from_hsv(_hue, 0, ggo::rand_float(0.75, 1)),
                    ggo::color::from_hsv(_hue, 0, ggo::rand_float(0.75, 1)),
                    ggo::color::from_hsv(_hue, 0, ggo::rand_float(0.75, 1)),
                    ggo::color::from_hsv(_hue, 0, ggo::rand_float(0.75, 1)));
		
	uint8_t * it = bkgd_buffer;
	for (int y = 0; y < get_render_height(); ++y)
	{
		for (int x = 0; x < get_render_width(); ++x)
		{
			int col = 10 * x / get_render_min_size();
			int row = 10 * y / get_render_min_size();
			
			if ((col + row) % 2)
			{
				it[0] = ggo::to<uint8_t>(0.9 * it[0]);
				it[1] = ggo::to<uint8_t>(0.9 * it[1]);
				it[2] = ggo::to<uint8_t>(0.9 * it[2]);
			}
			
			it += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo_flies_artist::render_next_frame_bkgd(uint8_t * buffer, int frame_index)
{
	float margin = 0.1f * get_render_min_size();
		
	// Collision detection.
	for (auto & fly : _flies)
	{
		ggo::vector2d_float velocity = ggo::vector2d_float::from_polar(fly._current_angle, fly._velocity);
		
		if (fly._current_angle > fly._target_angle + 0.001)
		{
			fly._current_angle -= std::min(0.5f, fly._current_angle - fly._target_angle);
		}
		else if (fly._current_angle < fly._target_angle - 0.001)
		{
			fly._current_angle += std::min(0.5f, fly._target_angle - fly._current_angle);
		}
		else
		{
			// Check borders.
			if ((fly._cur_pos.x() < margin) && (velocity.x() < 0))
			{
				velocity.x() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.x() > get_render_width() - margin) && (velocity.x() > 0))
			{
				velocity.x() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.y() < margin) && (velocity.y() < 0))
			{
				velocity.y() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.y() > get_render_height() - margin) && (velocity.y() > 0))
			{
				velocity.y() *= -1.f;
				fly._timer = 1;
			}

			// Check for fly collision.
			const ggo_fly * closest_fly = nullptr;
			float dist_min = 0;
			for (const auto & fly2 : _flies)
			{
				if (&fly == &fly2)
				{
					continue;
				}
				
				ggo::vector2d_float diff = fly2._cur_pos - fly._cur_pos;
				float dist = diff.get_length();
				
				// The other fly is too far.
				if (dist > margin)
				{
					continue;
				}
				
				// The other fly is behind the current one.
				if (ggo::dot(velocity, diff) < 0)
				{
					continue;
				}
				
				if ((closest_fly == nullptr) || (dist < dist_min))
				{
					closest_fly = &fly2;
					dist_min = dist;
				}
			}
			
			if (closest_fly != nullptr)
			{
				ggo::vector2d_float ortho(velocity.y(), -velocity.x());
				ggo::vector2d_float diff = closest_fly->_cur_pos - fly._cur_pos;

				if (ggo::dot(ortho, diff) > 0)
				{
					ortho *= -1;
				}

				velocity = ortho;
				
				fly._timer = 1;
			}

			// Update target angle.
			float angle = velocity.get_angle();
			float min_diff = std::abs(fly._current_angle - angle);
			fly._target_angle = angle;
			
			angle = velocity.get_angle() + 2 * ggo::PI<float>();
			if (std::abs(fly._current_angle - angle) < min_diff)
			{
				min_diff = std::abs(fly._current_angle - angle);
				fly._target_angle = angle;
			}
			
			angle = velocity.get_angle() - 2 * ggo::PI<float>();
			if (std::abs(fly._current_angle - angle) < min_diff)
			{
				min_diff = std::abs(fly._current_angle - angle);
				fly._target_angle = angle;
			}
		}
	}
	
	// Paint shadows.
	float shadow_scale = 0.035f * get_render_min_size();
	float shadow_offset = 0.02f * get_render_min_size();
	float shadow_blur = 0.0075f * get_render_min_size();
	
	for (const auto & fly : _flies)
	{
		ggo::vector2d_float v1 = ggo::vector2d_float::from_polar(fly._current_angle, shadow_scale);
		ggo::vector2d_float v2 = ggo::vector2d_float::from_polar(fly._current_angle + 2 * ggo::PI<float>() / 3, shadow_scale);
		ggo::vector2d_float v3 = ggo::vector2d_float::from_polar(fly._current_angle - 2 * ggo::PI<float>() / 3, shadow_scale);

		auto triangle = std::make_shared<ggo::polygon2d_float>(3);
		triangle->add_point(fly._cur_pos + v1);
		triangle->add_point(fly._cur_pos + v2);
		triangle->add_point(fly._cur_pos + v3);
		
		triangle->move(shadow_offset, shadow_offset);
        
    ggo::paint(buffer, get_render_width(), get_render_height(),
               triangle,
               ggo::color::BLACK, 1, std::make_shared<ggo::rgb_alpha_blender>(),
               ggo::blur_pixel_sampler(shadow_blur));
	}
	
	// Paint detection circles.
	for (const auto & fly : _flies)
	{
		ggo::paint(buffer, get_render_width(), get_render_height(),
               std::make_shared<ggo::disc_float>(fly._cur_pos, margin),
               ggo::color::WHITE, 0.5f * fly._timer);
	}
	
	// Paint flies.	
	float flies_scale = 0.025f * get_render_min_size();
	
	for (const auto & fly : _flies)
	{
		ggo::vector2d_float v1 = ggo::vector2d_float::from_polar(fly._current_angle, flies_scale);
		ggo::vector2d_float v2 = ggo::vector2d_float::from_polar(fly._current_angle + 2 * ggo::PI<float>() / 3, flies_scale);
		ggo::vector2d_float v3 = ggo::vector2d_float::from_polar(fly._current_angle - 2 * ggo::PI<float>() / 3, flies_scale);

		auto triangle = std::make_shared<ggo::polygon2d_float>(3);
		triangle->add_point(fly._cur_pos + v1);
		triangle->add_point(fly._cur_pos + v2);
		triangle->add_point(fly._cur_pos + v3);

		float val = std::min(1.f, 0.5f + 0.5f * fly._timer);
		
		ggo::paint(buffer, get_render_width(), get_render_height(),
               triangle, 
               ggo::color::from_hsv(fly._hue, 1, val));

    float width = 0.0025f * get_render_min_size();
    auto border = std::make_shared<ggo::multi_shape_float>();
    border->add_shape(std::make_shared<ggo::extended_segment_float>(triangle->get_point(0), triangle->get_point(1), width));
    border->add_shape(std::make_shared<ggo::extended_segment_float>(triangle->get_point(1), triangle->get_point(2), width));
    border->add_shape(std::make_shared<ggo::extended_segment_float>(triangle->get_point(2), triangle->get_point(0), width));

		ggo::paint(buffer, get_render_width(), get_render_height(), border, ggo::color::BLACK);
	}
	
	// Move flies.
	for (auto & fly : _flies)
	{
		ggo::vector2d_float velocity = ggo::vector2d_float::from_polar(fly._current_angle, fly._velocity);

		fly._cur_pos += velocity;

		fly._timer = std::max(0.f, fly._timer - 0.1f);
	}
	
	// Fade out.
	if (frame_index > 400)
	{
        return false;
	}
	
	return true;
}
