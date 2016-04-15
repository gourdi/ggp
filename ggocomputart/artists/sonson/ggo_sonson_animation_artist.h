#ifndef __GGO_SONSON_ANIMATION_ARTIST__
#define __GGO_SONSON_ANIMATION_ARTIST__

#include <ggo_gray_image_buffer.h>
#include <ggo_animation_artist_abc.h>
#include <ggo_curve.h>

class ggo_sonson_animation_artist : public ggo_animation_artist_abc
{
public:

              ggo_sonson_animation_artist(int render_width, int render_height);

  void        init_sub() override;
  bool        render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

  ggo::color  get_color(const int x, const int y) const;
  ggo::color  map_color(const uint8_t v) const;

private:

  ggo::gray_image_buffer_uint8    _mask;
  ggo::color                      _background_color;
  ggo::linear_curve<float, float> _hue_map;
  ggo::linear_curve<float, float> _sat_map;
  ggo::linear_curve<float, float> _val_map;

  ggo::point2d_float  _pos;
  ggo::vector2d_float _vel;
};

#endif
