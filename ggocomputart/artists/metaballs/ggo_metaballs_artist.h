// Reference: http://www.geisswerks.com/ryan/BLOBS/blobs.html

#ifndef __GGO_METABALLS_ARTIST__
#define __GGO_METABALLS_ARTIST__

#include "ggo_artist_abc.h"
#include "ggo_raytracer_global.h"
#include "ggo_renderer_abc.h"

class ggo_camera_abc;

class ggo_metaballs_artist : public ggo_artist_abc
{
public:
	
	struct ggo_metaballs_params
	{
    ggo_metaballs_params();
    
		std::vector<ggo::point3d_float>	_centers;
		float 							            _threshold;
		float					                  _phong_factor;
		float					                  _phong_shininess;
		ggo::point3d_float				      _light1;
		ggo::point3d_float				      _light2;
    ggo::color                      _background_color;
	};
	
public:

        ggo_metaballs_artist(int render_width, int render_height);

	void 	render_bitmap(uint8_t * buffer, ggo::renderer_abc & renderer, const ggo_metaballs_params & params);
};

#endif

