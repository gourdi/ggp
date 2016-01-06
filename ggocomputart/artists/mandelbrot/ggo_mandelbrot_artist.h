#ifndef __GGO_MANDELBROT_ARTIST__
#define __GGO_MANDELBROT_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <complex>
#include <ggo_array.h>

class ggo_mandelbrot_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_mandelbrot_artist(int render_width, int render_height);
	
private:
	
	void	render_bitmap(uint8_t * buffer) override;
	
	void	setup_palette();
	void	setup_block();
			
	int		iterate(double x, double y) const;
	
private:
	
	struct ggo_rgb
	{
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
	};
	
	std::complex<double>	_center;
	double					      _range;				
	ggo::array<ggo_rgb>		_palette;
};

#endif