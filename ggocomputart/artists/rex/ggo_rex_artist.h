#ifndef __GGO_REX_ARTIST__
#define __GGO_REX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_io.h>

class ggo_rex_artist : public ggo_bitmap_artist_abc
{
public:
	
			ggo_rex_artist(int render_width, int render_height);
	
private:

	void	render_bitmap(uint8_t * buffer) override;
	
	void	render_color_triangles(uint8_t * buffer) const;
	void	render_patterns(uint8_t * buffer) const;
	void	render_edges(uint8_t * buffer) const;
	void	render_clipped_discs(uint8_t * buffer) const;
	
	void	init();
	void	clear();
	
private:

	struct ggo_color_triangle
	{
		ggo::point2d_float  _v1, _v2, _v3;
		ggo::color			    _color1, _color2, _color3;
	};
	
	struct ggo_pattern_triangle
	{
		ggo::point2d_float	_v1, _v2, _v3;
		float				        _delta;
		float				        _radius;
	};
	
	struct ggo_disc_clip_triangle
	{
		ggo::point2d_float	_v1, _v2, _v3;	
	};

  struct ggo_opened_disc_data
  {
    ggo::point2d_float  _center;
    float               _radius;
    float               _width;
  };

	std::vector<ggo::segment_float>		    _edges;
	std::vector<ggo_color_triangle>		    _color_triangles;
	std::vector<ggo_pattern_triangle>	    _pattern_triangles;
	std::vector<ggo_disc_clip_triangle>	  _discs_clip_triangles;
	std::vector<ggo_opened_disc_data>     _opened_discs;
};

#endif
