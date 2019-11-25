#include "ggo_neon_realtime_artist.h"
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::neon_realtime_artist::neon_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
:
realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _substeps_per_frame = 800.f * static_cast<float>(fps._den) / static_cast<float>(fps._num);
  _angle = 0;
  _radius_prv = ggo::rand<float>();
  _radius_cur = _radius_prv;
  _radius_attractor = ggo::rand<float>();
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _attractor_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
}

//////////////////////////////////////////////////////////////
void ggo::neon_realtime_artist::preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  if (_substeps_count == 0)
  {
    memset(buffer, 0, line_byte_step() * height());
  }

  _substeps += _substeps_per_frame;

  _attractor_points.clear();
  _points.clear();
  for (; _substeps >= 1.f; _substeps -= 1.f, ++_substeps_count)
  {
    const float velocity = _radius_cur - _radius_prv;
    const float force = 0.0000075f * ggo::sign(_radius_attractor - _radius_cur);
    _radius_prv = _radius_cur;
    _radius_cur += velocity + force;

    _points.push_back(_radius_cur * ggo::vec2_f::from_angle(_angle));
    _attractor_points.push_back(_radius_attractor * ggo::vec2_f::from_angle(_angle));

    _angle += 0.0025f;

    if ((_substeps_count % 1000) == 0)
    {
      _radius_attractor = ggo::rand<float>(0.2f, 1);
      _attractor_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), 1.0f, 8.f / 255.f);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::neon_realtime_artist::render_tile(void * buffer, const ggo::rect_int & clipping)
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
bool ggo::neon_realtime_artist::finished()
{
  return _substeps_count > 8000;
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::neon_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  for (const auto & p : _points)
  {
    paint_point_t(img, p, ggo::rgb_8u(uint8_t(0x08), uint8_t(0x08), uint8_t(0x08)), clipping);
  }

  for (const auto & p : _attractor_points)
  {
    paint_point_t(img, p, _attractor_color, clipping);
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


