#ifndef __GGO_CRYSTAL_ANIMATION_ARTIST__
#define __GGO_CRYSTAL_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_crystal_artist.h"

namespace ggo
{
  class crystal_animation_artist : public animation_artist_abc
  {
  public:

    crystal_animation_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    ggo::crystal_artist	        _artist;
    ggo::color_8u   		        _bkgd_colors[4];
    ggo::crystal_artist::params _start_params;
    ggo::crystal_artist::params _end_params;
  };
}

#endif
