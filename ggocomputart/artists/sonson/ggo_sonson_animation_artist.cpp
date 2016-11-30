#include "ggo_sonson_animation_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_fill.h>
#include <ggo_pixel_rect.h>
#include <ggo_multi_scale_paint.h>
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blender.h>
#include <ggo_gradient_brush.h>

namespace
{
  const int frames_count = 200;

  //////////////////////////////////////////////////////////////
  void paint_arc(ggo::rle_image<float> & opacity_mask,
                 ggo::rle_image<ggo::color_32f> & color_mask,
                 const ggo::pos2f & center, float radius1, float radius2, float angle1, float angle2,
                 const ggo::color_32f & color)
  {
    GGO_ASSERT_EQ(opacity_mask.get_width(), color_mask.get_width());
    GGO_ASSERT_EQ(opacity_mask.get_height(), color_mask.get_height());

    float radius_inf = std::min(radius1, radius2);
    float radius_sup = std::max(radius1, radius2);
    float angle_inf = std::min(angle1, angle2);
    float angle_sup = std::max(angle1, angle2);
    GGO_ASSERT_LT(radius_inf, radius_sup);
    GGO_ASSERT_LT(angle_inf, angle_sup);

    ggo::pixel_rect pixel_rect(ggo::rect_float::from_left_right_bottom_top(center.get<0>() - radius_sup,
                                                                           center.get<0>() + radius_sup,
                                                                           center.get<1>() - radius_sup,
                                                                           center.get<1>() + radius_sup));

    if (pixel_rect.crop(opacity_mask.get_width(), opacity_mask.get_height()) == false)
    {
      return;
    }

    auto paint_pixel = [&](int x, int y)
    {
      float dy = ggo::to<float>(y - center.get<1>());
      float dx = ggo::to<float>(x - center.get<0>());

      float angle = std::atan2(dy, dx);
      if (angle < angle_inf || angle > angle_sup)
      {
        return;
      }

      float dist = ggo::distance(dx, dy);
      if (dist < radius_inf || dist > radius_sup)
      {
        return;
      }

      opacity_mask.set(x, y, 1.0f);
      color_mask.set(x, y, color);
    };

    pixel_rect.for_each_pixel(paint_pixel);
  }

  //////////////////////////////////////////////////////////////
  void paint_rect(ggo::rle_image<float> & opacity_mask,
    ggo::rle_image<ggo::color_32f> & color_mask, const ggo::pos2f & p1, const ggo::pos2f & p2,
    const ggo::color_32f & color)
  {
    GGO_ASSERT_EQ(opacity_mask.get_width(), color_mask.get_width());
    GGO_ASSERT_EQ(opacity_mask.get_height(), color_mask.get_height());

    ggo::rect_float rect(p1, p2);

    ggo::paint_multi_scale<ggo::sampling_1>(
      opacity_mask.get_width(), opacity_mask.get_height(), rect, 8, 2,
      ggo::make_solid_brush(1.f), ggo::overwrite_blender<float>(),
      [&](int x, int y) { return opacity_mask.get(x, y); },
      [&](int x, int y, float c) { opacity_mask.set(x, y, c); },
      [&](const ggo::pixel_rect & block) { block.for_each_pixel([&](int x, int y) { opacity_mask.set(x, y, 1.f); }); });

    ggo::paint_multi_scale<ggo::sampling_1>(
      color_mask.get_width(), color_mask.get_height(), rect, 8, 2,
      ggo::make_solid_brush(color), ggo::overwrite_blender<ggo::color_32f>(),
      [&](int x, int y) { return color_mask.get(x, y); },
      [&](int x, int y, const ggo::color_32f & c) { color_mask.set(x, y, c); },
      [&](const ggo::pixel_rect & block) { block.for_each_pixel([&](int x, int y) { color_mask.set(x, y, color); }); });
  }

