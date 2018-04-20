#include "ggo_newton_artist.h"
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/ggo_blit.h>

//////////////////////////////////////////////////////////////
ggo::newton_artist::newton_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt)
:
animation_artist_abc(width, height, line_step, format, rt)
{
  _background.reset(new uint8_t[line_step * height]);
}

//////////////////////////////////////////////////////////////
void ggo::newton_artist::init_animation()
{
  _frame_index = -1;

  _hue = ggo::rand<float>();

  _shadow_offset = ggo::vec2f(ggo::rand<float>(-0.01f, 0.01f) * get_min_size(), ggo::rand<float>(0.02f, 0.05f) * get_min_size());

  _newtons.clear();

  while (_newtons.size() < 10)
  {
    ggo::newton_artist::newton newton;

    ggo::pos2f pos(ggo::rand<float>() * get_width(), ggo::rand<float>() * get_height());
    newton._cur_pos = newton._prv_pos = pos;
    newton._weight = ggo::rand<float>(1, 2);
    newton._timer = 0;
    if (ggo::rand_bool())
    {
      newton._timer = ggo::rand<int>(0, 200);
    }

    _newtons.push_back(newton);
  }

  while (_strings.size() < 25)
  {
    int index1 = ggo::rand<int>(0, static_cast<int>(_newtons.size()) - 1);
    int index2 = ggo::rand<int>(0, static_cast<int>(_newtons.size()) - 1);

    if ((index1 != index2) && (find_string(index1, index2) == false))
    {
      ggo::newton_artist::string string;
      string._index1 = index1;
      string._index2 = index2;
      string._length = ggo::rand<float>(0.1f, 0.5f) * get_min_size();

      _strings.push_back(string);
    }
  }

  // Render background.
  const ggo::color_8u color1 = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(0, 0.5), 1);
  const ggo::color_8u color2 = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(0, 0.5), 1);
  const ggo::color_8u color3 = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(0, 0.5), 1);
  const ggo::color_8u color4 = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(0, 0.5), 1);

  ggo::fill_4_colors<ggo::rgb_8u_yu>(_background.get(), get_width(), get_height(), get_line_step(),
    color1, color2, color3, color4, ggo::rect_int::from_width_height(get_width(), get_height()));

  std::unique_ptr<uint8_t> perlin_buffer(new uint8_t[get_width() * get_height()]);
  ggo::fill_perlin<ggo::y_8u_yu>(perlin_buffer.get(), get_width(), get_height(), get_width(),
    0.4f * get_min_size(), uint8_t(0), uint8_t(192));
  
  for (int y = 0; y < get_height(); ++y)
  {
    uint8_t * ptr = _background.get() + get_line_step();

    for (int x = 0; x < get_width(); ++x)
    {
      float val = *(perlin_buffer.get() + y * get_width() + x) / 255.f;

      ptr[3 * x + 0] = ggo::round_to<uint8_t>(val * ptr[3 * x + 0]);
      ptr[3 * x + 1] = ggo::round_to<uint8_t>(val * ptr[3 * x + 1]);
      ptr[3 * x + 2] = ggo::round_to<uint8_t>(val * ptr[3 * x + 2]);
    }
  }
}

//////////////////////////////////////////////////////////////
bool ggo::newton_artist::prepare_frame()
{
  // Remove some strings at random.
  if ((_frame_index < 300) && (ggo::rand<int>(0, 50) == 0))
  {
    _strings.erase(_strings.begin() + ggo::rand<int>(0, static_cast<int>(_strings.size()) - 1));
  }

  newton_update();

  // Look for maximum velocity.
  float max_velocity = 0;
  for (const auto & newton_item : _newtons)
  {
    float velocity = ggo::distance(newton_item._prv_pos, newton_item._cur_pos);

    max_velocity = std::max(max_velocity, velocity);
  }

  _max_velocities.push_back(max_velocity);

  if (_max_velocities.size() < 25)
  {
    return true;
  }
  else
  {
    _max_velocities.erase(_max_velocities.begin());
  }

  max_velocity = 0;
  for (std::vector<float>::const_iterator it = _max_velocities.begin(); it != _max_velocities.end(); ++it)
  {
    max_velocity = std::max(max_velocity, *it);
  }

  return max_velocity > 0.0001 * get_min_size();
}

//////////////////////////////////////////////////////////////
void ggo::newton_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  memcpy(buffer, _background.get(), get_width() * get_line_step());

  newton_paint(buffer);
}

//////////////////////////////////////////////////////////////
bool ggo::newton_artist::find_string(int index1, int index2) const
{
  for (const auto & string : _strings)
  {
    if ((string._index1 == index1) && (string._index2 == index2))
    {
      return true;
    }

    if ((string._index1 == index2) && (string._index2 == index1))
    {
      return true;
    }
  }

  return false;
}

