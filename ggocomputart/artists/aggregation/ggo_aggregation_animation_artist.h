#ifndef __GGO_AGGREGATION_ANIMATION_ARTIST__
#define __GGO_AGGREGATION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <memory>
#include "ggo_aggregation_artist.h"

class ggo_aggregation_animation_artist : public ggo_animation_artist_abc
{
public:

        ggo_aggregation_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

  std::unique_ptr<ggo::aggregation_artist> _artist;
};

#endif
