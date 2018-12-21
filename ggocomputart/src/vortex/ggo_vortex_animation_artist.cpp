#include "ggo_vortex_animation_artist.h"
#include <kernel/math/interpolation/ggo_interpolation1d.h>

//////////////////////////////////////////////////////////////
ggo::vortex_animation_artist::vortex_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, 250)
{
  const int vortices_count = 20;

  _params._color1 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color2 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color3 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._color4 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
  _params._split_horz = ggo::rand<float>(0.4f * width, 0.6f * width);
  _params._split_vert = ggo::rand<float>(0.4f * height, 0.6f * height);

  _params._vortices = ggo::array<vortex_artist::vortex, 1>(vortices_count);
  for (auto & vortex : _params._vortices)
  {
    vortex._angle = ggo::rand<float>(ggo::pi<float>() / 2, ggo::pi<float>() / 2 + 0.5f);
    vortex._speed = ggo::rand<float>(0.004f, 0.008f) * max_size();
    vortex._power = ggo::rand<float>(6, 8);

    ggo::vortex_animation_artist::vortex_path vortex_path;
    vortex_path._start_pos.x() = ggo::rand<float>() * width;
    vortex_path._start_pos.y() = ggo::rand<float>() * height;
    vortex_path._end_pos.x() = ggo::rand<float>() * width;
    vortex_path._end_pos.y() = ggo::rand<float>() * height;

    _vortices_paths[&vortex] = vortex_path;
  }
}

//////////////////////////////////////////////////////////////
void ggo::vortex_animation_artist::render_frame(void * buffer, int frame_index)
{
  // Interpolate vortices position.
  float interp = ggo::ease_inout_to<float>(frame_index, frames_count());
  for (auto & vortex : _params._vortices)
  {
    const ggo::pos2_f & start_pos = _vortices_paths[&vortex]._start_pos;
    const ggo::pos2_f & end_pos = _vortices_paths[&vortex]._end_pos;

    vortex._pos = ggo::linear_interpolation(0.f, start_pos, 1.f, end_pos, interp);
  }

  ggo::vortex_artist::render(buffer, width(), height(), line_step(), ggo::rgb_8u_yu, _params);
}
