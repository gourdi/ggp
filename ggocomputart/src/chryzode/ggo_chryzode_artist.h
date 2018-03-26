#ifndef __GGO_CHRYZODE_ARTIST__
#define __GGO_CHRYZODE_ARTIST__

#include <ggo_artist.h>

namespace ggo
{
  struct chryzode_params
  {
    float _multiplier1;
    float _multiplier2;
    float _offset1;
    float _offset2;
    int _modulo_start;
    int _modulo_end;
  };

  class chryzode_artist : public artist
  {
  public:

    chryzode_artist(int width, int height);

    void render_chryzode(void * buffer, int line_step, ggo::pixel_buffer_format pbf,
      float radius, const chryzode_params & params, float hue_start, float hue_end) const;
  };
}

#endif
