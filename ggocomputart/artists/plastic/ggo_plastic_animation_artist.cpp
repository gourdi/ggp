#include "ggo_plastic_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::plastic_animation_artist::plastic_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::plastic_animation_artist::init()
{
  _frame_index = -1;

  _params.clear();

  for (int i = 0; i < 50; ++i)
  {
    anim_plastic_params params;

    params._center_x = ggo::rand<float>(-1.5f, 1.5f);
    params._center_y = ggo::rand<float>(-1.5f, 1.5f);
    params._radius = ggo::rand<float>(0.1f, 0.5f);
    params._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    params._dangle = ggo::rand<float>(0.02f, 0.08f);
    params._den = ggo::rand<float>(0.5f, 2.f);
    params._mult = ggo::rand<float>(0.5f, 2.f);
    params._power = ggo::rand<float>(2.f, 3.f);

    _params.push_back(params);
  }

  _color = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(), 1);
  _altitude_factor = ggo::rand<float>(0.04f, 0.08f);
}

//////////////////////////////////////////////////////////////
bool ggo::plastic_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > 300)
  {
    return false;
  }

  for (auto & anim_params : _params)
  {
    anim_params._angle += anim_params._dangle;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::plastic_animation_artist::process_frame(void * buffer, const ggo::rect_int & clipping)
{
  std::vector<ggo::plastic_artist::params> bitmap_params;
  for (auto & anim_params : _params)
  {
    ggo::plastic_artist::params	params;
    params._pos_x = anim_params._center_x + anim_params._radius * std::cos(anim_params._angle);
    params._pos_y = anim_params._center_y + anim_params._radius * std::sin(anim_params._angle);
    params._den = anim_params._den;
    params._mult = anim_params._mult;
    params._power = anim_params._power;

    bitmap_params.push_back(params);
  }

  ggo::plastic_artist artist(get_width(), get_height());
  artist.render(buffer, get_line_step(), get_pixel_buffer_format(), bitmap_params, _color, _altitude_factor);
}

