#include "ggo_sonson_animation_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_fill.h>
#include <ggo_gradient_brush.h>
#include <ggo_paint.h>
#include <ggo_pixel_rect.h>
#include <numeric>

namespace
{
  const int frames_count = 200;

  //////////////////////////////////////////////////////////////
  void paint_arc(ggo::gray_image_abc & opacity_mask,
                 ggo::rgb_image_abc & color_mask,
                 const ggo::point2d_float & center, float radius1, float radius2, float angle1, float angle2,
                 const ggo::color & color)
  {
    GGO_ASSERT_EQ(opacity_mask.get_width(), color_mask.get_width());
    GGO_ASSERT_EQ(opacity_mask.get_height(), color_mask.get_height());

    float radius_inf = std::min(radius1, radius2);
    float radius_sup = std::max(radius1, radius2);
    float angle_inf = std::min(angle1, angle2);
    float angle_sup = std::max(angle1, angle2);
    GGO_ASSERT_LT(radius_inf, radius_sup);
    GGO_ASSERT_LT(angle_inf, angle_sup);

    ggo::pixel_rect pixel_rect(ggo::rect_float::from_left_right_bottom_top(center.x() - radius_sup,
                                                                           center.x() + radius_sup,
                                                                           center.y() - radius_sup,
                                                                           center.y() + radius_sup));

    if (pixel_rect.crop(opacity_mask.get_width(), opacity_mask.get_height()) == false)
    {
      return;
    }

    auto paint_pixel = [&](int x, int y)
    {
      float dy = ggo::to<float>(y - center.y());
      float dx = ggo::to<float>(x - center.x());

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

      opacity_mask.write(x, y, 1.0f);
      color_mask.write(x, y, color);
    };

    pixel_rect.for_each_pixel(paint_pixel);
  }

  //////////////////////////////////////////////////////////////
  void paint_rect(ggo::gray_image_abc & opacity_mask,
                  ggo::rgb_image_abc & color_mask, const ggo::point2d_float & p1, const ggo::point2d_float & p2,
                  const ggo::color & color)
  {
    GGO_ASSERT_EQ(opacity_mask.get_width(), color_mask.get_width());
    GGO_ASSERT_EQ(opacity_mask.get_height(), color_mask.get_height());

    ggo::rect_float rect(p1, p2);

    ggo::paint(opacity_mask, std::make_shared<ggo::rect_float>(rect), 1.f, 1.f, std::make_shared<ggo::gray_alpha_blender>(), ggo::pixel_sampler_1(), ggo::space_partitionning::block8x8);
    ggo::paint(color_mask, std::make_shared<ggo::rect_float>(rect), color, 1.f, std::make_shared<ggo::rgb_alpha_blender>(), ggo::pixel_sampler_1(), ggo::space_partitionning::block8x8);
  }

  //////////////////////////////////////////////////////////////
  void resolve_pi(float & angle1, float & angle2)
  {
    if (std::abs(std::abs(angle1) - ggo::PI<float>()) < 0.01f)
    {
      angle1 = angle2 > 0 ? ggo::PI<float>() : -ggo::PI<float>();
    }

    if (std::abs(std::abs(angle2) - ggo::PI<float>()) < 0.01f)
    {
      angle2 = angle1 > 0 ? ggo::PI<float>() : -ggo::PI<float>();
    }
  }

  //////////////////////////////////////////////////////////////
  int strips_size(const std::vector<std::pair<int, ggo::color>> & strips)
  {
    auto func = [](int sum, const std::pair<int, ggo::color> & p) { return sum + p.first; };

    return std::accumulate(strips.begin(), strips.end(), 0, func);
  }
}

//////////////////////////////////////////////////////////////
ggo_sonson_animation_artist::ggo_line::ggo_line(int width, int height, int scale_factor)
:
_opacity_mask(scale_factor * width, scale_factor * height),
_color_mask(scale_factor * width, scale_factor * height),
_scale_factor(scale_factor)
{

}

