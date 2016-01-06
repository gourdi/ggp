#ifndef __GGO_VORONOI_ARTIST__
#define __GGO_VORONOI_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_2d.h>

class ggo_voronoi_artist : public ggo_bitmap_artist_abc
{
public:
    
        ggo_voronoi_artist(int render_width, int render_height);
    
private:
    
  struct ggo_voronoi_point
  {
    ggo::point2d_float  _position;
    uint8_t				      _r;
    uint8_t				      _g;
    uint8_t				      _b;
  };
    
private:

	void	render_bitmap(uint8_t * buffer) override;

	void	fill_block(uint8_t * buffer, int x_from, int x_to, int y_from, int y_to, int r, int g, int b) const;
	void	process_block(uint8_t * buffer, int x_from, int x_to, int y_from, int y_to) const;
	void	sample(float x, float y, int & r, int & g, int & b) const;
	int		check_point(float x, float y) const;
    
private:

    std::vector<ggo_voronoi_point>  _points;
};

#endif

