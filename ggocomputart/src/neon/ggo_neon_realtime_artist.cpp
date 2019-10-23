#include "ggo_neon_realtime_artist.h"
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::neon_realtime_artist::neon_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_realtime_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _angle = 0;
  _radius_prv = ggo::rand<float>();
  _radius_cur = _radius_prv;
  _radius_attractor = ggo::rand<float>();
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _attractor_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
}

//////////////////////////////////////////////////////////////
void ggo::neon_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
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

    _points[substep] = _radius_cur * ggo::vec2_f::from_angle(_angle);
    _attractor_points[substep] = _radius_attractor * ggo::vec2_f::from_angle(_angle);

    _angle += 0.001f;
  }
}

//////////////////////////////////////////////////////////////
void ggo::neon_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, frame_index, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, frame_index, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, frame_index, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::neon_realtime_artist::render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  if (frame_index == 0)
  {
    ggo::fill_black(img, clipping);
  }

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    paint_point_t(img, _points[substep], ggo::rgb_8u(uint8_t(0x08), uint8_t(0x08), uint8_t(0x08)), clipping);

    if (substep & 1)
    {
      paint_point_t(img, _attractor_points[substep], _attractor_color, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type_, ggo::lines_order memory_lines_order_>
void ggo::neon_realtime_artist::paint_point_t(image_t<pixel_type_, memory_lines_order_> & img, const ggo::pos2_f & point_pos, const ggo::rgb_8u & color, const ggo::rect_int & clipping) const
{
  const float radius = 0.01f * min_size();

  ggo::pos2_f pos = map_fit(0.8f * point_pos, -1, 1);

  ggo::disc_f disc1(pos, radius);
  ggo::paint<ggo::sampling_4x4>(img, disc1, ggo::make_solid_brush(color), ggo::add_blender<ggo::rgb_8u>(), clipping);

  pos.x() = width() - pos.x() - 1;

  ggo::disc_f disc2(pos, radius);
  ggo::paint<ggo::sampling_4x4>(img, disc2, ggo::make_solid_brush(color), ggo::add_blender<ggo::rgb_8u>(), clipping);
}


