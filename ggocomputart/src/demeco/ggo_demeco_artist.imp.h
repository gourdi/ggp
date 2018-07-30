#include <2d/ggo_blit.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
typename ggo::image_format_traits<format>::color_t ggo::demeco_artist<format, sampling>::from_8u(const ggo::color_8u & c)
{
  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;

  if constexpr(std::is_same<color_t, ggo::color_8u>::value)
  {
    return c;
  }
  else if constexpr(std::is_same<color_t, ggo::alpha_color_8u>::value)
  {
    return alpha_color_8u(c, 255);
  }
  else
  {
    static_assert(false);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
ggo::demeco_artist<format, sampling>::demeco_artist(int width, int height)
:
_width(width), _height(height),
_background_image(width, height)
{
  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;

  // Create palette.
  for (auto & c : _palette)
  {
    c = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand(0.3f, 1.0f), ggo::rand(0.7f, 0.9f));
  }

  // Create demecos.
  const float demeco_radius = min_size() * ggo::rand(0.04f, 0.08f);
  const float main_radius = 0.5f * min_size() - demeco_radius;

  auto create_demeco = [&](float x, float y)
  {
    float radius = ggo::distance(x, y);

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
        demeco.reset(new demeco1(center() + pos2f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
        break;
      case 1:
        demeco.reset(new demeco2(center() + pos2f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
        break;
      case 2:
        demeco.reset(new demeco3(center() + pos2f(x, y), demeco_radius, ggo::rand<int>(-200, 0), _palette));
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
  if constexpr(std::is_same<color_t, ggo::color_8u>::value)
  {
    _background_image.fill(white_8u());
  }
  else if constexpr(std::is_same<color_t, ggo::alpha_color_8u>::value)
  {
    _background_image.fill({ 0, 0, 0, 0 });
  }
  else
  {
    static_assert(false);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
bool ggo::demeco_artist<format, sampling>::update()
{
  bool finished = true;
  
  _background_paint_shapes.clear();
  _active_paint_shapes.clear();

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
      auto shapes = demeco->get_paint_shapes();

      demeco->_counter++;

      auto & paint_shapes_dst = demeco->finished() ? _background_paint_shapes : _active_paint_shapes;
      for (auto & shape : shapes)
      {
        paint_shapes_dst.push_back(std::move(shape));
      }

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
template <ggo::image_format format, ggo::sampling sampling>
void ggo::demeco_artist<format, sampling>::render_tile(void * buffer, int line_byte_step, int frame_index, const ggo::rect_int & clipping)
{
  GGO_ASSERT_EQ(_width, _background_image.width());
  GGO_ASSERT_EQ(_height, _background_image.height());

  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;

  // Render the background demecos.
  auto adaptator_background = make_adaptor(_background_paint_shapes, [](const auto & item) { return item.get(); });
  paint_shapes<format, sampling>(
    _background_image.data(), _background_image.width(), _background_image.height(), _background_image.line_byte_step(),
    adaptator_background, clipping);

  // Blit the background.
  const int line_size = clipping.width() * format_traits::pixel_byte_size;
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * src = get_pixel_ptr<format>(_background_image.data(), clipping.left(), y, _background_image.height(), _background_image.line_byte_step());
    void * dst = get_pixel_ptr<format>(buffer, clipping.left(), y, _height, line_byte_step);

    memcpy(dst, src, line_size);
  }

  // Render the active demecos.
  auto adaptator_active = make_adaptor(_active_paint_shapes, [](const auto & item) { return item.get(); });
  paint_shapes<format, sampling>(buffer, _width, _height, line_byte_step, adaptator_active, clipping);
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
ggo::demeco_artist<format, sampling>::demeco1::demeco1(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette)
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
  demeco_artist<format, sampling>::palette_t palette_shuffled = palette;
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
template <ggo::image_format format, ggo::sampling sampling>
typename ggo::demeco_artist<format, sampling>::paint_shapes_t ggo::demeco_artist<format, sampling>::demeco1::get_paint_shapes() const
{
  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;
  using paint_shape_t = static_paint_shape<multi_shape_float, color_t, solid_color_brush<color_t>, alpha_blender<color_t>>;

  int angle_counter = std::min(_counter, _angle_counter_max);
  float max_angle = std::min(ggo::ease_inout(angle_counter, _angle_counter_max, 0.f, 2.f * ggo::pi<float>()), 2.f * ggo::pi<float>());

  int fade_in_counter = std::min(_counter, _fade_in_counter_max);
  float opacity = ggo::ease_inout(fade_in_counter, _fade_in_counter_max, 0.f, 1.f);

  constexpr int discs_count = 96;

  paint_shapes_t shapes;

  for (const auto & arc : _arcs)
  {
    auto blender = ggo::alpha_blender<color_t>(opacity);
    auto paint_arc = std::make_unique<paint_shape_t>(multi_shape_float(), from_8u(arc._color), blender);

    for (int i = 0; i < discs_count; ++i)
    {
      float angle = arc._start_angle + (arc._ccw ? 1.f : -1) * i * max_angle / discs_count;

      paint_arc->_shape.add_shape(std::make_shared<ggo::disc_float>(_pos + from_polar(angle, arc._radius), arc._width / 2));
    }

    shapes.push_back(std::move(paint_arc));
  }

  return shapes;
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
bool ggo::demeco_artist<format, sampling>::demeco1::finished() const
{
  return _counter > std::max(demeco1::_angle_counter_max, demeco1::_fade_in_counter_max);
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
ggo::demeco_artist<format, sampling>::demeco2::demeco2(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette)
:
demeco(pos, radius, counter)
{
  // Pick up 3 colors.
  demeco_artist<format, sampling>::palette_t palette_shuffled = palette;
  ggo::shuffle(palette_shuffled);

  _animations.emplace_back(palette_shuffled[0], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[1], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));
  _animations.emplace_back(palette_shuffled[2], radius * ggo::rand(0.25f, 0.4f), ggo::rand(0.f, ggo::pi<float>()));

  _init_angle = ggo::rand(0.f, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
typename ggo::demeco_artist<format, sampling>::paint_shapes_t ggo::demeco_artist<format, sampling>::demeco2::get_paint_shapes() const
{
  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;

  using paint_box = static_paint_shape<ggo::oriented_box_float, color_t>;
  paint_shapes_t paint_boxes;

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
  ggo::vec2f dir1 = ggo::from_polar(angle, 1.f);
  ggo::vec2f dir2{ dir1.y(), -dir1.x() };

  float main_size = _animations[0]._size;

  // Animation 2.
  if (_counter >= _anim2_counter_start)
  {
    int counter = std::min(_counter, _anim2_counter_end);
    float size = ggo::ease_inout(counter, _anim2_counter_start, _anim2_counter_end, main_size, main_size + _animations[1]._size);
    auto shape_color = from_8u(_animations[1]._color);

    paint_boxes.emplace_back(std::make_unique<paint_box>(ggo::oriented_box_float(_pos, dir1, main_size, size), shape_color));
  }

  // Animation 3.
  if (_counter >= _anim3_counter_start)
  {
    int counter = std::min(_counter, _anim3_counter_end);
    float size = ggo::ease_inout(counter, _anim3_counter_start, _anim3_counter_end, main_size, main_size + _animations[2]._size);
    auto shape_color = from_8u(_animations[2]._color);

    paint_boxes.emplace_back(std::make_unique<paint_box>(ggo::oriented_box_float(_pos, dir2, main_size, size), shape_color));
  }

  // Animation 1. Must be last.
  if (_counter >= _anim1_counter_start)
  {
    int counter = std::min(_counter, _anim1_counter_end);
    float size = ggo::ease_inout(counter, _anim1_counter_start, _anim1_counter_end, 0.f, main_size);
    auto shape_color = from_8u(_animations[0]._color);

    paint_boxes.emplace_back(std::make_unique<paint_box>(ggo::oriented_box_float(_pos, dir2, main_size, size), shape_color));
  }

  return paint_boxes;
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
bool ggo::demeco_artist<format, sampling>::demeco2::finished() const
{
  return _counter > _anim3_counter_end;
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
ggo::demeco_artist<format, sampling>::demeco3::demeco3(const ggo::pos2f & pos, float radius, int counter, const palette_t & palette)
:
demeco(pos, radius, counter)
{
  demeco_artist<format, sampling>::palette_t palette_shuffled = palette;
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
template <ggo::image_format format, ggo::sampling sampling>
typename ggo::demeco_artist<format, sampling>::paint_shapes_t ggo::demeco_artist<format, sampling>::demeco3::get_paint_shapes() const
{
  using format_traits = image_format_traits<format>;
  using color_t = format_traits::color_t;

  using paint_polygon = static_paint_shape<ggo::polygon2d_float, color_t>;
  paint_shapes_t paint_polygons;

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

    ggo::pos2f p0 = _pos;
    ggo::pos2f p1 = _pos + from_polar(peak._angle - 0.5f * peak._aperture, radius_inf);
    ggo::pos2f p2 = _pos + from_polar(peak._angle, radius_sup);
    ggo::pos2f p3 = _pos + from_polar(peak._angle + 0.5f * peak._aperture, radius_inf);

    ggo::polygon2d_float polygon({ p0, p1, p2, p3 });
    auto shape_color = from_8u(peak._color);

    paint_polygons.emplace_back(std::make_unique<paint_polygon>(polygon, shape_color));
  }

  return paint_polygons;
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling sampling>
bool ggo::demeco_artist<format, sampling>::demeco3::finished() const
{
  return _counter > _layer_delay * (_layers_count - 1) + _anim1_duration + _anim2_duration;
}


