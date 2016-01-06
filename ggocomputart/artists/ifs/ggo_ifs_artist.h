#ifndef __GGO_IFS_ARTIST__
#define __GGO_IFS_ARTIST__

#include "ggo_artist_abc.h"
#include <ggo_array.h>
#include <ggo_set3.h>

class ggo_ifs_artist : public ggo_artist_abc
{
public:

			ggo_ifs_artist(int render_width, int render_height);

	void	render(uint8_t * buffer, float transform[4], float hue, float angle1, float angle2);

private:

	void	update(ggo::point3d_float & point, const float transform[4]);
			
	void	paint_buffer(uint8_t * buffer, uint8_t color, const ggo::array_float & accumul_buffer) const;
};

#endif


