#ifndef __GGO_REDIFF_ANIMPATION_ARTIST__
#define __GGO_REDIFF_ANIMPATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_gray_image_buffer.h>

class ggo_rediff_animation_artist : public ggo_accumulation_animation_artist_abc
{
public:

        ggo_rediff_animation_artist(int render_width, int render_height);

private:

  void	init_sub() override;
  void	init_output_buffer(uint8_t * buffer) override;
  bool	render_next_frame_acc(uint8_t * buffer, int frame_index) override;

  void  diffuse(ggo::gray_image_buffer_float & food_output, ggo::gray_image_buffer_float & life_output) const;
  void  update();
  void  render(ggo::rgb_image_abc & image) const;

private:

  float                 _hue;
  ggo::array<float, 2>  _feed_map;
  ggo::array<float, 2>  _kill_map;
  float                 _food_stddev;
  float                 _life_stddev;
  ggo::array<float, 2>  _food;
  ggo::array<float, 2>  _life;
};

#endif
