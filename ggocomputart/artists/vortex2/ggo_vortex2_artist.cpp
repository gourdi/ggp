#include "ggo_vortex2_artist.h"
#include <ggo_color.h>
#include <ggo_fill.h>
#include <thread>

namespace
{
  const int	moves_count = 100;
}

//////////////////////////////////////////////////////////////
void ggo::vortex2_artist::render(void * buffer, int render_width, int render_height, const ggo::vortex2_artist::params & params)
{
  int counter_max = render_width * render_height;
  int max_size = std::max(render_width, render_height);
  
  ggo::array<ggo::color_32f, 2> color_buffer(render_width, render_height, ggo::black<ggo::color_32f>());
	
	for (int counter = 0; counter < counter_max; ++counter)
	{
		ggo::pos2f particle;

		particle.x() = ggo::rand<float>(-0.1f * render_width, 1.1f * render_width);
		particle.y() = ggo::rand<float>(-0.1f * render_height, 1.1f * render_height);

		// The particle color.
		ggo::color_32f color;
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
		for (int i = 0; i < moves_count; ++i)
		{
			// Apply the vortices.
			ggo::vec2f move(0.f, 0.f);
			for (const auto & vortex : params._vortices)
			{
				float dx	= particle.x() - vortex._pos.x();
				float dy	= particle.y() - vortex._pos.y();
				float angle	= std::atan2(dy, dx);
				float dist	= std::sqrt(dx * dx + dy * dy) / max_size;
				float force	= vortex._speed * std::pow(1 / (1 + dist), vortex._power);

				move.x() += force * std::cos(angle + vortex._angle);
				move.y() += force * std::sin(angle + vortex._angle);
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
				color_buffer(render_x, render_y) += color;
			}
		}
	}
    
    // Merge render buffer from each thread.
  uint8_t * ptr = static_cast<uint8_t *>(buffer);
	for (int i = 0; i < render_width * render_height; ++i)
	{
    const ggo::color_8u color = ggo::convert_color_to<ggo::color_8u>(ggo::white<ggo::color_32f>() - color_buffer.data()[i]);
    
		*ptr++ = color.r();
		*ptr++ = color.g();
		*ptr++ = color.b();
	}
}

