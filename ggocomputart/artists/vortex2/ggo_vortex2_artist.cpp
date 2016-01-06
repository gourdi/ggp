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
  
  ggo::array<ggo::color> color_buffer(render_width * render_height, ggo::color::BLACK);
	
	for (int counter = 0; counter < counter_max; ++counter)
	{
		ggo::point2d_float particle;

		particle.x() = ggo::rand_float(-0.1f * render_width, 1.1f * render_width);
		particle.y() = ggo::rand_float(-0.1f * render_height, 1.1f * render_height);

		// The particle color.
		ggo::color color;
		if ((particle.x() <= params._split_horz) && (particle.y() <= params._split_vert))
		{
			color = params._color1;
		}
		if ((particle.x() >= params._split_horz) && (particle.y() <= params._split_vert))
		{
			color = params._color2;
		}
		if ((particle.x() <= params._split_horz) && (particle.y() >= params._split_vert))
		{
			color = params._color3;
		}
		if ((particle.x() >= params._split_horz) && (particle.y() >= params._split_vert))
		{
			color = params._color4;
		}

		// Move and render the particle.
		for (int i = 0; i < MOVES_COUNT; ++i)
		{
			// Apply the vortices.
			ggo::vector2d_float move(0, 0);
			for (int vortex = 0; vortex < params._vortices.get_size(); ++vortex )
			{
				float dx	= particle.x() - params._vortices[vortex]._pos.x();
				float dy	= particle.y() - params._vortices[vortex]._pos.y();
				float angle	= std::atan2(dy, dx);
				float dist	= std::sqrt(dx * dx + dy * dy) / max_size;
				float force	= params._vortices[vortex]._speed * std::pow(1 / (1 + dist), params._vortices[vortex]._power);

				move.x() += force * std::cos(angle + params._vortices[vortex]._angle);
				move.y() += force * std::sin(angle + params._vortices[vortex]._angle);
			}
			particle += move;

			if (particle.x() < 0) { particle.x() += render_width; }
			else if (particle.x() > render_width) { particle.x() -= render_width; }

			if (particle.y() < 0) { particle.y() += render_height; }
			else if (particle.y() > render_height) { particle.y() -= render_height; }

			// Render the particle.
			int render_x = ggo::to<int>(particle.x());
			int render_y = ggo::to<int>(particle.y());

			if ((render_x >= 0) && (render_x < render_width) &&
				(render_y >= 0) && (render_y < render_height))
			{
				color_buffer[render_y * render_width + render_x] += color;
			}
		}
	}
    
    // Merge render buffer from each thread.
	for (int i = 0; i < render_width * render_height; ++i)
	{
    const ggo::color & color = ggo::color::WHITE - color_buffer[i];
    
		*buffer++ = color.r8();
		*buffer++ = color.g8();
		*buffer++ = color.b8();
	}
}

