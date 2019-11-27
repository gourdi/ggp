#ifndef __GGO_POUPETTE_ARTIST__
#define __GGO_POUPETTE_ARTIST__

#include <kernel/ggo_rect_int.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <2d/ggo_color.h>
#include <2d/ggo_pixel_type.h>

namespace ggo
{
  class poupette_artist
  {
  public:

    poupette_artist(float color_max);

    void render_tile(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, 
                     float progress, const ggo::rect_int & clipping, ggo::pixel_sampling sampling) const;

    std::array<ggo::rgb_32f, 4> interpolate_colors(float progress) const;

  private:

    ggo::rgb_32f _c1_start;
    ggo::rgb_32f _c2_start;
    ggo::rgb_32f _c3_start;
    ggo::rgb_32f _c4_start;
    ggo::rgb_32f _c1_end;
    ggo::rgb_32f _c2_end;
    ggo::rgb_32f _c3_end;
    ggo::rgb_32f _c4_end;
  };
}

#endif