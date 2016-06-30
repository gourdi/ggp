#ifndef __GGO_ARTIST_KANJI__
#define __GGO_ARTIST_KANJI__

#include "ggo_artist_abc.h"
#include <vector>

class ggo_kanji_artist : public ggo_artist_abc
{
public:

        ggo_kanji_artist(int render_width, int render_height);

	bool	render_frame(uint8_t * buffer, int frame_index);
	void	init();
  void  init_output_buffer(uint8_t * buffer) const;
	
private:

	void	init_shaker();
	void	init_attractor();

private:

	struct ggo_particle
	{
		ggo::pos2f _prv_pos;
		ggo::pos2f _cur_pos;
	};

	std::vector<ggo_particle>	_particles;
	ggo::pos2f        			  _attractor;
	int							          _timer_max;
	ggo::color			          _parts_color;
	int							          _attractor_counter;
	int							          _shake_counter;
};

#endif
