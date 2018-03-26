#ifndef __GGO_KANJI_ANIMATION_ARTIST__
#define __GGO_KANJI_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_kanji_artist.h"

namespace ggo
{
  class kanji_animation_artist : public animation_artist_abc
  {
  public:

          kanji_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    ggo::kanji_artist	_kanji_artist;
    int               _frame_index;
  };
}

#endif