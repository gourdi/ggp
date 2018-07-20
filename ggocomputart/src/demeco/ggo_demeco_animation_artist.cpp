#include "ggo_demeco_animation_artist.h"
#include <2d/ggo_blit.h>
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  constexpr ggo::image_format render_format = ggo::rgba_8u_yd;
  constexpr ggo::image_format shadow_format = ggo::y_8u_yd;

  using render_format_traits = ggo::image_format_traits<render_format>;
  using shadow_format_traits = ggo::image_format_traits<shadow_format>;
}

//////////////////////////////////////////////////////////////
ggo::demeco_animation_artist::demeco_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
animation_artist_abc(width, height, line_step, format),
_render_image(width, height, render_format),
_shadow_image(width, height, shadow_format)
{
  // Create palette.
  for (auto & c : _palette)
  {
    c = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand(0.3f, 1.0f), ggo::rand(0.7f, 0.9f));
  }

  // Create demecos.
  const int steps = ggo::rand<int>(3, 5);
  const float main_radius = ggo::rand<float>(0.4f, 0.5f) * min_size();
  const float delta = 2 * main_radius / float(2 * steps + 1);

  for (int y = -steps; y <= steps; ++y)
  {
    for (int x = -steps; x <= steps; ++x)
    {
      float x_f = float(x) * delta;
      float y_f = float(y) * delta;
      float radius = ggo::distance(x_f, y_f);

      bool create = true;
      if (radius > main_radius)
      {
        create = false;
      }
      else if (radius > main_radius / 2)
      {
        create = ggo::rand<float>() > ggo::ease_inout((radius - main_radius / 2) / (main_radius / 2));
      }

      if (create == true)
      {
        std::unique_ptr<demeco> demeco;

        switch (ggo::rand(0, 2))
        {
        case 0:
          demeco.reset(new demeco1(get_center() + pos2f(x_f, y_f), 0.5f * delta, ggo::rand<int>(-200, 0), _palette));
          break;
        case 1:
          demeco.reset(new demeco2(get_center() + pos2f(x_f, y_f), 0.5f * delta, ggo::rand<int>(-200, 0), _palette));
          break;
        case 2:
          demeco.reset(new demeco3(get_center() + pos2f(x_f, y_f), 0.5f * delta, ggo::rand<int>(-200, 0), _palette));
          break;
        }
        
        _demecos.emplace_back(std::move(demeco));
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::demeco_animation_artist::render_frame(void * buffer, int frame_index, bool & finished)
{
  // Render the demecos.
  finished = true;
  ggo::fill_solid<ggo::rgba_8u_yd>(_render_image.data(), _render_image.width(), _render_image.height(), _render_image.line_byte_step(), ggo::alpha_color_8u(0, 0, 0, 0));
  for (auto & demeco : _demecos)
  {
    if (demeco->_counter < 0)
    {
      finished = false;
    }
    else
    {
      if (demeco->render(_render_image) == true)
      {
        finished = false;
      }
    }

    demeco->_counter++;
  }

  // Blit render buffer alpha channel into shadow buffer.
  ggo::fill_solid<shadow_format>(_shadow_image.data(), _shadow_image.width(), _shadow_image.height(), _shadow_image.line_byte_step(), 255);
  const int shadow_dx = min_size() / 100;
  const int shadow_dy = min_size() / 100;
  for (int y = shadow_dy; y < height(); ++y)
  {
    const void * src = _render_image.pixel_ptr(0, y);
    void * dst = _shadow_image.pixel_ptr(shadow_dx, y - shadow_dy);

    for (int x = shadow_dx; x < width(); ++x)
    {
      auto pixel = read_pixel<render_format>(src);
      write_pixel<shadow_format>(dst, 0xff - pixel.a());
      
      src = ggo::ptr_offset<render_format_traits::pixel_byte_size>(src);
      dst = ggo::ptr_offset<shadow_format_traits::pixel_byte_size>(dst);
    }
  }

  // Blur shadow.
  float stddev = min_size() / 100.f;
  gaussian_blur2d<shadow_format>(_shadow_image.data(), _shadow_image.line_byte_step(), _shadow_image.size(), stddev);
  
  // Blit shadow and render buffers into output buffer.
  blit<shadow_format, ggo::rgb_8u_yu>(_shadow_image.data(), _shadow_image.width(), _shadow_image.height(), _shadow_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);

  blit<render_format, ggo::rgb_8u_yu>(_render_image.data(), _render_image.width(), _render_image.height(), _render_image.line_byte_step(),
    buffer, width(), height(), line_step(), 0, 0);
}

//////////////////////////////////////////////////////////////
ggo::demeco_animation_artist::demeco1::demeco1(const ggo::pos2f & pos, float radius, int counter, const palette & palette)
:
demeco(pos, radius, counter)
{
  float r_inf = radius * ggo::rand<float>(0.01f, 0.1f);
  float r_sup = r_inf + radius * ggo::rand<float>(0.1f, 0.2f);
  float radius_max = radius * ggo::rand(0.5f, 0.8f);
  float start_angle = ggo::rand(0.f, 2 * ggo::pi<float>());
  float dstart_angle = ggo::rand(-0.5f, 0.5f);
  bool ccw = ggo::rand<bool>();

  // Shuffle palette.
  demeco_animation_artist::palette palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  int color_index = 0;

  while (r_sup < radius_max)
  {
    _arcs.emplace_back(palette_shuffled[color_index % palette_shuffled.size()], 0.5f * (r_inf + r_sup), r_sup - r_inf, start_angle, ccw);

    r_inf = r_sup + radius * ggo::rand<float>(0.01f, 0.1f);
    r_sup = r_inf + radius * ggo::rand<float>(0.1f, 0.2f);
    start_angle += dstart_angle;
    color_index++;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::demeco_animation_artist::demeco1::render(ggo::image & image) const
{
  constexpr int angle_counter_max = 50;
  int angle_counter = std::min(_counter, angle_counter_max);
  float max_angle = std::min(ggo::ease_inout(angle_counter, angle_counter_max, 0.f, 2.f * ggo::pi<float>()), 2.f * ggo::pi<float>());

  constexpr int fade_in_counter_max = 40;
  int fade_in_counter = std::min(_counter, fade_in_counter_max);
  uint8_t alpha = ggo::round_to<uint8_t>(ggo::ease_inout<float>(fade_in_counter, fade_in_counter_max, 0.f, 255.f));

  constexpr int discs_count = 96;

  for (const auto & arc : _arcs)
  {
    multi_shape_float paint_arc;

    for (int i = 0; i < discs_count; ++i)
    {
      float angle = arc._start_angle + (arc._ccw ? 1.f : -1) * i * max_angle / discs_count;

      paint_arc.add_shape(std::make_shared<ggo::disc_float>(_pos + from_polar(angle, arc._radius), arc._width / 2));
    }

    alpha_color_8u shape_color(arc._color, alpha);
    ggo::paint_shape<render_format, ggo::sampling_8x8>(image.data(), image.width(), image.height(), image.line_byte_step(), paint_arc, shape_color);
  }

  return _counter <= std::max(angle_counter_max, fade_in_counter_max);
}

//////////////////////////////////////////////////////////////
ggo::demeco_animation_artist::demeco2::demeco2(const ggo::pos2f & pos, float radius, int counter, const palette & palette)
:
demeco(pos, radius, counter)
{
  // Pick up 3 colors.
  demeco_animation_artist::palette palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  _animations.emplace_back(palette_shuffled[0], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[1], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[2], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));

  _init_angle = ggo::rand(0.f, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
bool ggo::demeco_animation_artist::demeco2::render(ggo::image & image) const
{
  constexpr int anim1_counter_start = 0;
  constexpr int anim1_counter_end = 10;
  constexpr int anim1_counter_delta = anim1_counter_end - anim1_counter_start;
  constexpr int anim2_counter_start = 40;
  constexpr int anim2_counter_end = 50;
  constexpr int anim2_counter_delta = anim2_counter_end - anim2_counter_start;
  constexpr int anim3_counter_start = 80;
  constexpr int anim3_counter_end = 90;
  constexpr int anim3_counter_delta = anim3_counter_end - anim3_counter_start;

  using paint_box = solid_color_shape<ggo::oriented_box_float, ggo::alpha_color_8u>;
  std::vector<paint_box> paint_boxes;
  
  // Compute current angle.
  float angle = _init_angle;
  if (_counter < anim1_counter_end)
  {
    angle = ggo::ease_inout(_counter, anim1_counter_delta, _init_angle, _animations[0]._end_angle);
  }
  else if (_counter <= anim2_counter_start)
  {
    angle = _animations[0]._end_angle;
  }
  else if (_counter < anim2_counter_end)
  {
    angle = ggo::ease_inout(_counter, anim2_counter_start, anim2_counter_end, _animations[0]._end_angle, _animations[1]._end_angle);
  }
  else if (_counter <= anim3_counter_start)
  {
    angle = _animations[1]._end_angle;
  }
  else if (_counter <= anim3_counter_end)
  {
    angle = ggo::ease_inout(_counter, anim3_counter_start, anim3_counter_end, _animations[1]._end_angle, _animations[2]._end_angle);
  }
  else
  {
    angle = _animations[2]._end_angle;
  }

  // Boxes animation.
  ggo::vec2f dir1 = ggo::from_polar(angle, 1.f);
  ggo::vec2f dir2{ dir1.y(), -dir1.x() };

  float main_size = _animations[0]._size;

  // Animation 2.
  if (_counter >= anim2_counter_start)
  {
    int counter = std::min(_counter, anim2_counter_end) - anim2_counter_start;
    float size = ggo::ease_inout(counter, anim2_counter_delta, main_size, main_size + _animations[1]._size);
    ggo::alpha_color_8u shape_color(_animations[1]._color, 255);

    paint_boxes.emplace_back(ggo::oriented_box_float(_pos, dir1, main_size, size), shape_color);
  }

  // Animation 3.
  if (_counter >= anim3_counter_start)
  {
    int counter = std::min(_counter, anim3_counter_end) - anim3_counter_start;
    float size = ggo::ease_inout(counter, anim3_counter_delta, main_size, main_size + _animations[2]._size);
    ggo::alpha_color_8u shape_color(_animations[2]._color, 255);

    paint_boxes.emplace_back(ggo::oriented_box_float(_pos, dir2, main_size, size), shape_color);
  }

  // Animation 1. Must be last.
  if (_counter >= anim1_counter_start)
  {
    int counter = std::min(_counter, anim1_counter_end) - anim1_counter_start;
    float size = ggo::ease_inout(counter, anim1_counter_delta, 0.f, main_size);
    ggo::alpha_color_8u shape_color(_animations[0]._color, 255);

    paint_boxes.emplace_back(ggo::oriented_box_float(_pos, dir2, main_size, size), shape_color);
  }

  paint_shapes<render_format, ggo::sampling_8x8>(image.data(), image.width(), image.height(), image.line_byte_step(), paint_boxes);

  return _counter <= anim3_counter_end;
}

//////////////////////////////////////////////////////////////
ggo::demeco_animation_artist::demeco3::demeco3(const ggo::pos2f & pos, float radius, int counter, const palette & palette)
:
demeco(pos, radius, counter)
{
  demeco_animation_artist::palette palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  int count = ggo::rand(16, 24);
  float radius_sup = radius * ggo::rand(0.7f,0.85f);
  float radius_inf = radius_sup * ggo::rand(0.6f, 0.8f);

  for (int l = 0; l < 4; ++l)
  {
    float aperture = 2 * ggo::pi<float>() / count;
    float angle_start = ggo::rand(0.f, ggo::pi<float>());

    for (int i = 0; i < count; ++i)
    {
      float angle = angle_start + i * 2 * ggo::pi<float>() / count;
      int delay = 30 * (2 - l);

      _peaks.emplace_back(angle, aperture, radius_inf, radius_sup, palette_shuffled[l], delay);
    }

    count -= ggo::rand(2, 5);
    radius_sup = radius_inf * ggo::rand(0.7f, 1.1f);
    radius_inf = radius_sup * ggo::rand(0.6f, 0.8f);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::demeco_animation_artist::demeco3::render(ggo::image & image) const
{
  using paint_polygon = solid_color_shape<ggo::polygon2d_float, ggo::alpha_color_8u>;
  std::vector<paint_polygon> paint_polygons;

  constexpr int anim1_duration = 10;
  constexpr int anim2_duration = 10;

  bool finished = true;

  auto curve = [&](int counter)
  {
    if (counter < anim1_duration)
    {
      finished = false;
      return ggo::ease_inout(counter, anim1_duration, 0.f, 1.2f);
    }
    else if (counter < anim1_duration + anim2_duration)
    {
      finished = false;
      return ggo::ease_inout(counter, anim1_duration, anim1_duration + anim2_duration, 1.2f, 1.f);
    }
    else
    {
      return 1.f;
    }
  };

  for (const auto & peak : _peaks)
  {
    float scale = curve(std::max(_counter - peak._delay, 0));
    float radius_inf = scale * peak._radius_inf;
    float radius_sup = scale * peak._radius_sup;

    ggo::pos2f p0 = _pos;
    ggo::pos2f p1 = _pos + from_polar(peak._angle - 0.5f * peak._aperture, radius_inf);
    ggo::pos2f p2 = _pos + from_polar(peak._angle, radius_sup);
    ggo::pos2f p3 = _pos + from_polar(peak._angle + 0.5f * peak._aperture, radius_inf);

    ggo::polygon2d_float polygon({ p0, p1, p2, p3 });
    ggo::alpha_color_8u shape_color(peak._color, 255);

    paint_polygons.emplace_back(polygon, shape_color);
  }

  paint_shapes<render_format, ggo::sampling_8x8>(image.data(), image.width(), image.height(), image.line_byte_step(), paint_polygons);

  return !finished;
}


