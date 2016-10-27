#include "ggo_alpha_animation_artist.h"
#include <ggo_array.h>
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_dct.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

namespace
{
  const int oscillo_size = 750;
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  alpha_animation_artist::alpha_animation_artist(int render_width, int render_height)
  :
  animation_artist_abc(render_width, render_height)
  {

  }

  //////////////////////////////////////////////////////////////
  void alpha_animation_artist::init_sub()
  {
    _remaining_counter = 7;
    _creation_counter = ggo::rand<int>(40, 60);

    _items.clear();

    add_new_item();

    _bkgd_color1 = ggo::color_8u(ggo::rand<uint8_t>());
    _bkgd_color2 = ggo::color_8u(ggo::rand<uint8_t>());
    _bkgd_color3 = ggo::color_8u(ggo::rand<uint8_t>());
    _bkgd_color4 = ggo::color_8u(ggo::rand<uint8_t>());
  }

  //////////////////////////////////////////////////////////////
  void alpha_animation_artist::add_new_item()
  {
    float margin = 0.1f * get_render_min_size();

    ggo::pos2f center = get_random_point(margin);

    // Try to create an item far away from the other already created items.
    if (_items.empty() == false)
    {
      float dist = 0;
      for (int i = 0; i < 5; ++i)
      {
        ggo::pos2f center_cur = get_random_point(margin);
        float dist_cur = ggo::distance(_items.front().get_center(), center_cur);
        for (const auto & item : _items)
        {
          dist_cur = std::min(dist_cur, ggo::distance(item.get_center(), center_cur));
        }

        if (dist_cur > dist)
        {
          dist = dist_cur;
          center = center_cur;
        }
      }
    }

    float inner_radius = ggo::rand<float>(0.04f, 0.08f) * get_render_min_size();
    float outter_radius = inner_radius + ggo::rand<float>(0.04f, 0.08f) * get_render_min_size();
    float hue = ggo::rand<float>();
    float sat = ggo::rand<float>(0.5, 1);
    float val = ggo::rand<float>(0.5, 1);
    _items.push_back(item(center, inner_radius, outter_radius, hue, sat, val));
  }

  //////////////////////////////////////////////////////////////
  bool alpha_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
  {
    if (buffer != nullptr)
    {
      ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
        _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4);
    }

    // Oscillos.
    ggo::remove_if(_oscillos, [&](oscillo & oscil) { return oscil.update() == false; });

    if (buffer != nullptr)
    {
      for (const auto & oscillo : _oscillos)
      {
        oscillo.draw(buffer, get_render_width(), get_render_height());
      }
    }

    if ((_remaining_counter > 0) && (_oscillos.size() < 5) && (ggo::rand<int>(0, 2) == 0))
    {
      float y = ggo::rand<float>(0.2f, 0.8f) * get_render_height();
      float dy = ggo::rand<float>(-0.01f, 0.01f) * get_render_min_size();

      if (_oscillos.empty() == false)
      {
        int index = ggo::rand<int>(0, static_cast<int>(_oscillos.size()) - 1);
        const oscillo & src = _oscillos[index];

        if ((src.y() > 0.2 * get_render_height()) && (src.y() < 0.8 * get_render_height()))
        {
          y = src.y();
        }
      }

      _oscillos.push_back(oscillo(y, dy));
    }

    // Items.
    ggo::remove_if(_items, [&](item & item) { return item.update(get_render_width(), get_render_height()) == false; });

    if (buffer != nullptr)
    {
      for (const auto & item : _items)
      {
        item.draw(buffer, get_render_width(), get_render_height());
      }
    }

    if (_creation_counter > 0)
    {
      --_creation_counter;

      if ((_creation_counter == 0) && (_remaining_counter > 0))
      {
        --_remaining_counter;

        _creation_counter = ggo::rand<int>(40, 60);

        add_new_item();
      }
    }

