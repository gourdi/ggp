#ifndef __GGO_STAIRS_ARTIST__
#define __GGO_STAIRS_ARTIST__

#include "ggo_raytracing_artist_abc.h"

class ggo_stairs_artist : public ggo_raytracing_artist_abc
{
public:

							ggo_stairs_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode);

private:

	virtual ggo_artist_id  	get_id() const { return GGO_ARTIST_STAIRS; };
	virtual std::string		get_name() const { return "Stairs"; };
	
			void			create_lights(float hue);
			void			create_objects(float hue);

private:

	ggo_raytracer		_raytracer;
	ggo_camera3d		_camera;
	ggo_raytrace_params	_raytrace_params;
	int					_line_index;
};

#endif


