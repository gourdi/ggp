#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_curve.h>

namespace ggo
{
  class duffing_animation_artist : public animation_artist_abc
  {
  public:

                duffing_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    void        init_animation() override;
    bool        prepare_frame() override;
    void        render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    std::unique_ptr<ggo::animation_artist_abc>  _artist;
  };
}

#endif
