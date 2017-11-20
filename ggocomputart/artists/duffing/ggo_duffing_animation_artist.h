#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_array.h>
#include <ggo_curve.h>

namespace ggo
{
  class duffing_animation_artist : public animation_artist_abc
  {
  public:

                duffing_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void        init() override;
    bool        prepare_frame() override;
    void        process_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    std::unique_ptr<ggo::animation_artist_abc>  _artist;
  };
}

#endif
