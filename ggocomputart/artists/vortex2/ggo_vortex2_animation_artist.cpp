#include "ggo_vortex2_animation_artist.h"
#include <ggo_interpolation1d.h>

//////////////////////////////////////////////////////////////
ggo::vortex2_animation_artist::vortex2_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::vortex2_animation_artist::init()
{
  const int vortices_count = 20;

  _vortices_paths.clear();

  _params._color1 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color2 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color3 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color4 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._split_horz = ggo::rand<float>(0.4f * get_width(), 0.6f * get_width());
  _params._split_vert = ggo::rand<float>(0.4f * get_height(), 0.6f * get_height());

  _params._vortices.resize(vortices_count);
  for (auto & vortex : _params._vortices)
  {
    vortex._angle = ggo::rand<float>(ggo::pi<float>() / 2, ggo::pi<float>() / 2 + 0.5f);
    vortex._speed = ggo::rand<float>(0.004f, 0.008f) * get_max_size();
    vortex._power = ggo::rand<float>(6, 8);

    ggo::vortex2_animation_artist::vortex_path vortex_path;
    vortex_path._start_pos.get<0>() = ggo::rand<float>() * get_width();
    vortex_path._start_pos.get<1>() = ggo::rand<float>() * get_height();
    vortex_path._end_pos.get<0>() = ggo::rand<float>() * get_width();
    vortex_path._end_pos.get<1>() = ggo::rand<float>() * get_height();

    _vortices_paths[&vortex] = vortex_path;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::vortex2_animation_artist::update()
{
  const int frames_count = 250;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  // Interpolate vortices position.
  float interp = ggo::ease_inout_to<float>(_frame_index, frames_count);
  for (auto & vortex : _params._vortices)
  {
    const ggo::pos2f & start_pos = _vortices_paths[&vortex]._start_pos;
    const ggo::pos2f & end_pos = _vortices_paths[&vortex]._end_pos;

    vortex._pos = ggo::linear_interpolation(0.f, start_pos, 1.f, end_pos, interp);
  }

  return true;

}

//////////////////////////////////////////////////////////////
void ggo::vortex2_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping) const
{
  ggo::vortex2_artist::render(buffer, get_width(), get_height(), get_line_step(), ggo::rgb_8u_yu, _params);
}
