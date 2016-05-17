#ifndef __GGO_JULIA_ARTIST__
#define __GGO_JULIA_ARTIST__

#include "ggo_artist_abc.h"
#include <complex>
#include <ggo_array.h>

class ggo_julia_artist : public ggo_artist_abc
{
public:
	
                              ggo_julia_artist(int render_width, int render_height);
									
          void					      render_bitmap(uint8_t * buffer, const std::complex<float> & seed, float range = 1.7);
			
	static	std::complex<float> pickup_seed();
	
private:

          int						      iterate(float x, float y, const std::complex<float> & seed) const;
	
private:
	
	struct ggo_rgb
	{
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
	};

	ggo::array<ggo_rgb, 1>	_palette;
};

#endif