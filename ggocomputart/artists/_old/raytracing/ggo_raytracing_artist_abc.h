#ifndef __GGO_RAYTRACING_ARTIST_ABC__
#define __GGO_RAYTRACING_ARTIST_ABC__

#include "ggo_artist_abc.h"
#include <ggo_raytracer.h>
#include <ggo_camera3d.h>

class ggo_raytracing_artist_abc : public ggo_artist_abc
{
public:
	
					ggo_raytracing_artist_abc(int render_width, int render_height, ggo_rendering_mode rendering_mode);

private:

	virtual	bool	process_sub(int counter, uint8_t * buffer);
	virtual	void	render_bkgd(uint8_t * buffer);

protected:

	ggo_camera3d		_camera;
	ggo_raytrace_params	_raytrace_params;
	ggo_raytracer		_raytracer;
};

#endif