#include "ggo_raytracing_artist_abc.h"
#include <iostream>

//////////////////////////////////////////////////////////////
ggo_raytracing_artist_abc::ggo_raytracing_artist_abc(int render_width, int render_height, ggo_rendering_mode rendering_mode)
:
ggo_artist_abc(render_width, render_height, rendering_mode)
{
}

//////////////////////////////////////////////////////////////
bool ggo_raytracing_artist_abc::process_sub(int counter, uint8_t * buffer)
{
	if (counter >= get_render_height())
	{
		std::cout << std::endl;
		return false;
	}

	std::cout << "Rendering line " << counter << "\r" << std::flush;
	
	_raytracer.render_line(buffer + 3 * counter * get_render_width(), get_render_width(), get_render_height(), _camera, counter, _raytrace_params);

	return true;
}

//////////////////////////////////////////////////////////////
void ggo_raytracing_artist_abc::render_bkgd(uint8_t * buffer)
{
	memset(buffer, 0, 3 * get_render_width() * get_render_height());
}
