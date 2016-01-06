#ifndef __GGO_CELLS_ARTIST__
#define __GGO_CELLS_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_paint.h>

class ggo_cells_artist : public ggo_bitmap_artist_abc
{
public:

			ggo_cells_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;

	float	compute_potential(const std::vector<ggo::point2d_float> & centers, int x, int y) const;
	bool	paint_point(uint8_t * buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, const std::vector<ggo::point2d_float> & centers) const;

private:

	class ggo_cell : public ggo::seed_paintable_shape2d_abc<float>
	{
	public:
		
                          ggo_cell(float left, float right, float top, float bottom, int render_width, int render_height);
		
		ggo::rect_data<float> get_seed_rect() const override;
		bool                  is_point_inside(float x, float y) const override;
		
	private:
		
		ggo::point2d_float	_centers[3];
		float				        _inv_render_width;
		float				        _inv_render_height;
	};
	
	struct ggo_polynom
	{
		float	_deg2;
		float	_deg1;
		float	_deg0;
	};
};

#endif
