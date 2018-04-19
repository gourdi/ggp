#ifndef __GGO_AGGREGATION_ANIMATION_ARTIST__
#define __GGO_AGGREGATION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_aggregation_artist.h"

namespace ggo
{
  class aggregation_animation_artist : public animation_artist_abc
  {
  public:

          aggregation_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    int _frame_index = 0;
    ggo::aggregation_artist _artist;
  };
}

#endif