    return _remaining_counter > 0 || !_items.empty() || !_oscillos.empty();
  }

  //////////////////////////////////////////////////////////////
  // ggo_oscillo

  //////////////////////////////////////////////////////////////
  alpha_animation_artist::oscillo::oscillo(float y, float dy)
  {
    _y = y;
    _dy = dy;
    _threshold = ggo::rand<int>(oscillo_size / 6, oscillo_size / 3);
    _counter = ggo::rand<int>(40, 50);
    _opacity = 0;
  }

  //////////////////////////////////////////////////////////////
  bool alpha_animation_artist::oscillo::update()
  {
    --_counter;

    if (_counter <= 0)
    {
      _opacity -= 0.05f;
    }
    else
    {
      _opacity = std::min(0.5f, _opacity + 0.1f);
    }

    _y += _dy;

    return _opacity > 0;
  }

  //////////////////////////////////////////////////////////////
  void alpha_animation_artist::oscillo::draw(void * buffer, int width, int height) const
  {
    int min_size = std::min(width, height);

    ggo::array_float freq(oscillo_size, 0.f), spat(oscillo_size, 0.f);
    for (int i = 0; i < _threshold; ++i)
    {
      freq(i) = ggo::to<float>(ggo::rand<int>(-1, 1));
    }
    ggo::dct(freq.data(), spat.data(), oscillo_size);

    ggo::multi_shape_float multi_shape;
    for (int i = 1; i < oscillo_size; ++i)
    {
      float x1 = (i - 1) * width / float(oscillo_size);
      float y1 = _y + 0.025f * spat(i - 1) * min_size;

      float x2 = i * width / float(oscillo_size);
      float y2 = _y + 0.025f * spat(i) * min_size;

      multi_shape.add_shape(std::make_shared<ggo::extended_segment_float>(ggo::pos2f(x1, y1), ggo::pos2f(x2, y2), 0.001f * min_size));
    }

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, width, height, 3 * width, multi_shape,
      ggo::make_solid_brush(ggo::color_8u::black),
      ggo::alpha_blender<ggo::color_8u>(_opacity));
  }

  //////////////////////////////////////////////////////////////
  // ggo_item

  //////////////////////////////////////////////////////////////
  alpha_animation_artist::item::item(const ggo::pos2f & center, float inner_radius, float outter_radius, float hue, float sat, float val)
    :
    _center(center)
  {
    const int lines_count = 60;

    for (int i = 0; i < lines_count; ++i)
    {
      ggo::color_8u color = ggo::color_8u::from_hsv(hue, sat + ggo::rand<float>(-0.2f, 0.2f), val + ggo::rand<float>(-0.2f, 0.2f));

      _lines.push_back(line(center, 2 * ggo::pi<float>() * i / lines_count, inner_radius, outter_radius, color));
    }
  }

  //////////////////////////////////////////////////////////////
  bool alpha_animation_artist::item::update(int width, int height)
  {
    ggo::remove_if(_lines, [&](line & line) { return line.update(width, height) == false; });

    return !_lines.empty();
  }

  //////////////////////////////////////////////////////////////
  void alpha_animation_artist::item::draw(void * buffer, int width, int height) const
  {
    for (const line & line : _lines)
    {
      line.draw(buffer, width, height);
    }
  }

  //////////////////////////////////////////////////////////////
  // line

  //////////////////////////////////////////////////////////////
  alpha_animation_artist::line::line(const ggo::pos2f & center, float angle, float inner_radius, float outter_radius, const ggo::color_8u & color)
  {
    _center = center;
    _angle = angle;
    _inner_radius = inner_radius;
    _outter_radius = outter_radius;
    _color = color;
    _angle_offset = ggo::rand<float>(-2 * ggo::pi<float>(), 2 * ggo::pi<float>());
    _counter = ggo::rand<int>(-30, 0);
  }

  //////////////////////////////////////////////////////////////
  float alpha_animation_artist::line::get_start_factor() const
  {
    return 1 / std::pow(1.f + 0.75f * _counter, 1.5f);
  }

  //////////////////////////////////////////////////////////////
  bool alpha_animation_artist::line::update(int width, int height)
  {
    ++_counter;

    int min_size = std::min(width, height);
    float start_factor = get_start_factor();

    _opacity = 1.f - start_factor;
    _blur = 1.f + 0.1f * min_size * start_factor;

    float inner_radius = _inner_radius * (1 + 10 * start_factor);
    float outter_radius = _outter_radius * (1 + 10 * start_factor);
    float angle = _angle + _angle_offset * start_factor;

    ggo::vec2f offset1 = ggo::from_polar(angle, inner_radius);
    ggo::vec2f offset2 = ggo::from_polar(angle, outter_radius);

    const int END_COUNTER = 100;
    if (_counter >= END_COUNTER)
    {
      ggo::vec2f line_center = 0.5f * (offset1 + offset2);

      float end_factor = std::pow(0.1f * (_counter - END_COUNTER), 2.5f);
      float length = ggo::distance(offset1, offset2);
      float angle = atan2(line_center.get<1>(), line_center.get<0>());

      angle = _angle + end_factor;
      line_center *= (1 + end_factor);
      offset1 = line_center + ggo::from_polar(angle, length / 2);
      offset2 = line_center - ggo::from_polar(angle, length / 2);

      _opacity -= 0.1f * end_factor;
      if (_opacity <= 0)
      {
        return false;
      }
    }

    _p1 = _center + offset1;
    _p2 = _center + offset2;

    return true;
  }

  //////////////////////////////////////////////////////////////
  void alpha_animation_artist::line::draw(void * buffer, int width, int height) const
  {
    if (_counter >= 0)
    {
      int min_size = std::min(width, height);
      float line_width = 0.0015f * min_size + 0.1f * min_size * get_start_factor();

      ggo::extended_segment_float segment(_p1, _p2, line_width);

      ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
        buffer, width, height, 3 * width, segment,
        ggo::make_solid_brush(_color),
        ggo::alpha_blender<ggo::color_8u>(_opacity));
    }
  }
}