//////////////////////////////////////////////////////////////
void ggo::newton_artist::newton_update()
{
  ggo::vec2f gravity(0.f, 0.001f * get_min_size());

  // Computes forces.
  for (int i = 0; i < _newtons.size(); ++i)
  {
    ggo::newton_artist::newton & newton_item = _newtons[i];

    newton_item._forces = ggo::vec2f(0.f, 0.f);//gravity;

    if (newton_item._timer > 0)
    {
      --newton_item._timer;
      continue;
    }

    // Look for a string.
    for (const auto & string_item : _strings)
    {
      auto it_diff = _newtons.cend();

      if (string_item._index1 == i)
      {
        it_diff = _newtons.begin() + string_item._index2;
      }
      if (string_item._index2 == i)
      {
        it_diff = _newtons.begin() + string_item._index1;
      }

      // Compute string force.
      if (it_diff != _newtons.end())
      {
        ggo::vec2f diff = it_diff->_cur_pos - newton_item._cur_pos;
        float length = diff.get_length();
        diff /= length; // Normalize.

        newton_item._forces += 0.02f * diff * std::min(0.025f * get_min_size(), length - string_item._length);
      }
    }
  }

  // Apply Newton.
  for (auto & newton_item : _newtons)
  {
    ggo::pos2f nxt_pos = newton_item._weight * newton_item._forces + 2.f * newton_item._cur_pos - newton_item._prv_pos;

    // Add friction.
    ggo::vec2f diff(nxt_pos - newton_item._cur_pos);
    diff *= 0.95f;
    nxt_pos = newton_item._cur_pos + diff;

    newton_item._prv_pos = newton_item._cur_pos;
    newton_item._cur_pos = nxt_pos;

    // Border bound.
  /*	if (newton._cur_pos.x() < 0)
    {
      newton._prv_pos.x() = -newton._prv_pos.x();
      newton._cur_pos.x() = -newton._cur_pos.x();
    }

    if (newton._cur_pos.x() > get_width())
    {
      newton._prv_pos.x() = 2 * get_width() - newton._prv_pos.x();
      newton._cur_pos.x() = 2 * get_width() - newton._cur_pos.x();
    }

    if (newton._cur_pos.y < 0)
    {
      newton._prv_pos.y = -newton._prv_pos.y;
      newton._cur_pos.y = -newton._cur_pos.y;
    }

    if (newton._cur_pos.y > get_height())
    {
      newton._prv_pos.y = 2 * get_height() - newton._prv_pos.y;
      newton._cur_pos.y = 2 * get_height() - newton._cur_pos.y;
    }*/
  }
}

//////////////////////////////////////////////////////////////
void ggo::newton_artist::newton_paint(void * buffer) const
{
  // Painting shadow.
  std::vector<uint8_t> shadow_buffer(get_width() * get_height(), 0xff);

  for (const auto & string : _strings)
  {
    const ggo::pos2f & p1 = _newtons[string._index1]._cur_pos + _shadow_offset;
    const ggo::pos2f & p2 = _newtons[string._index2]._cur_pos + _shadow_offset;

    ggo::polygon2d_float line;
    ggo::polygon2d_float::create_oriented_box((p1 + p2) / 2.f, p2 - p1, ggo::distance(p1, p2) / 2.f, 0.005f * get_min_size(), line);

    float dist = ggo::distance(p1, p2);
    float coef = 1 - 1 / (1 + 25 * std::fabs(dist - string._length) / get_min_size());
    float opacity = 0.25f + 0.75f * coef;
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_2x2>(
      shadow_buffer.data(), get_width(), get_height(), get_line_step(),
      line, ggo::make_solid_brush(uint8_t(0)), ggo::alpha_blender_y8u(opacity));
  }

  for (const auto & newton_item : _newtons)
  {
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_2x2>(
      shadow_buffer.data(), get_width(), get_height(), get_line_step(),
      ggo::disc_float(newton_item._cur_pos + _shadow_offset, 0.025f * get_min_size()), uint8_t(0));
  }

  ggo::gaussian_blur2d_mirror<ggo::y_8u_yu>(
    shadow_buffer.data(), get_width(), get_height(), get_line_step(), 0.05f * get_min_size());

  ggo::blit<ggo::y_8u_yu, ggo::rgb_8u_yu>(
    shadow_buffer.data(), get_width(), get_height(), get_line_step(),
    buffer, get_width(), get_height(), get_line_step(), 0, 0);

  // Paint shapes.
  for (const auto & string_item : _strings)
  {
    const ggo::pos2f & p1 = _newtons[string_item._index1]._cur_pos;
    const ggo::pos2f & p2 = _newtons[string_item._index2]._cur_pos;

    ggo::polygon2d_float line;
    ggo::polygon2d_float::create_oriented_box((p1 + p2) / 2.f, p2 - p1, ggo::distance(p1, p2) / 2, 0.005f * get_min_size(), line);

    float dist = ggo::distance(p1, p2);
    float coef = 1 - 1 / (1 + 25 * std::fabs(dist - string_item._length) / get_min_size());
    float opacity = 0.25f + 0.75f * coef;

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      line, ggo::make_solid_brush(ggo::white<ggo::color_8u>()), ggo::alpha_blender_rgb8u(opacity));
  }

  for (const auto & newton_item : _newtons)
  {
    ggo::disc_float disc;
    disc.center() = newton_item._cur_pos;
    disc.radius() = 0.025f * get_min_size();
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(),
      disc, ggo::black<ggo::color_8u>());

    float velocity = ggo::distance(newton_item._cur_pos, newton_item._prv_pos) / get_min_size();
    float coef = 1 - 1 / (1 + 500 * std::fabs(velocity));
    float sat = 1 - 0.5f * coef;
    float val = 0.25f + 0.75f * coef;
    disc.radius() = 0.02f * get_min_size();
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(), disc,
      ggo::from_hsv<ggo::color_8u>(_hue, sat, val));

    disc.center().y() -= 0.0125f * get_min_size();
    disc.radius() = 0.002f * get_min_size();
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(), disc,
      ggo::make_solid_brush(ggo::white<ggo::color_8u>()), ggo::alpha_blender_rgb8u(0.75f));
  }
}


