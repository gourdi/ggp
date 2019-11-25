#include "ggo_duffing_animation_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/processing/ggo_blit.h>
#include <2d/processing/ggo_mask.h>

namespace
{
  constexpr ggo::ratio duration{ 8, 1 };
}

//////////////////////////////////////////////////////////////
ggo::duffing_animation_artist::duffing_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order),
_fps(fps)
{
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _paint_points_per_frame = to<float>(2500 / fps);
  _opacity_start = 0.25f;
  _opacity_decay = 0.25f * to<float>(1 / fps);

  // Init color mappings.
  float hue = ggo::rand<float>();
  _point_color = ggo::from_hsv<ggo::rgb_8u>(hue, 1.f, 1.f);
  _bkgd_color1 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.5f, 1.f), ggo::rand<float>());
  _bkgd_color2 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.5f, 1.f), ggo::rand<float>());
  _bkgd_color3 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.5f, 1.f), ggo::rand<float>());
  _bkgd_color4 = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.5f, 1.f), ggo::rand<float>());
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::render_frame(void * buffer, bool & finished)
{
  if (_elapsed_time > duration)
  {
    _paint_points_per_frame -= to<float>(10 / _fps);
  }

  if (_paint_points_per_frame > 0)
  {
    _paint_points_to_create += _paint_points_per_frame;
    for (; _paint_points_to_create >= 1.f; _paint_points_to_create -= 1.f)
    {
      ggo::pos2_f point = _duffing.update(0.01f);

      point = ggo::rotate(point, _angle);
      point = map_fit(point, -1.7f, 1.7f);

      _paint_points.push_back({ point, _opacity_start });

      _angle += 0.00025f;
    }
  }

  float radius = min_size() / 100.f;
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size());

  // Render the background.
  ggo::fill_4_colors(img, _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4);

  // Render the shadow points.
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> img_shadow(size());
  fill_solid(img_shadow, 0x00);
  for (const auto & p : _paint_points)
  {
    // Offset the shadow.
    ggo::pos2_f render_pt = p._pos;
    render_pt.x() += 0.05f * min_size();
    render_pt.y() -= 0.05f * min_size();

    ggo::paint<ggo::sampling_2x2>(img_shadow, ggo::disc_f(render_pt, radius), 0xff_u8, p._opacity);
  }

  // Blur and blend the shadow.
  ggo::gaussian_blur(img_shadow, 0.01f * min_size());
  ggo::apply_mask(img, img_shadow, [](int x, int y) { return ggo::black_8u(); });
  
  // Render the points.
  for (const auto & p : _paint_points)
  {
    ggo::paint<ggo::sampling_4x4>(img, ggo::disc_f(p._pos, radius), _point_color, p._opacity);
  }

  // Opacity decay.
  for (auto & p : _paint_points)
  {
    p._opacity -= _opacity_decay;
  }
  ggo::remove_if(_paint_points, [](const paint_point & p) { return p._opacity <= 0.f; });

  _elapsed_time += 1 / _fps;
  finished = _paint_points.empty();
}
