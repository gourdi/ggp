#include "ggo_chryzode_animation_artist.h"
#include "ggo_chryzode_artist.h"

//////////////////////////////////////////////////////////////
ggo::chryzode_animation_artist::chryzode_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
animation_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::chryzode_animation_artist::init_sub()
{
  const float delta = 1.001f;
  
  _multiplier1_start = ggo::rand<float>(1 << 10, 1 << 16);
  _multiplier1_end = _multiplier1_start * ggo::rand<float>(1 / delta, delta);
  _multiplier2_start = ggo::rand<float>(1 << 10, 1 << 16);
  _multiplier2_end = _multiplier2_start * ggo::rand<float>(1 / delta, delta);
  _offset1_start = ggo::rand<float>(-127, 127);
  _offset1_end = _offset1_start * ggo::rand<float>(1 / delta, delta);
  _offset2_start = ggo::rand<float>(-127, 127);
  _offset2_end = _offset2_start * ggo::rand<float>(1 / delta, delta);
  _modulo = (1 << 16);
  
  _radius_start = ggo::rand<float>(0.2f, 0.35f) * get_min_size();
  _radius_end = ggo::rand<float>(0.2f, 0.35f) * get_min_size();
  
  _hue_start = ggo::rand<float>();
  _hue_end = _hue_start + (ggo::rand_bool() ? -0.2f : 0.2f);
}

//////////////////////////////////////////////////////////////
bool ggo::chryzode_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 300;
  
  if (frame_index > frames_count)
  {
    return false;
  }

  chryzode_artist artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format());

  chryzode_params params;
  params._multiplier1 = ggo::ease_inout(frame_index, frames_count, _multiplier1_start, _multiplier1_end);
  params._multiplier2 = ggo::ease_inout(frame_index, frames_count, _multiplier2_start, _multiplier2_end);
  params._offset1 = ggo::ease_inout(frame_index, frames_count, _offset1_start, _offset1_end);
  params._offset2 = ggo::ease_inout(frame_index, frames_count, _offset2_start, _offset2_end);
  params._modulo_start = _modulo;
  params._modulo_end = _modulo + 1;
  
  float radius = ggo::ease_inout(frame_index, frames_count, _radius_start, _radius_end);
  
  artist.render_chryzode(buffer, radius, params, _hue_start, _hue_end);

  return true;
}
