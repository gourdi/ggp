#include "ggo_neon_artist.h"
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void paint_point_t(const ggo::neon_artist & artist, void * buffer, const ggo::pos2f & point_pos, const ggo::rgb_8u & color, const ggo::rect_int & clipping)
  {
    const float radius = 0.01f * artist.min_size();

    ggo::pos2f pos = artist.map_fit(0.8f * point_pos, -1, 1);

    ggo::disc_float disc1(pos, radius);
    ggo::paint<format, ggo::sampling_4x4>(
      buffer, artist.width(), artist.height(), artist.line_step(),
      disc1, ggo::make_solid_brush(color), ggo::add_blender<ggo::rgb_8u>(), clipping);

    pos.x() = artist.width() - pos.x() - 1;

    ggo::disc_float disc2(pos, radius);
    ggo::paint<format, ggo::sampling_4x4>(
      buffer, artist.width(), artist.height(), artist.line_step(),
      disc2, ggo::make_solid_brush(color), ggo::add_blender<ggo::rgb_8u>(), clipping);
  }
}

//////////////////////////////////////////////////////////////
ggo::neon_artist::neon_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_realtime_artist_abc(width, height, line_step, format)
{
  _angle = 0;
  _radius_prv = ggo::rand<float>();
  _radius_cur = _radius_prv;
  _radius_attractor = ggo::rand<float>();
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _attractor_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2i cursor_pos)
{
  if ((frame_index % 100) == 0)
  {
    _radius_attractor = ggo::rand<float>(0.2f, 1);
    _attractor_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
  }

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    const float velocity = _radius_cur - _radius_prv;
    const float force = 0.0000075f * ggo::sign(_radius_attractor - _radius_cur);
    _radius_prv = _radius_cur;
    _radius_cur += velocity + force;

    _points[substep] = _radius_cur * ggo::vec2f::from_angle(_angle);
    _attractor_points[substep] = _radius_attractor * ggo::vec2f::from_angle(_angle);

    _angle += 0.001f;
  }
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  if (frame_index == 0)
  {
    switch (format())
    {
    case ggo::rgb_8u_yu:
      ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(), ggo::black_8u(), clipping);
      break;
    case ggo::bgrx_8u_yd:
      ggo::fill_solid<ggo::bgrx_8u_yd>(buffer, width(), height(), line_step(), ggo::black_8u(), clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    paint_point(buffer, _points[substep], ggo::rgb_8u(uint8_t(0x08), uint8_t(0x08), uint8_t(0x08)), clipping);

    if (substep & 1)
    {
      paint_point(buffer, _attractor_points[substep], _attractor_color, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::rgb_8u & color, const ggo::rect_int & clipping) const
{
  switch (format())
  {
  case ggo::rgb_8u_yu:
    paint_point_t<ggo::rgb_8u_yu>(*this, buffer, point_pos, color, clipping);
    break;
  case ggo::bgrx_8u_yd:
    paint_point_t<ggo::bgrx_8u_yd>(*this, buffer, point_pos, color, clipping);
    break;
  default:
    GGO_FAIL();
    break;
  }
}

