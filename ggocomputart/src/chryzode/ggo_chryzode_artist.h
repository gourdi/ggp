#ifndef __GGO_CHRYZODE_ARTIST__
#define __GGO_CHRYZODE_ARTIST__

#include <ggo_artist.h>
#include <2d/ggo_pixel_type.h>

namespace ggo
{
  struct chryzode_params
  {
    float _multiplier1 = 0.f;
    float _multiplier2 = 0.f;
    float _offset1 = 0.f;
    float _offset2 = 0.f;
    int _modulo_start = 0;
    int _modulo_end = 0;
  };

  class chryzode_artist : public artist
  {
  public:

    chryzode_artist(int width, int height);

    void render_chryzode(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      float radius, const chryzode_params & params, float hue_start, float hue_end) const;
  };
}

#endif
