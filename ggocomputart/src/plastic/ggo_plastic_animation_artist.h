#ifndef __GGO_PLASTIC_ANIMATION_ARTIST__
#define __GGO_PLASTIC_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include "ggo_plastic_artist.h"

namespace ggo
{
  class plastic_animation_artist : public progress_animation_artist
  {
  public:

          plastic_animation_artist(int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_frame_t(void * buffer, float progress) const;

    float compute_altitude(float progress, float x, float y) const;

  private:

    plastic_artist _artist;
  };
}

#endif
