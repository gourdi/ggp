#include "ggo_neon_artist.h"
#include <ggo_color.h>
#include <ggo_pbf_paint.h>
#include <ggo_pbf_fill.h>
#include <ggo_brush.h>
#include <ggo_blend.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf>
  void paint_point_t(const ggo::neon_artist & artist, void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color, const ggo::rect_int & clipping)
  {
    const float radius = 0.01f * artist.get_min_size();

    ggo::pos2f pos = artist.map_fit(0.8f * point_pos, -1, 1);

    ggo::disc_float disc1(pos, radius);
    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      disc1, ggo::make_solid_brush(color), ggo::add_blender<ggo::color_8u>(), clipping);

    pos.x() = artist.get_width() - pos.x() - 1;

    ggo::disc_float disc2(pos, radius);
    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      disc2, ggo::make_solid_brush(color), ggo::add_blender<ggo::color_8u>(), clipping);
  }
}

//////////////////////////////////////////////////////////////
ggo::neon_artist::neon_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::init_animation()
{
  _frame_index = -1;
  _angle = 0;
  _radius_prv = ggo::rand<float>();
  _radius_cur = _radius_prv;
  _radius_attractor = ggo::rand<float>();
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _attractor_color = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
}

//////////////////////////////////////////////////////////////
bool ggo::neon_artist::prepare_frame()
{
  ++_frame_index;

  const int frames_count = 1000;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  if ((_frame_index % 100) == 0)
  {
    _radius_attractor = ggo::rand<float>(0.2f, 1);
    _attractor_color = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
  }

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    const float velocity = _radius_cur - _radius_prv;
    const float force = 0.0000075f * ggo::sign(_radius_attractor - _radius_cur);
    _radius_prv = _radius_cur;
    _radius_cur += velocity + force;

    _points[substep] = ggo::from_polar(_angle, _radius_cur);
    _attractor_points[substep] = ggo::from_polar(_angle, _radius_attractor);

    _angle += 0.001f;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  if (_frame_index == 0)
  {
    switch (get_pixel_buffer_format())
    {
    case ggo::rgb_8u_yu:
      ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>(), clipping);
      break;
    case ggo::bgra_8u_yd:
      ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>(), clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    paint_point(buffer, _points[substep], ggo::color_8u(uint8_t(0x08), uint8_t(0x08), uint8_t(0x08)), clipping);

    if (substep & 1)
    {
      paint_point(buffer, _attractor_points[substep], _attractor_color, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color, const ggo::rect_int & clipping) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    paint_point_t<ggo::rgb_8u_yu>(*this, buffer, point_pos, color, clipping);
    break;
  case ggo::bgra_8u_yd:
    paint_point_t<ggo::bgra_8u_yd>(*this, buffer, point_pos, color, clipping);
    break;
  default:
    GGO_FAIL();
    break;
  }
}