//////////////////////////////////////////////////////////////
ggo_sonson_animation_artist::ggo_line * ggo_sonson_animation_artist::ggo_line::create(float hue, int width, int height, int scale_factor)
{
  ggo_line * line = new ggo_line(width, height, scale_factor);

  float velocity = ggo::to<float>(scale_factor * ggo::to<int>(0.025f * std::min(width, height)));

  switch (ggo::rand_int(1, 4))
  {
  case 1: // Left
    line->_pos = { -0.5f, scale_factor * ggo::to<float>(ggo::rand_int(0, height)) - 0.5f };
    line->_velocity = { velocity, 0 };
    line->_strip_dir = { 0, -1 };
    break;
  case 2: // Right
    line->_pos = { scale_factor * ggo::to<float>(width) - 0.5f, scale_factor * ggo::to<float>(ggo::rand_int(0, height)) - 0.5f };
    line->_velocity = { -velocity, 0 };
    line->_strip_dir = { 0, 1 };
    break;
  case 3: // Bottom
    line->_pos = { scale_factor * ggo::to<float>(ggo::rand_int(0, width)) - 0.5f, -0.5f };
    line->_velocity = { 0, velocity };
    line->_strip_dir = { 1, 0 };
    break;
  case 4: // Top
    line->_pos = { scale_factor * ggo::to<float>(ggo::rand_int(0, width)) - 0.5f, scale_factor * ggo::to<float>(height) - 0.5f };
    line->_velocity = { 0, -velocity };
    line->_strip_dir = { -1, 0 };
    break;
  }

  line->_step = ggo::rand_int(3, 20);
  line->_step_end = 0;

  int strip_size = ggo::rand_int(3, 10);

  line->_strips.push_back(std::make_pair(scale_factor, ggo::color::BLACK));
  float sat = ggo::rand_float(0.5f, 1.0f);
  float val = ggo::rand_float(0.5f, 1.0f);
  for (int i = 0; i < strip_size; ++i)
  {
    float sat_cur = sat + ggo::rand_float(-0.1f, 0.1f);
    float val_cur = sat + ggo::rand_float(-0.1f, 0.1f);

    int min_size = std::min(width, height);
    int strip_size = scale_factor * ggo::to<int>(ggo::rand_float(0.002f * min_size, 0.008f * min_size));

    line->_strips.push_back(std::make_pair(strip_size, ggo::color::from_hsv(hue, sat_cur, val_cur)));
    line->_strips.push_back(std::make_pair(scale_factor, ggo::color::BLACK));
  }

  return line;
}

