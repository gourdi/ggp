#ifndef __GGO_KANJI_ANIMATION_ARTIST__
#define __GGO_KANJI_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_kanji_artist.h"

namespace ggo
{
  class kanji_animation_artist : public accumulation_animation_artist_abc
  {
  public:

    kanji_animation_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    void  init_output_buffer(void * buffer) const override;
    bool	render_next_frame_acc(void * buffer, int frame_index) override;

  private:

    ggo::kanji_artist	_kanji_artist;
  };
}

#endif
