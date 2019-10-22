#ifndef __GGO_POUPETTE_ARTIST__
#define __GGO_POUPETTE_ARTIST__

#include <kernel/ggo_rect_int.h>
#include <2d/ggo_color.h>
#include <2d/ggo_pixel_type.h>
#include <2d/paint/ggo_pixel_sampling.h>

namespace ggo
{
  class poupette_artist
  {
  public:

    static constexpr int _frames_count = 300;

    poupette_artist();

    void render_tile(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, 
                     int frame_index, const ggo::rect_int & clipping, ggo::pixel_sampling sampling) const;
    bool finished(int frame_index) const;

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