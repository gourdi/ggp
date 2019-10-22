#include "ggo_duffing_animation_artist.h"
#include "ggo_duffing.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/processing/ggo_blit.h>

namespace
{
  const int iterations_count = 200000;
  const int points_per_frame = 500;
  const int visible_points_count = 12000;
}

//////////////////////////////////////////////////////////////
ggo::duffing_animation_artist::duffing_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  // Compute points.
  ggo::duffing duffing;
  float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());

  _points.clear();
  _points.reserve(iterations_count);

  for (int i = 0; i < iterations_count; ++i)
  {
    ggo::pos2_f point = duffing.update(0.002f);

    point = ggo::rotate(point, angle_offset);
    point = map_fit(point, -1.7f, 1.7f);

    _points.push_back(point);

    angle_offset = std::fmod(angle_offset + 0.00005f, 2 * ggo::pi<float>());
  }

  // Init color mappings.
  _hue_curve.reset();
  _hue_curve.push_point(0, ggo::rand<float>());
  _hue_curve.push_point(1, ggo::rand<float>());
  _hue_curve.push_point(0.5, ggo::rand<float>());

  _sat_curve.reset();
  _sat_curve.push_point(0, ggo::rand<float>(0.5, 1));
  _sat_curve.push_point(1, ggo::rand<float>(0.5, 1));
  _sat_curve.push_point(0.5, ggo::rand<float>(0.5, 1));

  _val_curve1.reset();
  _val_curve1.push_point(0, ggo::rand<float>());
  _val_curve1.push_point(1, ggo::rand<float>());
  _val_curve1.push_point(0.25, ggo::rand<float>());
  _val_curve1.push_point(0.5, ggo::rand<float>());
  _val_curve1.push_point(0.75, ggo::rand<float>());

  _val_curve2.reset();
  _val_curve2.push_point(0, ggo::rand<float>());
  _val_curve2.push_point(1, ggo::rand<float>());
  _val_curve2.push_point(0.25, ggo::rand<float>());
  _val_curve2.push_point(0.5, ggo::rand<float>());
  _val_curve2.push_point(0.75, ggo::rand<float>());

  _val_curve3.reset();
  _val_curve3.push_point(0, ggo::rand<float>());
  _val_curve3.push_point(1, ggo::rand<float>());
  _val_curve3.push_point(0.25, ggo::rand<float>());
  _val_curve3.push_point(0.5, ggo::rand<float>());
  _val_curve3.push_point(0.75, ggo::rand<float>());

  _val_curve4.reset();
  _val_curve4.push_point(0, ggo::rand<float>());
  _val_curve4.push_point(1, ggo::rand<float>());
  _val_curve4.push_point(0.25, ggo::rand<float>());
  _val_curve4.push_point(0.5, ggo::rand<float>());
  _val_curve4.push_point(0.75, ggo::rand<float>());
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::render_frame(void * buffer, int frame_index, float time_step, bool & finished)
{
  int last_point = std::max(1, frame_index * points_per_frame);
  int first_point = last_point - visible_points_count;

  float radius = min_size() / 100.f;
  ggo::image_t<ggo::pixel_type::rgb_32f, ggo::lines_order::up> img_float(size());

  // Render the background.
  float t = float(points_per_frame) * frame_index / _points.size();

  float hue = _hue_curve.evaluate(t);
  ggo::rgb_32f color1 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve1.evaluate(t));
  ggo::rgb_32f color2 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve2.evaluate(t));
  ggo::rgb_32f color3 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve3.evaluate(t));
  ggo::rgb_32f color4 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve4.evaluate(t));

  ggo::fill_4_colors(img_float, color1, color2, color3, color4);

  // Render the shadow points.
  ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_shadow(size());
  fill_solid(img_shadow, 1.f);
  for (int i = first_point; i <= last_point; ++i)
  {
    if (i >= 0 && i < _points.size())
    {
      float opacity = 0.02f * (i - first_point) / visible_points_count;

      // Offset the shadow.
      ggo::pos2_f render_pt = _points[i];
      render_pt.x() += 0.05f * min_size();
      render_pt.y() += 0.05f * min_size();

      ggo::paint<ggo::sampling_2x2>(img_shadow, ggo::disc_f(render_pt, radius), 0.f, opacity);
    }
  }

  // Blur and blend the shadow.
  ggo::gaussian_blur(img_shadow, 0.4f * min_size());

  for (int y = 0; y < img_float.height(); ++y)
  {
    for (int x = 0; x < img_float.height(); ++x)
    {
      ggo::rgb_32f c = img_float.read_pixel(x, y);
      c *= img_shadow.read_pixel(x, y);
      img_float.write_pixel(x, y, c);
    }
  }

  // Render the points.
  float point_opacity = 0.02f;
  for (int i = first_point; i <= last_point; ++i)
  {
    if (i >= 0 && i < _points.size())
    {
      float opacity = 0.02f * (i - first_point) / visible_points_count;

      float t = float(i) / _points.size();
      float hue = _hue_curve.evaluate(t);
      float sat = _sat_curve.evaluate(t);
      ggo::rgb_32f color = ggo::from_hsv<ggo::rgb_32f>(hue, sat, 1);

      ggo::paint<ggo::sampling_4x4>(img_float, ggo::disc_f(_points[i], radius), color, opacity);
    }
  }

  // From float to uint8_t.
  ggo::blit(img_float, image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, size(), line_byte_step()));

  finished = first_point > iterations_count;
}
