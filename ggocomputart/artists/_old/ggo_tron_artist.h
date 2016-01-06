#ifndef __GGO_TRON_ARTIST__
#define __GGO_TRON_ARTIST__

#include "ggo_artist_abc.h"
#include <ggo_array.h>
#include <ggo_grid.h>
#include <ggo_2d.h>

class ggo_tron_artist : public ggo_artist_abc
{
public:

							ggo_tron_artist(int render_width, int render_height);

private:

	struct ggo_tron
	{
		int		_pos_x;
		int	 	_pos_y;
		int		_dx;
		int		_dy;
		float	_hue;
		float	_sat;
		float	_delta_hue;
	};

private:

	virtual	void			update_sub(int counter, std::ostringstream & oss);
	virtual	void			render_sub(uint8_t * buffer);
	virtual	void			init_sub();
	virtual	void			render_bkgd(uint8_t * buffer);
	virtual	bool			is_finished(int counter) const;
	virtual	ggo_artist_id	get_id() const { return GGO_ARTIST_TRON; };
	virtual	std::string		get_name() const { return "Tron"; };

			bool			is_finished() const;
			void			circular_clamp(int & pos_x, int & pos_y) const;
			bool			turn_tron(ggo_tron & tron) const;

private:

	ggo_grid<int>		_grid;
	ggo_array<ggo_tron>	_trons;
	int					_counter;
};

#endif
