#ifndef __GGO_RUBIKS_CUBE_ARTIST__
#define __GGO_RUBIKS_CUBE_ARTIST__

#include "ggo_raytracing_artist_abc.h"

class ggo_rubiks_cube_artist : public ggo_raytracing_artist_abc
{
public:

							ggo_rubiks_cube_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode);

private:

	virtual ggo_artist_id   get_id() const { return GGO_ARTIST_RUBIKS_CUBE; };
	virtual std::string		get_name() const { return "RubiksCube"; };
	
			void			create_cubes();
};

#endif


