#include "ggo_duffing_animation_artist.h"
#include "ggo_duffing.h"
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/ggo_blit.h>

namespace
{
  const int iterations_count = 200000;
  const int points_per_frame = 500;
  const int visible_points_count = 12000;
}

//////////////////////////////////////////////////////////////
ggo::duffing_animation_artist::duffing_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
animation_artist_abc(width, height, line_step, format)
{
  // Compute points.
  ggo::duffing duffing;
  float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());

  _points.clear();
  _points.reserve(iterations_count);

  for (int i = 0; i < iterations_count; ++i)
  {
    ggo::pos2f point = duffing.update(0.002f);

    float angle = atan2(point.y(), point.x()) + angle_offset;
    float dist = point.get_length();

    point.set(dist * std::cos(angle), dist * std::sin(angle));
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
void ggo::duffing_animation_artist::render_frame(void * buffer, int frame_index, bool & finished)
{
  int last_point = std::max(1, frame_index * points_per_frame);
  int first_point = last_point - visible_points_count;

  float radius = min_size() / 100.f;
  std::vector<float> buffer_float(3 * width() * height());

  // Render the background.
  float t = float(points_per_frame) * frame_index / _points.size();

  float hue = _hue_curve.evaluate(t);
  ggo::rgb_32f color1 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve1.evaluate(t));
  ggo::rgb_32f color2 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve2.evaluate(t));
  ggo::rgb_32f color3 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve3.evaluate(t));
  ggo::rgb_32f color4 = ggo::from_hsv<ggo::rgb_32f>(hue, 0.2f, _val_curve4.evaluate(t));

  ggo::fill_4_colors<ggo::rgb_32f_yu>(buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
    color1, color2, color3, color4, ggo::rect_int::from_width_height(width(), height()));

  // Render the shadow points.
  std::vector<float> shadow_buffer(width() * height(), 1.f);
  for (int i = first_point; i <= last_point; ++i)
  {
    if (i >= 0 && i < _points.size())
    {
      float opacity = 0.02f * (i - first_point) / visible_points_count;

      // Offset the shadow.
      ggo::pos2f render_pt = _points[i];
      render_pt.x() += 0.05f * min_size();
      render_pt.y() += 0.05f * min_size();

      ggo::paint<ggo::y_32f_yu, ggo::sampling_2x2>(
        shadow_buffer.data(), width(), height(), sizeof(float) * width(),
        ggo::disc_float(render_pt, radius), ggo::make_solid_brush(0.f), ggo::alpha_blender_y32f(opacity));
    }
  }

  // Blur and blend the shadow.
  ggo::gaussian_blur2d<ggo::y_32f_yu>(shadow_buffer.data(), sizeof(float) * width(), size(), 0.4f * min_size());

  apply_shadow(buffer_float.data(), shadow_buffer.data());

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

      ggo::paint<ggo::rgb_32f_yu, ggo::sampling_4x4>(
        buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
        ggo::disc_float(_points[i], radius), ggo::make_solid_brush(color), ggo::alpha_blender_rgb32f(opacity));
    }
  }

  // From float to uint8_t.
  ggo::blit<ggo::rgb_32f_yu, ggo::rgb_8u_yu>(
    buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
    buffer, width(), height(), line_step(), 0, 0);

  finished = first_point > iterations_count;
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
{
  int count = width() * height();

  for (int i = 0; i < count; ++i)
  {
    buffer[0] = shadow_buffer[0] * buffer[0];
    buffer[1] = shadow_buffer[0] * buffer[1];
    buffer[2] = shadow_buffer[0] * buffer[2];

    buffer += 3;
    shadow_buffer += 1;
  }
}