//////////////////////////////////////////////////////////////
bool ggo_sonson_animation_artist::ggo_line::update()
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
void ggo_sonson_animation_artist::ggo_line::update_strips()
{
  --_step;

  _pos.x() += _velocity.x();
  _pos.y() += _velocity.y();

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
    float velocity = std::max(std::abs(_velocity.x()), std::abs(_velocity.y()));
    float mid_radius = _radius + 0.5f * strips_size(_strips);
    _step_end = -ggo::to<int>(mid_radius * 0.5f * ggo::PI<float>() / velocity);
  }

  // Set up the straight line.
  if (_step == _step_end)
  {
    _step = ggo::rand_int(5, 30);
    _pos = _center + ggo::vector2d_float::from_polar(_angle_end, _radius + (_clock_wise ? strips_size(_strips) : 0.f));

    float velocity = std::max(std::abs(_velocity.x()), std::abs(_velocity.y()));

    ggo::vector2d_float new_velocity = (_clock_wise ? 1.f : -1.f) * _strip_dir * velocity;

    _strip_dir = (_clock_wise ? -1.f : 1.f) * _velocity.get_normalized();
    _velocity = new_velocity;
  }

  // Check if outisde of screen.
  if (_step >= 0)
  {
    auto glow_segment = get_glow_segment();

    int width = _opacity_mask.get_width() / _scale_factor;
    int height = _opacity_mask.get_height() / _scale_factor;

    if (glow_segment.get_rect_intersection(ggo::rect_float::from_left_right_bottom_top(-0.5f, width - 0.5f, -0.5f, height - 0.5f)) == ggo::rect_intersection::DISJOINTS)
    {
      _strips.clear();
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::ggo_line::render_masks()
{
  if (_step > 0) // Positive step => line is moving forward.
  {
    ggo::point2d_float p1 = _pos;

    for (const auto & strip : _strips)
    {
      float strip_size = ggo::to<float>(strip.first);

      ggo::point2d_float p2 = p1;
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
void ggo_sonson_animation_artist::ggo_line::setup_cw()
{
  _radius = ggo::to<float>(_scale_factor * ggo::rand_int(10, 50));

  float outter_radius = _radius + strips_size(_strips);

  _center = _pos + outter_radius * _strip_dir;
  _clock_wise = true;

  ggo::point2d_float pos_end = _center + _velocity.get_normalized() * outter_radius;

  ggo::vector2d_float diff_start = _pos - _center;
  ggo::vector2d_float diff_end = _velocity.get_normalized() * _radius;

  _angle_start = std::atan2(diff_start.y(), diff_start.x());
  _angle_end = std::atan2(diff_end.y(), diff_end.x());

  resolve_pi(_angle_start, _angle_end);
}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::ggo_line::setup_ccw()
{
  _radius = ggo::to<float>(_scale_factor * ggo::rand_int(10, 50));
  _center = _pos - _radius * _strip_dir;
  _clock_wise = false;

  ggo::point2d_float pos_end = _center + _velocity.get_normalized() * _radius;

  ggo::vector2d_float diff_start = _pos - _center;
  ggo::vector2d_float diff_end = _velocity.get_normalized() * _radius;

  _angle_start = std::atan2(diff_start.y(), diff_start.x());
  _angle_end = std::atan2(diff_end.y(), diff_end.x());

  resolve_pi(_angle_start, _angle_end);
}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::ggo_line::update_sparks()
{
  int render_min_size = std::min(_opacity_mask.get_width(), _opacity_mask.get_height()) / _scale_factor;

  // Create sparks if possible.
  if (_strips.empty() == false)
  {
    for (int i = 0; i < 5; ++i)
    {
      float weight = ggo::rand_float();
      ggo::segment_float segment = get_segment();
      ggo::point2d_float p1 = weight * segment.p1() + (1.f - weight) * segment.p2();

      float angle = ggo::rand_float(0.f, 2.f * ggo::PI<float>());
      float length = 0.01f * render_min_size;
      ggo::vector2d_float vel = ggo::vector2d_float::from_polar(angle, length);

      _sparks.push_back(ggo::segment_float(p1, p1 + vel));
    }
  }

  // Move sparks.
  for (auto & spark : _sparks)
  {
    ggo::point2d_float vel = spark.p2() - spark.p1();
    vel.y() -= 0.0025f * render_min_size;
    spark.p1() = spark.p2();
    spark.p2() += vel;
  }

  // Remove dead sparks.
  ggo::remove_if(_sparks, [](const ggo::segment_float & spark) { return spark.p1().y() < 0.f; });
}

//////////////////////////////////////////////////////////////
std::pair<float, ggo::color> ggo_sonson_animation_artist::ggo_line::get_pixel(int x, int y) const
{
  float opacity = 0.f;
  ggo::color layer_color = ggo::color::BLACK;

  for (int y_mask = _scale_factor * y; y_mask < _scale_factor * (y + 1); ++y_mask)
  {
    for (int x_mask = _scale_factor * x; x_mask < _scale_factor * (x + 1); ++x_mask)
    {
      opacity += _opacity_mask.read(x_mask, y_mask);
      layer_color += _color_mask.read(x_mask, y_mask);
    }
  }

  opacity /= ggo::to<float>(ggo::square(_scale_factor));
  layer_color /= ggo::to<float>(ggo::square(_scale_factor));

  return std::make_pair(opacity, layer_color);
}

//////////////////////////////////////////////////////////////
ggo::segment_float ggo_sonson_animation_artist::ggo_line::get_segment() const
{
  ggo::point2d_float p1, p2;

  if (_step >= 0)
  {
    p1 = _pos;
    p2 = _pos + _strip_dir * ggo::to<float>(strips_size(_strips));
  }
  else
  {
    float angle = ggo::map(static_cast<float>(-_step), 0.f, static_cast<float>(-_step_end), _angle_start, _angle_end);

    p1 = _center + ggo::vector2d_float::from_polar(angle, _radius);
    p2 = _center + ggo::vector2d_float::from_polar(angle, _radius + strips_size(_strips));
  }

  p1 /= ggo::to<float>(_scale_factor);
  p2 /= ggo::to<float>(_scale_factor);

  return ggo::segment_float(p1, p2);
}

//////////////////////////////////////////////////////////////
ggo::extended_segment_float ggo_sonson_animation_artist::ggo_line::get_glow_segment() const
{
  const int min_size = std::min(_opacity_mask.get_width(), _opacity_mask.get_height()) / _scale_factor;
  const float radius = 0.0025f * min_size;
  const ggo::segment_float segment = get_segment();

  return ggo::extended_segment_float(segment.p1(), segment.p2(), radius);
}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::ggo_line::render(ggo::rgb_image_buffer_uint8 & image) const
{
  uint8_t * buffer = image.data();

  // Paint the strips.
  uint8_t * it = buffer;
  for (int y = 0; y < image.get_height(); ++y)
  {
    for (int x = 0; x < image.get_width(); ++x)
    {
      ggo::color pixel_color(it[0] / 255.f, it[1] / 255.f, it[2] / 255.f);

      auto line_pixel = get_pixel(x, y);

      pixel_color = line_pixel.first * line_pixel.second + (1.f - line_pixel.first) * pixel_color;

      it[0] = pixel_color.r8();
      it[1] = pixel_color.g8();
      it[2] = pixel_color.b8();

      it += 3;
    }
  }

  // Paint line's glow.
  auto glow_segment = get_glow_segment();
  ggo::paint(image, std::make_shared<ggo::extended_segment_float>(glow_segment), ggo::color::WHITE);

  // Paint sparks.
  for (const auto & spark : _sparks)
  {
    auto opacity_brush = std::make_shared<ggo::gradient_brush<float, true>>();
    opacity_brush->_pos1 = spark.p1();
    opacity_brush->_value1 = 0.f;
    opacity_brush->_pos2 = spark.p2();
    opacity_brush->_value2 = 1.f;

    ggo::paint(image,
               std::make_shared<ggo::extended_segment_float>(spark.p1(), spark.p2(), 0.001f * std::min(image.get_width(), image.get_height())),
               std::make_shared<ggo::rgb_solid_brush>(ggo::color::WHITE),
               opacity_brush);
  }
}

//////////////////////////////////////////////////////////////
ggo_sonson_animation_artist::ggo_sonson_animation_artist(int width, int height)
:
ggo_animation_artist_abc(width, height)
{

}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::init_sub()
{
  _hue1 = ggo::rand_float();
  _hue2 = _hue1 + ggo::rand_float(0.25f, 0.75f);

  _lines.clear();
  _lines.resize(32);

  create_line(0, true); // At least one line in the foreground.
  for (int i = 0; i < 8; ++i)
  {
    create_line(0, false);
  }
}

//////////////////////////////////////////////////////////////
bool ggo_sonson_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (buffer != nullptr)
  {
    auto image = make_image_buffer(buffer);
    ggo::fill_4_colors(image, ggo::color::WHITE, ggo::color::WHITE, ggo::color::WHITE, ggo::color::BLACK);
  }

  // Create new lines.
  if (frame_index < frames_count && ggo::rand_float() < 0.25f)
  {
    create_line(frame_index, false);
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

  // Paint lines.
  if (buffer != nullptr)
  {
    auto image = make_image_buffer(buffer);
    for (const auto & sub_lines : _lines)
    {
      for (const auto & line : sub_lines)
      {
        line->render(image);
      }

      // Blur (only when needed).
      if (&sub_lines != &_lines.back())
      {
        float stddev = 0.001f * get_render_min_size();
        ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 0, buffer + 0, get_render_width(), get_render_height(), stddev, 0.01f);
        ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 1, buffer + 1, get_render_width(), get_render_height(), stddev, 0.01f);
        ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 2, buffer + 2, get_render_width(), get_render_height(), stddev, 0.01f);
      }
    }
  }

  return buffer == nullptr || render_next_frame == true;
}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::create_line(int frame_index, bool foreground)
{
  float hue = (ggo::rand_float() < ggo::map(static_cast<float>(frame_index), 0.f, static_cast<float>(frames_count), 0.1f, 0.9f)) ? _hue1 : _hue2;

  int stack_size = ggo::to<int>(_lines.size());
  int stack_index = foreground ? stack_size - 1 : ggo::rand_int(0, stack_size - 1);

  int scale_factor = 8;
  if (stack_index < _lines.size() / 4)
  {
    scale_factor = 2;
  }
  else if (stack_index < _lines.size() / 2)
  {
    scale_factor = 4;
  }

  _lines[stack_index].emplace_back(ggo_line::create(hue, get_render_width(), get_render_height(), scale_factor));
}
