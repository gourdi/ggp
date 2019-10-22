#include <2d/processing/ggo_blit.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
typename ggo::pixel_type_traits<pixel_type>::color_t ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::from_8u(const ggo::rgb_8u & c)
{
  using color_t = pixel_type_traits<pixel_type>::color_t;

  if constexpr(std::is_same<color_t, ggo::rgb_8u>::value)
  {
    return c;
  }
  else if constexpr(std::is_same<color_t, ggo::rgba_8u>::value)
  {
    return rgba_8u(c, 255);
  }
  else
  {
    static_assert(false);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco_artist(int width, int height)
:
_width(width), _height(height),
_background_image({ width, height })
{
  using color_t = typename pixel_type_traits<pixel_type>::color_t;

  // Create palette.
  for (auto & c : _palette)
  {
    c = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand(0.3f, 1.0f), ggo::rand(0.7f, 0.9f));
  }

  // Create demecos.
  const float demeco_radius = min_size() * ggo::rand(0.04f, 0.08f);
  const float main_radius = 0.5f * min_size() - demeco_radius;

  auto create_demeco = [&](float x, float y)
  {
    float radius = ggo::length(ggo::pos2_f(x, y));

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
        demeco.reset(new demeco1(center() + pos2_f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
        break;
      case 1:
        demeco.reset(new demeco2(center() + pos2_f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
        break;
      case 2:
        demeco.reset(new demeco3(center() + pos2_f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
        break;
      }

      _demecos.emplace_back(std::move(demeco));
    }
  };

  for (float center_y = demeco_radius; center_y < main_radius; center_y += 2 * demeco_radius)
  {
    for (float center_x = demeco_radius; center_x < main_radius; center_x += 2 * demeco_radius)
    {
      create_demeco(center_x, center_y);
      create_demeco(-center_x, center_y);
      create_demeco(center_x, -center_y);
      create_demeco(-center_x, -center_y);
    }
  }

  // Fill background.
  image_t<pixel_type, memory_lines_order> background(_background_image.data(), _background_image.size(), _background_image.line_byte_step());

  if constexpr(std::is_same_v<color_t, ggo::rgb_8u> == true)
  {
    fill_solid(background, white_8u());
  }
  else if constexpr(std::is_same_v<color_t, ggo::rgba_8u> == true)
  {
    fill_solid(background, { 0, 0, 0, 0 });
  }
  else
  {
    static_assert(false);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
bool ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::update()
{
  bool finished = true;
  
  _background_scene.clear();
  _active_scene.clear();

  for (auto & demeco : _demecos)
  {
    if (demeco->_counter < 0)
    {
      finished = false;

      demeco->_counter++;
    }
    else
    {
      // Store paint into into either the background or active paint shapes list.
      auto & scene_dst = demeco->finished() ? _background_scene : _active_scene;
      demeco->get_paint_shapes(scene_dst);

      demeco->_counter++;

      if (demeco->finished() == false)
      {
        finished = false;
      }
    }
  }

  ggo::remove_if(_demecos, [](const auto & demeco) { return demeco->finished(); });

  return finished;
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::render_tile(void * buffer, int line_byte_step, int frame_index, const ggo::rect_int & clipping)
{
  GGO_ASSERT_EQ(_width, _background_image.width());
  GGO_ASSERT_EQ(_height, _background_image.height());

  image_t<pixel_type, memory_lines_order> img(buffer, { _width, _height }, line_byte_step);

  // Render the background demecos.
  paint<sampling>(_background_image, _background_scene, clipping);

  // Blit the background.
  blit(*make_image_view(_background_image, clipping), *make_image_view(img, clipping));

  // Render the active demecos.
  paint<sampling>(img, _active_scene, clipping);
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco1::demeco1(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette)
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
  demeco_artist<pixel_type, memory_lines_order, sampling>::palette_t palette_shuffled = palette;
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
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco1::get_paint_shapes(scene_t & scene) const
{
  using color_t = pixel_type_traits<pixel_type>::color_t;

  int angle_counter = std::min(_counter, _angle_counter_max);
  float max_angle = std::min(ggo::ease_inout(angle_counter, _angle_counter_max, 0.f, 2.f * ggo::pi<float>()), 2.f * ggo::pi<float>());

  int fade_in_counter = std::min(_counter, _fade_in_counter_max);
  float opacity = ggo::ease_inout(fade_in_counter, _fade_in_counter_max, 0.f, 1.f);

  constexpr int discs_count = 96;

  for (const auto & arc : _arcs)
  {
    auto paint_arc = scene.make_paint_shape_t(multi_shape_f(), from_8u(arc._color), opacity);

    for (int i = 0; i < discs_count; ++i)
    {
      float angle = arc._start_angle + (arc._ccw ? 1.f : -1) * i * max_angle / discs_count;

      paint_arc._shape.add_shape(std::make_shared<ggo::disc_f>(_pos + arc._radius * ggo::vec2_f::from_angle(angle), arc._width / 2));
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
bool ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco1::finished() const
{
  return _counter > std::max(demeco1::_angle_counter_max, demeco1::_fade_in_counter_max);
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco2::demeco2(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette)
:
demeco(pos, radius, counter)
{
  // Pick up 3 colors.
  demeco_artist<pixel_type, memory_lines_order, sampling>::palette_t palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  _animations.emplace_back(palette_shuffled[0], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[1], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[2], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));

  _init_angle = ggo::rand(0.f, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco2::get_paint_shapes(scene_t & scene) const
{
  // Compute current angle.
  float angle = _init_angle;
  if (_counter < _anim1_counter_end)
  {
    angle = ggo::ease_inout(_counter, _anim1_counter_start, _anim1_counter_end, _init_angle, _animations[0]._end_angle);
  }
  else if (_counter <= _anim2_counter_start)
  {
    angle = _animations[0]._end_angle;
  }
  else if (_counter < _anim2_counter_end)
  {
    angle = ggo::ease_inout(_counter, _anim2_counter_start, _anim2_counter_end, _animations[0]._end_angle, _animations[1]._end_angle);
  }
  else if (_counter <= _anim3_counter_start)
  {
    angle = _animations[1]._end_angle;
  }
  else if (_counter <= _anim3_counter_end)
  {
    angle = ggo::ease_inout(_counter, _anim3_counter_start, _anim3_counter_end, _animations[1]._end_angle, _animations[2]._end_angle);
  }
  else
  {
    angle = _animations[2]._end_angle;
  }

  // Boxes animation.
  float main_size = _animations[0]._size;

  // Animation 2.
  if (_counter >= _anim2_counter_start)
  {
    int counter = std::min(_counter, _anim2_counter_end);
    float size = ggo::ease_inout(counter, _anim2_counter_start, _anim2_counter_end, main_size, main_size + _animations[1]._size);
    auto shape_color = from_8u(_animations[1]._color);

    scene.make_paint_shape_t(ggo::oriented_box_f(_pos, angle, main_size, size), shape_color);
  }

  // Animation 3.
  if (_counter >= _anim3_counter_start)
  {
    int counter = std::min(_counter, _anim3_counter_end);
    float size = ggo::ease_inout(counter, _anim3_counter_start, _anim3_counter_end, main_size, main_size + _animations[2]._size);
    auto shape_color = from_8u(_animations[2]._color);

    scene.make_paint_shape_t(ggo::oriented_box_f(_pos, angle + ggo::pi<float>() / 2, main_size, size), shape_color);
  }

  // Animation 1. Must be last.
  if (_counter >= _anim1_counter_start)
  {
    int counter = std::min(_counter, _anim1_counter_end);
    float size = ggo::ease_inout(counter, _anim1_counter_start, _anim1_counter_end, 0.f, main_size);
    auto shape_color = from_8u(_animations[0]._color);

    scene.make_paint_shape_t(ggo::oriented_box_f(_pos, angle + ggo::pi<float>() / 2, main_size, size), shape_color);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
bool ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco2::finished() const
{
  return _counter > _anim3_counter_end;
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco3::demeco3(const ggo::pos2_f & pos, float radius, int counter, const palette_t & palette)
:
demeco(pos, radius, counter)
{
  demeco_artist<pixel_type, memory_lines_order, sampling>::palette_t palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  int count = ggo::rand(16, 24);
  float radius_sup = radius * ggo::rand(0.7f,0.85f);
  float radius_inf = radius_sup * ggo::rand(0.6f, 0.8f);

  for (int l = 0; l < _layers_count; ++l)
  {
    float aperture = 2 * ggo::pi<float>() / count;
    float angle_start = ggo::rand(0.f, ggo::pi<float>());

    for (int i = 0; i < count; ++i)
    {
      float angle = angle_start + i * 2 * ggo::pi<float>() / count;
      int delay = _layer_delay * (_layers_count - l - 1);

      _peaks.emplace_back(angle, aperture, radius_inf, radius_sup, palette_shuffled[l], delay);
    }

    count -= ggo::rand(2, 5);
    radius_sup = radius_inf * ggo::rand(0.7f, 1.1f);
    radius_inf = radius_sup * ggo::rand(0.6f, 0.8f);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco3::get_paint_shapes(scene_t & scene) const
{
  auto curve = [&](int counter)
  {
    if (counter < _anim1_duration)
    {
      return ggo::ease_inout(counter, _anim1_duration, 0.f, 1.2f);
    }
    else if (counter < _anim1_duration + _anim2_duration)
    {
      return ggo::ease_inout(counter, _anim1_duration, _anim1_duration + _anim2_duration, 1.2f, 1.f);
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

    ggo::pos2_f p0 = _pos;
    ggo::pos2_f p1 = _pos + radius_inf * vec2_f::from_angle(peak._angle - 0.5f * peak._aperture);
    ggo::pos2_f p2 = _pos + radius_sup * vec2_f::from_angle(peak._angle);
    ggo::pos2_f p3 = _pos + radius_inf * vec2_f::from_angle(peak._angle + 0.5f * peak._aperture);

    ggo::polygon2d_f polygon({ p0, p1, p2, p3 });
    auto shape_color = from_8u(peak._color);

    scene.make_paint_shape_t(polygon, shape_color);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
bool ggo::demeco_artist<pixel_type, memory_lines_order, sampling>::demeco3::finished() const
{
  return _counter > _layer_delay * (_layers_count - 1) + _anim1_duration + _anim2_duration;
}


