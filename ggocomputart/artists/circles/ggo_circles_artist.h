#ifndef __GGO_CIRCLES_ARTIST__
#define __GGO_CIRCLES_ARTIST__

#include <ggo_artist.h>

namespace ggo
{
  class circles_artist
  {
  public:

    struct colored_disc
    {
      ggo::disc_float _disc;
      ggo::color_8u   _color;
    };

    static std::vector<std::vector<colored_disc>> generate_discs(int width, int height);

    static void paint_disc(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const ggo::pos2f & pos, float radius, const ggo::color_8u & color);
    static void paint_disc(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const colored_disc & colored_disc);
  };
}

#endif