  //////////////////////////////////////////////////////////////
  void resolve_pi(float & angle1, float & angle2)
  {
    if (std::abs(std::abs(angle1) - ggo::pi<float>()) < 0.01f)
    {
      angle1 = angle2 > 0 ? ggo::pi<float>() : -ggo::pi<float>();
    }

    if (std::abs(std::abs(angle2) - ggo::pi<float>()) < 0.01f)
    {
      angle2 = angle1 > 0 ? ggo::pi<float>() : -ggo::pi<float>();
    }
  }

  //////////////////////////////////////////////////////////////
  int strips_size(const std::vector<std::pair<int, ggo::color_32f>> & strips)
  {
    auto func = [](int sum, const std::pair<int, ggo::color_32f> & p) { return sum + p.first; };

    return std::accumulate(strips.begin(), strips.end(), 0, func);
  }
}

//////////////////////////////////////////////////////////////
ggo::sonson_animation_artist::line::line(int width, int height, int scale_factor)
:
_opacity_mask(scale_factor * width, scale_factor * height),
_color_mask(scale_factor * width, scale_factor * height),
_scale_factor(scale_factor)
{

}

//////////////////////////////////////////////////////////////
ggo::sonson_animation_artist::line * ggo::sonson_animation_artist::line::create(float hue, int width, int height, int scale_factor)
{
  auto * line = new ggo::sonson_animation_artist::line(width, height, scale_factor);

  float velocity = ggo::to<float>(scale_factor * ggo::to<int>(0.025f * std::min(width, height)));

  switch (ggo::rand<int>(1, 4))
  {
  case 1: // Left
    line->_pos = { -0.5f, scale_factor * ggo::to<float>(ggo::rand<int>(0, height)) - 0.5f };
    line->_velocity = { velocity, 0.f };
    line->_strip_dir = { 0.f, -1.f };
    break;
  case 2: // Right
    line->_pos = { scale_factor * ggo::to<float>(width) - 0.5f, scale_factor * ggo::to<float>(ggo::rand<int>(0, height)) - 0.5f };
    line->_velocity = { -velocity, 0.f };
    line->_strip_dir = { 0.f, 1.f };
    break;
  case 3: // Bottom
    line->_pos = { scale_factor * ggo::to<float>(ggo::rand<int>(0, width)) - 0.5f, -0.5f };
    line->_velocity = { 0.f, velocity };
    line->_strip_dir = { 1.f, 0.f };
    break;
  case 4: // Top
    line->_pos = { scale_factor * ggo::to<float>(ggo::rand<int>(0, width)) - 0.5f, scale_factor * ggo::to<float>(height) - 0.5f };
    line->_velocity = { 0.f, -velocity };
    line->_strip_dir = { -1.f, 0.f };
    break;
  }

  line->_step = ggo::rand<int>(3, 20);
  line->_step_end = 0;

  int strip_size = ggo::rand<int>(3, 10);

  line->_strips.push_back(std::make_pair(scale_factor, ggo::black<ggo::color_32f>()));
  float sat = ggo::rand<float>(0.5f, 1.0f);
  float val = ggo::rand<float>(0.5f, 1.0f);
  for (int i = 0; i < strip_size; ++i)
  {
    float sat_cur = sat + ggo::rand<float>(-0.1f, 0.1f);
    float val_cur = sat + ggo::rand<float>(-0.1f, 0.1f);

    int min_size = std::min(width, height);
    int strip_size = scale_factor * ggo::to<int>(ggo::rand<float>(0.002f * min_size, 0.008f * min_size));

    line->_strips.push_back(std::make_pair(strip_size, ggo::from_hsv<ggo::color_32f>(hue, sat_cur, val_cur)));
    line->_strips.push_back(std::make_pair(scale_factor, ggo::black<ggo::color_32f>()));
  }

  return line;
}

