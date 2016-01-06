#ifndef __GGO_DELAUNAY_ARTIST__
#define __GGO_DELAUNAY_ARTIST__

#include "ggo_artist_abc.h"
#include <ggo_array.h>
#include <ggo_math.h>
#include <ggo_2d.h>
#include <vector>
#include <list>

/*class ggo_delaunay_artist : public ggo_artist_abc
{
public:

									ggo_delaunay_artist(int render_width, int render_height);
	virtual		 				   ~ggo_delaunay_artist();

	virtual	void					update(ggo_user_feedback_abc * feedback);
	virtual	void					render(uint8_t * buffer);
	virtual	bool					finished() const;
	virtual	ggo_artist_id			get_id() const { return ( GGO_ARTIST_DELAUNAY ); };
	virtual	void					randomize_sub(int render_width, int render_height);

private:

			ggo_oriented_box_float	get_render_edge(const ggo_segment_float & edge) const;


private:

	struct ggo_color_triangle
	{
		ggo_color			color;
		ggo_triangle<float>	triangle;
	};
	std::vector<ggo_color_triangle>	_triangles;
	std::vector<ggo_segment_float>	_edges;
};*/

#endif


