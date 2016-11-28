#include "ggo_flies_artist.h"
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_blur_paint.h>
#include <ggo_blender.h>
#include <ggo_brush.h>

//////////////////////////////////////////////////////////////
ggo::flies_artist::flies_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
static_background_animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::flies_artist::init_sub()
{
	_flies.clear();

	_hue = ggo::rand<float>();
	
	float margin = 0.1f * get_min_size();
	int count = ggo::rand<int>(10, 20);
	for (int i = 0; i < count; ++i)
	{
		fly fly;

		fly._cur_pos.get<0>() = ggo::rand<float>(margin, get_width() - margin);
		fly._cur_pos.get<1>() = ggo::rand<float>(margin, get_height() - margin);
		fly._current_angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
		fly._target_angle = fly._current_angle;
		fly._velocity = ggo::rand<float>(0.01f, 0.02f) * get_min_size();
		fly._hue = std::fmod(_hue + ggo::rand<float>(0, 0.15f), 1.f);
		fly._timer = 0;

		_flies.push_back(fly);
	}
}

//////////////////////////////////////////////////////////////
void ggo::flies_artist::init_bkgd_buffer(void * bkgd_buffer) const
{
	ggo::fill_4_colors<ggo::rgb_8u_yu>(
    bkgd_buffer, get_width(), get_height(), get_line_step(),
    ggo::from_hsv<ggo::color_8u>(_hue, 0, ggo::rand<float>(0.75, 1)),
    ggo::from_hsv<ggo::color_8u>(_hue, 0, ggo::rand<float>(0.75, 1)),
    ggo::from_hsv<ggo::color_8u>(_hue, 0, ggo::rand<float>(0.75, 1)),
    ggo::from_hsv<ggo::color_8u>(_hue, 0, ggo::rand<float>(0.75, 1)));
		
	uint8_t * it = static_cast<uint8_t *>(bkgd_buffer);
	for (int y = 0; y < get_height(); ++y)
	{
		for (int x = 0; x < get_width(); ++x)
		{
			int col = 10 * x / get_min_size();
			int row = 10 * y / get_min_size();
			
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
bool ggo::flies_artist::render_next_frame_bkgd(void * buffer, int frame_index)
{
	float margin = 0.1f * get_min_size();
		
	// Collision detection.
	for (auto & fly : _flies)
	{
		ggo::vec2f velocity = ggo::from_polar(fly._current_angle, fly._velocity);
		
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
			if ((fly._cur_pos.get<0>() < margin) && (velocity.get<0>() < 0))
			{
				velocity.get<0>() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.get<0>() > get_width() - margin) && (velocity.get<0>() > 0))
			{
				velocity.get<0>() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.get<1>() < margin) && (velocity.get<1>() < 0))
			{
				velocity.get<1>() *= -1.f;
				fly._timer = 1;
			}
			
			if ((fly._cur_pos.get<1>() > get_height() - margin) && (velocity.get<1>() > 0))
			{
				velocity.get<1>() *= -1.f;
				fly._timer = 1;
			}

			// Check for fly collision.
			const ggo::flies_artist::fly * closest_fly = nullptr;
			float dist_min = 0;
			for (const auto & fly2 : _flies)
			{
				if (&fly == &fly2)
				{
					continue;
				}
				
				ggo::vec2f diff = fly2._cur_pos - fly._cur_pos;
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
				ggo::vec2f ortho(velocity.get<1>(), -velocity.get<0>());
				ggo::vec2f diff = closest_fly->_cur_pos - fly._cur_pos;

				if (ggo::dot(ortho, diff) > 0)
				{
					ortho *= -1;
				}

				velocity = ortho;
				
				fly._timer = 1;
			}

			// Update target angle.
			float angle = ggo::get_angle(velocity);
			float min_diff = std::abs(fly._current_angle - angle);
			fly._target_angle = angle;
			
			angle = ggo::get_angle(velocity) + 2 * ggo::pi<float>();
			if (std::abs(fly._current_angle - angle) < min_diff)
			{
				min_diff = std::abs(fly._current_angle - angle);
				fly._target_angle = angle;
			}
			
			angle = ggo::get_angle(velocity) - 2 * ggo::pi<float>();
			if (std::abs(fly._current_angle - angle) < min_diff)
			{
				min_diff = std::abs(fly._current_angle - angle);
				fly._target_angle = angle;
			}
		}
	}
	
	// Paint shadows.
	float shadow_scale = 0.035f * get_min_size();
	float shadow_offset = 0.02f * get_min_size();
	float shadow_blur = 0.0075f * get_min_size();
	
	for (const auto & fly : _flies)
	{
		ggo::vec2f v1 = ggo::from_polar(fly._current_angle, shadow_scale);
		ggo::vec2f v2 = ggo::from_polar(fly._current_angle + 2 * ggo::pi<float>() / 3, shadow_scale);
		ggo::vec2f v3 = ggo::from_polar(fly._current_angle - 2 * ggo::pi<float>() / 3, shadow_scale);

    ggo::polygon2d_float triangle(3);
		triangle.add_point(fly._cur_pos + v1);
		triangle.add_point(fly._cur_pos + v2);
		triangle.add_point(fly._cur_pos + v3);
		
		triangle.move(shadow_offset, shadow_offset);

    auto paint_pixel = [&](int x, int y, int samples_count, int samples_sup)
    {
      ggo::color_8u c_8u = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step());
      uint8_t r = uint8_t(ggo::round_div<uint32_t>((samples_sup - samples_count) * c_8u.r(), samples_count));
      uint8_t g = uint8_t(ggo::round_div<uint32_t>((samples_sup - samples_count) * c_8u.g(), samples_count));
      uint8_t b = uint8_t(ggo::round_div<uint32_t>((samples_sup - samples_count) * c_8u.b(), samples_count));

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), ggo::color_8u(r, g, b));
    };
        
    ggo::paint_blur_shape<ggo::blur_samples_type::disc_52_samples>(
      triangle, get_width(), get_height(), shadow_blur, paint_pixel);
	}
	
	// Paint detection circles.
	for (const auto & fly : _flies)
	{
		ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      ggo::disc_float(fly._cur_pos, margin),
      ggo::make_solid_brush(ggo::white<ggo::color_8u>()), ggo::alpha_blender_rgb8u(0.5f * fly._timer));
	}
	
	// Paint flies.	
	float flies_scale = 0.025f * get_min_size();
	
	for (const auto & fly : _flies)
	{
		ggo::vec2f v1 = ggo::from_polar(fly._current_angle, flies_scale);
		ggo::vec2f v2 = ggo::from_polar(fly._current_angle + 2 * ggo::pi<float>() / 3, flies_scale);
		ggo::vec2f v3 = ggo::from_polar(fly._current_angle - 2 * ggo::pi<float>() / 3, flies_scale);

    ggo::polygon2d_float triangle(3);
		triangle.add_point(fly._cur_pos + v1);
		triangle.add_point(fly._cur_pos + v2);
		triangle.add_point(fly._cur_pos + v3);

		float val = std::min(1.f, 0.5f + 0.5f * fly._timer);
		
		ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      triangle, ggo::from_hsv<ggo::color_8u>(fly._hue, 1, val));

    float width = 0.0025f * get_min_size();
    ggo::multi_shape_float border;
    border.add_shape(std::make_shared<ggo::extended_segment_float>(triangle.get_point(0), triangle.get_point(1), width));
    border.add_shape(std::make_shared<ggo::extended_segment_float>(triangle.get_point(1), triangle.get_point(2), width));
    border.add_shape(std::make_shared<ggo::extended_segment_float>(triangle.get_point(2), triangle.get_point(0), width));

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      border, ggo::black<ggo::color_8u>());
	}
	
	// Move flies.
	for (auto & fly : _flies)
	{
		ggo::vec2f velocity = ggo::from_polar(fly._current_angle, fly._velocity);

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
