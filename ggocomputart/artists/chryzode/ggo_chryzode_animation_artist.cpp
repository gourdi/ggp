#include "ggo_chryzode_animation_artist.h"
#include "ggo_chryzode_artist.h"

//////////////////////////////////////////////////////////////
ggo_chryzode_animation_artist::ggo_chryzode_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_chryzode_animation_artist::init_sub()
{
  const float DELTA = 1.001f;
  
  _multiplier1_start = ggo::rand_float(1 << 10, 1 << 16);
  _multiplier1_end = _multiplier1_start * ggo::rand_float(1 / DELTA, DELTA);
  _multiplier2_start = ggo::rand_float(1 << 10, 1 << 16);
  _multiplier2_end = _multiplier2_start * ggo::rand_float(1 / DELTA, DELTA);
  _offset1_start = ggo::rand_float(-127, 127);
  _offset1_end = _offset1_start * ggo::rand_float(1 / DELTA, DELTA);
  _offset2_start = ggo::rand_float(-127, 127);
  _offset2_end = _offset2_start * ggo::rand_float(1 / DELTA, DELTA);
  _modulo = (1 << 16);
  
  _radius_start = ggo::rand_float(0.2f, 0.35f) * get_render_min_size();
  _radius_end = ggo::rand_float(0.2f, 0.35f) * get_render_min_size();
  
  _hue_start = ggo::rand_float();
  _hue_end = _hue_start + ggo::rand_bool() ? -0.2f : 0.2f;
}

//////////////////////////////////////////////////////////////
bool ggo_chryzode_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  const int FRAMES_COUNT = 300;
  
  if (frame_index > FRAMES_COUNT)
  {
    return false;
  }

  ggo_chryzode_artist artist(get_render_width(), get_render_height());

  ggo_chryzode_params params;
  params._multiplier1 = ggo::ease_inout(frame_index, FRAMES_COUNT, _multiplier1_start, _multiplier1_end);
  params._multiplier2 = ggo::ease_inout(frame_index, FRAMES_COUNT, _multiplier2_start, _multiplier2_end);
  params._offset1 = ggo::ease_inout(frame_index, FRAMES_COUNT, _offset1_start, _offset1_end);
  params._offset2 = ggo::ease_inout(frame_index, FRAMES_COUNT, _offset2_start, _offset2_end);
  params._modulo_start = _modulo;
  params._modulo_end = _modulo + 1;
  
  float radius = ggo::ease_inout(frame_index, FRAMES_COUNT, _radius_start, _radius_end);
  
  artist.render_chryzode(buffer, radius, params, _hue_start, _hue_end);

  return true;
}
