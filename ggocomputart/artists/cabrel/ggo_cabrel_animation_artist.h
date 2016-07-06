#ifndef __GGO_CABREL_ANIMATION_ARTIST__
#define __GGO_CABREL_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_cabrel_curve.h"

class ggo_cabrel_animation_artist : public ggo_animation_artist_abc
{
public:

  ggo_cabrel_animation_artist(int render_width, int render_height);

private:

  void init_sub() override;
  bool render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

  std::unique_ptr<ggo_cabrel_curve<6>>  _curve;
  ggo::rgb_image_buffer_float           _image_float;
};

#endif
