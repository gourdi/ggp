#include "ggo_vortex_artist.h"
#include <2d/ggo_color.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  const int	moves_count = 100;
}

//////////////////////////////////////////////////////////////
void ggo::vortex_artist::render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::vortex_artist::params & params)
{
  int counter_max = width * height;
  int max_size = std::max(width, height);
  
  ggo::array<ggo::rgb_32f, 2> color_buffer(width, height, ggo::black<ggo::rgb_32f>());
	
	for (int counter = 0; counter < counter_max; ++counter)
	{
		ggo::pos2f particle;

		particle.x() = ggo::rand<float>(-0.1f * width, 1.1f * width);
		particle.y() = ggo::rand<float>(-0.1f * height, 1.1f * height);

		// The particle color.
		ggo::rgb_32f color;
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

			if (particle.x() < 0) { particle.x() += width; }
			else if (particle.x() > width) { particle.x() -= width; }

			if (particle.y() < 0) { particle.y() += height; }
			else if (particle.y() > height) { particle.y() -= height; }

			// Render the particle.
			int render_x = ggo::round_to<int>(particle.x());
			int render_y = ggo::round_to<int>(particle.y());

			if ((render_x >= 0) && (render_x < width) &&
				  (render_y >= 0) && (render_y < height))
			{
				color_buffer(render_x, render_y) += color;
			}
		}
	}
    
  // Merge render buffer from each thread.
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      const ggo::rgb_8u color = ggo::convert_color_to<ggo::rgb_8u>(ggo::white<ggo::rgb_32f>() - color_buffer(x, y));

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, line_step, color);
    }
	}
}

