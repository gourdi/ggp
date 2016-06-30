#include "ggo_vortex2_artist.h"
#include <ggo_color.h>
#include <ggo_fill.h>
#include <thread>

namespace
{
  const int	MOVES_COUNT = 100;
}

//////////////////////////////////////////////////////////////
void ggo_vortex2_artist::render(uint8_t * buffer, int render_width, int render_height, const ggo_vortex2_params & params)
{
  int counter_max = render_width * render_height;
  int max_size = std::max(render_width, render_height);
  
  ggo::array<ggo::color, 2> color_buffer(render_width, render_height, ggo::color::BLACK);
	
	for (int counter = 0; counter < counter_max; ++counter)
	{
		ggo::pos2f particle;

		particle.get<0>() = ggo::rand_float(-0.1f * render_width, 1.1f * render_width);
		particle.get<1>() = ggo::rand_float(-0.1f * render_height, 1.1f * render_height);

		// The particle color.
		ggo::color color;
		if ((particle.get<0>() <= params._split_horz) && (particle.get<1>() <= params._split_vert))
		{
			color = params._color1;
		}
		if ((particle.get<0>() >= params._split_horz) && (particle.get<1>() <= params._split_vert))
		{
			color = params._color2;
		}
		if ((particle.get<0>() <= params._split_horz) && (particle.get<1>() >= params._split_vert))
		{
			color = params._color3;
		}
		if ((particle.get<0>() >= params._split_horz) && (particle.get<1>() >= params._split_vert))
		{
			color = params._color4;
		}

		// Move and render the particle.
		for (int i = 0; i < MOVES_COUNT; ++i)
		{
			// Apply the vortices.
			ggo::vec2f move(0.f, 0.f);
			for (const auto & vortex : params._vortices)
			{
				float dx	= particle.get<0>() - vortex._pos.get<0>();
				float dy	= particle.get<1>() - vortex._pos.get<1>();
				float angle	= std::atan2(dy, dx);
				float dist	= std::sqrt(dx * dx + dy * dy) / max_size;
				float force	= vortex._speed * std::pow(1 / (1 + dist), vortex._power);

				move.get<0>() += force * std::cos(angle + vortex._angle);
				move.get<1>() += force * std::sin(angle + vortex._angle);
			}
			particle += move;

			if (particle.get<0>() < 0) { particle.get<0>() += render_width; }
			else if (particle.get<0>() > render_width) { particle.get<0>() -= render_width; }

			if (particle.get<1>() < 0) { particle.get<1>() += render_height; }
			else if (particle.get<1>() > render_height) { particle.get<1>() -= render_height; }

			// Render the particle.
			int render_x = ggo::to<int>(particle.get<0>());
			int render_y = ggo::to<int>(particle.get<1>());

			if ((render_x >= 0) && (render_x < render_width) &&
				  (render_y >= 0) && (render_y < render_height))
			{
				color_buffer(render_x, render_y) += color;
			}
		}
	}
    
    // Merge render buffer from each thread.
	for (int i = 0; i < render_width * render_height; ++i)
	{
    const ggo::color & color = ggo::color::WHITE - color_buffer.data()[i];
    
		*buffer++ = color.r8();
		*buffer++ = color.g8();
		*buffer++ = color.b8();
	}
}

