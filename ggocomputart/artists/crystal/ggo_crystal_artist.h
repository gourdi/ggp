#ifndef __GGO_CRYSTAL_ARTIST__
#define __GGO_CRYSTAL_ARTIST__

#include "ggo_artist_abc.h"
#include <ggo_array.h>

#define GGO_TRANSFORMS_COUNT 6

class ggo_crystal_artist : public ggo_artist_abc
{
public:
	
	struct ggo_params
	{
		float	_coefs[GGO_TRANSFORMS_COUNT][6];
		float	_dr[GGO_TRANSFORMS_COUNT], _dg[GGO_TRANSFORMS_COUNT], _db[GGO_TRANSFORMS_COUNT];
	};
	
public:

                ggo_crystal_artist(int render_width, int render_height);
					
          void	render_bitmap(uint8_t * buffer, const ggo_params & params, const ggo::color bkgd_colors[4]);
					
	static	void	randomize_params(ggo_params & params);

private:

          void	process_transform(const ggo_params & params);
			
private:
			
	ggo::array_float	_render_buffer;
};

#endif