//////////////////////////////////////////////////////////////
bool ggo::sonson_animation_artist::line::update()
{
  if (_strips.empty() == false)
  {
    render_masks();
    update_strips();
  }

  update_sparks();

  return _strips.empty() == false || _sparks.empty() == false; // Update the line while there are sparks or strips. 
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::update_strips()
{
  --_step;

  _pos.get<0>() += _velocity.get<0>();
  _pos.get<1>() += _velocity.get<1>();

  // Set up the arc.
  if (_step == 0)
  {
    if (ggo::rand_bool() == true)
    {
      setup_cw();
    }
    else
    {
      setup_ccw();
    }

    // Compute arc steps number to match velocity.
    float velocity = std::max(std::abs(_velocity.get<0>()), std::abs(_velocity.get<1>()));
    float mid_radius = _radius + 0.5f * strips_size(_strips);
    _step_end = -ggo::to<int>(mid_radius * 0.5f * ggo::pi<float>() / velocity);
  }

  // Set up the straight line.
  if (_step == _step_end)
  {
    _step = ggo::rand<int>(5, 30);
    _pos = _center + ggo::from_polar(_angle_end, _radius + (_clock_wise ? strips_size(_strips) : 0.f));

    float velocity = std::max(std::abs(_velocity.get<0>()), std::abs(_velocity.get<1>()));

    ggo::vec2f new_velocity = (_clock_wise ? 1.f : -1.f) * _strip_dir * velocity;

    _strip_dir = (_clock_wise ? -1.f : 1.f) * _velocity.get_normalized();
    _velocity = new_velocity;
  }

  // Check if outisde of screen.
  if (_step >= 0)
  {
    auto glow_segment = get_glow_segment();

    int width = _opacity_mask.get_width() / _scale_factor;
    int height = _opacity_mask.get_height() / _scale_factor;

    if (glow_segment.get_rect_intersection(ggo::rect_float::from_left_right_bottom_top(-0.5f, width - 0.5f, -0.5f, height - 0.5f)) == ggo::rect_intersection::disjoints)
    {
      _strips.clear();
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::render_masks()
{
  if (_step > 0) // Positive step => line is moving forward.
  {
    ggo::pos2f p1 = _pos;

    for (const auto & strip : _strips)
    {
      float strip_size = ggo::to<float>(strip.first);

      ggo::pos2f p2 = p1;
      p2 += strip_size * _strip_dir + _velocity;

      paint_rect(_opacity_mask, _color_mask, p1, p2, strip.second);

      p1 += strip_size * _strip_dir;
    }
  }
  else // Otherwise, the line is turning.
  {
    if (_clock_wise == true)
    {
      float radius_sup = strips_size(_strips) + _radius;
      for (const auto & strip : _strips)
      {
        float radius_inf = radius_sup - strip.first;
        float angle1 = ggo::map(static_cast<float>(-_step), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);
        float angle2 = ggo::map(static_cast<float>(-_step + 1), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);

        paint_arc(_opacity_mask, _color_mask, _center, radius_inf, radius_sup, angle1, angle2, strip.second);
        radius_sup = radius_inf;
      }
    }
    else
    {
      float radius_inf = _radius;
      for (const auto & strip : _strips)
      {
        float radius_sup = radius_inf + strip.first;
        float angle1 = ggo::map(static_cast<float>(-_step), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);
        float angle2 = ggo::map(static_cast<float>(-_step + 1), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);

        paint_arc(_opacity_mask, _color_mask, _center, radius_inf, radius_sup, angle1, angle2, strip.second);
        radius_inf = radius_sup;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::setup_cw()
{
  _radius = ggo::to<float>(_scale_factor * ggo::rand<int>(10, 50));

  float outter_radius = _radius + strips_size(_strips);

  _center = _pos + outter_radius * _strip_dir;
  _clock_wise = true;

  ggo::pos2f pos_end = _center + _velocity.get_normalized() * outter_radius;

  ggo::vec2f diff_start = _pos - _center;
  ggo::vec2f diff_end = _velocity.get_normalized() * _radius;

  _angle_start = std::atan2(diff_start.get<1>(), diff_start.get<0>());
  _angle_end = std::atan2(diff_end.get<1>(), diff_end.get<0>());

  resolve_pi(_angle_start, _angle_end);
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::setup_ccw()
{
  _radius = ggo::to<float>(_scale_factor * ggo::rand<int>(10, 50));
  _center = _pos - _radius * _strip_dir;
  _clock_wise = false;

  ggo::pos2f pos_end = _center + _velocity.get_normalized() * _radius;

  ggo::vec2f diff_start = _pos - _center;
  ggo::vec2f diff_end = _velocity.get_normalized() * _radius;

  _angle_start = std::atan2(diff_start.get<1>(), diff_start.get<0>());
  _angle_end = std::atan2(diff_end.get<1>(), diff_end.get<0>());

  resolve_pi(_angle_start, _angle_end);
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::update_sparks()
{
  int render_min_size = std::min(_opacity_mask.get_width(), _opacity_mask.get_height()) / _scale_factor;

  // Create sparks if possible.
  if (_strips.empty() == false)
  {
    for (int i = 0; i < 5; ++i)
    {
      float weight = ggo::rand<float>();
      ggo::segment_float segment = get_segment();
      ggo::pos2f p1 = weight * segment.p1() + (1.f - weight) * segment.p2();

      float angle = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
      float length = 0.01f * render_min_size;
      ggo::vec2f vel = ggo::from_polar(angle, length);

      _sparks.push_back(ggo::segment_float(p1, p1 + vel));
    }
  }

  // Move sparks.
  for (auto & spark : _sparks)
  {
    ggo::pos2f vel = spark.p2() - spark.p1();
    vel.get<1>() -= 0.0025f * render_min_size;
    spark.p1() = spark.p2();
    spark.p2() += vel;
  }

  // Remove dead sparks.
  ggo::remove_if(_sparks, [](const ggo::segment_float & spark) { return spark.p1().get<1>() < 0.f; });
}

//////////////////////////////////////////////////////////////
std::pair<float, ggo::color_32f> ggo::sonson_animation_artist::line::get_pixel(int x, int y) const
{
  float opacity = 0.f;
  ggo::color_32f layer_color = ggo::black<ggo::color_32f>();

  for (int y_mask = _scale_factor * y; y_mask < _scale_factor * (y + 1); ++y_mask)
  {
    for (int x_mask = _scale_factor * x; x_mask < _scale_factor * (x + 1); ++x_mask)
    {
      opacity += _opacity_mask.get(x_mask, y_mask);
      layer_color += _color_mask.get(x_mask, y_mask);
    }
  }

  opacity /= ggo::to<float>(ggo::square(_scale_factor));
  layer_color /= ggo::to<float>(ggo::square(_scale_factor));

  return std::make_pair(opacity, layer_color);
}

//////////////////////////////////////////////////////////////
ggo::segment_float ggo::sonson_animation_artist::line::get_segment() const
{
  ggo::pos2f p1, p2;

  if (_step >= 0)
  {
    p1 = _pos;
    p2 = _pos + _strip_dir * ggo::to<float>(strips_size(_strips));
  }
  else
  {
    float angle = ggo::map(static_cast<float>(-_step), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);

    p1 = _center + ggo::from_polar(angle, _radius);
    p2 = _center + ggo::from_polar(angle, _radius + strips_size(_strips));
  }

  p1 /= ggo::to<float>(_scale_factor);
  p2 /= ggo::to<float>(_scale_factor);

  return ggo::segment_float(p1, p2);
}

//////////////////////////////////////////////////////////////
ggo::extended_segment_float ggo::sonson_animation_artist::line::get_glow_segment() const
{
  const int min_size = std::min(_opacity_mask.get_width(), _opacity_mask.get_height()) / _scale_factor;
  const float radius = 0.0025f * min_size;
  const ggo::segment_float segment = get_segment();

  return ggo::extended_segment_float(segment.p1(), segment.p2(), radius);
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::line::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf) const
{
  // Paint the strips.
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto line_pixel = get_pixel(x, y);

      ggo::color_32f c_32f = ggo::convert_color_to<ggo::color_32f>(ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, line_step));

      c_32f = line_pixel.first * line_pixel.second + (1.f - line_pixel.first) * c_32f;

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, line_step, ggo::convert_color_to<ggo::color_8u>(c_32f));
    }
  }

  // Paint line's glow.
  auto glow_segment = get_glow_segment();
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, width, height, 3 * width, glow_segment, ggo::white<ggo::color_8u>());

  // Paint sparks.
  for (const auto & spark : _sparks)
  {
    ggo::gradient_brush<float> opacity_brush(0.f, spark.p1(), 1.f, spark.p2());

    auto blend = [&](int x, int y, const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color)
    {
      const float opacity = opacity_brush(x, y);
      const ggo::alpha_blender<ggo::color_8u> blender(opacity);
      return blender(x, y, bkgd_color, brush_color);
    };

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, width, height, 3 * width, ggo::extended_segment_float(spark.p1(), spark.p2(), 0.001f * std::min(width, height)),
      ggo::white_brush_8u(), blend);
  }
}

//////////////////////////////////////////////////////////////
ggo::sonson_animation_artist::sonson_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::init()
{
  _frame_index = -1;

  _hue1 = ggo::rand<float>();
  _hue2 = _hue1 + ggo::rand<float>(0.25f, 0.75f);

  _lines.clear();
  _lines.resize(32);

  create_line(0, true); // At least one line in the foreground.
  for (int i = 0; i < 8; ++i)
  {
    create_line(0, false);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::sonson_animation_artist::update()
{
  ++_frame_index;

  // Create new lines.
  if (_frame_index < frames_count && ggo::rand<float>() < 0.25f)
  {
    create_line(_frame_index, false);
  }

  // Update lines.
  bool render_next_frame = false;

  for (auto & sub_lines : _lines)
  {
    for (auto & line : sub_lines)
    {
      if (line->update() == true)
      {
        render_next_frame = true;
      }
    }
  }

  return render_next_frame;
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping) const
{
  if (buffer != nullptr)
  {
    ggo::fill_4_colors<ggo::rgb_8u_yu>(
      buffer, get_width(), get_height(), get_line_step(),
      ggo::white<ggo::color_8u>(), ggo::white<ggo::color_8u>(), ggo::white<ggo::color_8u>(), ggo::black<ggo::color_8u>());
  }

  // Paint lines.
  if (buffer != nullptr)
  {
    for (const auto & sub_lines : _lines)
    {
      for (const auto & line : sub_lines)
      {
        line->render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format());
      }

      // Blur (only when needed).
      if (&sub_lines != &_lines.back())
      {
        const float stddev = 0.001f * get_min_size();
        ggo::gaussian_blur2d_mirror<ggo::rgb_8u_yu>(
          buffer, get_width(), get_height(), get_line_step(), stddev);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::sonson_animation_artist::create_line(int frame_index, bool foreground)
{
  float hue = (ggo::rand<float>() < ggo::map(static_cast<float>(frame_index), 0.f, static_cast<float>(frames_count), 0.1f, 0.9f)) ? _hue1 : _hue2;

  int stack_size = ggo::to<int>(_lines.size());
  int stack_index = foreground ? stack_size - 1 : ggo::rand<int>(0, stack_size - 1);

  int scale_factor = 8;
  if (stack_index < _lines.size() / 4)
  {
    scale_factor = 2;
  }
  else if (stack_index < _lines.size() / 2)
  {
    scale_factor = 4;
  }

  _lines[stack_index].emplace_back(ggo::sonson_animation_artist::line::create(hue, get_width(), get_height(), scale_factor));
}
