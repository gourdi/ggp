#ifndef __GGO_CHRYZODE_ARTIST__
#define __GGO_CHRYZODE_ARTIST__

#include <ggo_artist_abc.h>

namespace ggo
{
  class gray_image_data_uint8;
  class pixel_sampler_abc;
}

struct ggo_chryzode_params
{
  float _multiplier1;
  float _multiplier2;
  float _offset1;
  float _offset2;
  int _modulo_start;
  int _modulo_end;
};

class ggo_chryzode_artist : public ggo_artist_abc
{
public:

                ggo_chryzode_artist(int width, int height);

          void  render_chryzode(uint8_t * buffer, float radius, const ggo_chryzode_params & params, float hue_start, float hue_end) const;
};

#endif
