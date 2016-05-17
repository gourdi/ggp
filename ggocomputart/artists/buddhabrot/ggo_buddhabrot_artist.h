#ifndef __GGO_BUDDHABROT_ARTIST__
#define __GGO_BUDDHABROT_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <complex>
#include <ggo_array.h>

class ggo_buddhabrot_artist : public ggo_bitmap_artist_abc
{
public:
    
                ggo_buddhabrot_artist(int render_width, int render_height);   

private:
   
          void	render_bitmap(uint8_t * buffer) override;
	
	static	bool	discard_point(const std::complex<double> & p);
	static	bool	iterate(const std::complex<double> & p, int escape_threshold);
	static	bool	iterate(const std::complex<double> & p, std::vector<std::complex<double> > & points, int escape_threshold);
          void	setup_seed_area();
          void	process(int escape_threshold, ggo::array<int, 2> & accumulation) const;

private:
	
	int						        _escape_threshold_r;
	int						        _escape_threshold_g;
	int						        _escape_threshold_b;
	ggo::array<int, 2>	  _accumulation_r;
	ggo::array<int, 2>	  _accumulation_g;
	ggo::array<int, 2>	  _accumulation_b;
	std::complex<double>	_center;
	double					      _range;
};

#endif

