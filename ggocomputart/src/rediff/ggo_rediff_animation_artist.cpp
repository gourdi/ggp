#include "ggo_rediff_animation_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::rediff_animation_artist::rediff_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_feed_map(width, height),
_kill_map(width, height),
_food(width, height),
_life(width, height)
{

}

//////////////////////////////////////////////////////////////
void ggo::rediff_animation_artist::init_animation()
{ 
  _frame_index = -1;

  _hue = ggo::rand<float>();

  _food_stddev = 0.005f * get_min_size();
  _life_stddev = ggo::rand<float>(0.4f, 0.6f) * _food_stddev;

  float feed_rate1 = ggo::rand<float>(0.02f, 0.06f);
  float feed_rate2 = ggo::rand<float>(0.02f, 0.06f);
  float kill_rate1 = ggo::rand<float>(0.05f, 0.07f);
  float kill_rate2 = ggo::rand<float>(0.05f, 0.07f);

  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      float dist = ggo::distance(get_center(), ggo::pos2f(static_cast<float>(x), static_cast<float>(y))) / get_min_size();

      _feed_map(x, y) = ggo::map(dist, 0.f, 1.f, feed_rate1, feed_rate2);
      _kill_map(x, y) = ggo::map(dist, 0.f, 1.f, kill_rate1, kill_rate2);
    }
  }

  std::fill(_food.begin(), _food.end(), 1.0f);
  std::fill(_life.begin(), _life.end(), 0.0f);

  for (int i = 0; i < 5; ++i)
  {
    ggo::paint_shape<ggo::y_32f_yu, ggo::sampling_4x4>(
      _life.data(), get_width(), get_height(), sizeof(float) * get_height(),
      ggo::disc_float(get_random_point(), 0.01f * get_min_size()), 1.0f);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::rediff_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > 350)
  {
    return false;
  }

  ggo::array<float, 2> diffused_food;
  ggo::array<float, 2> diffused_life;

  for (int i = 0; i < 16; ++i)
  {
    diffused_food = _food;
    diffused_life = _life;
    ggo::gaussian_blur2d_loop<ggo::y_32f_yu>(_food.data(), get_width(), get_height(), sizeof(float) * get_width(), _food_stddev);
    ggo::gaussian_blur2d_loop<ggo::y_32f_yu>(_life.data(), get_width(), get_height(), sizeof(float) * get_width(), _life_stddev);

    for (int y = 0; y < get_height(); ++y)
    {
      for (int x = 0; x < get_width(); ++x)
      {
        const float feed_rate = _feed_map(x, y);
        const float kill_rate = _kill_map(x, y);
        const float reaction = _food(x, y) * ggo::square(_life(x, y));
        const float food = diffused_food(x, y) - reaction + feed_rate * (1.f - _food(x, y));
        const float life = diffused_life(x, y) + reaction - (kill_rate + feed_rate) * _life(x, y);

        _food(x, y) = food;
        _life(x, y) = life;
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::rediff_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  if (buffer == nullptr)
  {
    return;
  }

  if (_frame_index == 0)
  {
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), clipping);
  }

  const float x1 = 0.8f;
  const float x2 = 0.16f;
  const float x3 = 0.24f;

  ggo::linear_curve<float, float> sat_map;
  sat_map.push_point(0.0f, 0.f);
  sat_map.push_point(x1, 0.f);
  sat_map.push_point(x2, 1.f);
  sat_map.push_point(x3, 1.f);
  sat_map.push_point(1.0f, 1.f);

  ggo::linear_curve<float, float> val_map;
  val_map.push_point(0.0f, 0.8f);
  val_map.push_point(x1, 0.8f);
  val_map.push_point(x2, 1.0f);
  val_map.push_point(x3, 0.0f);
  val_map.push_point(1.0f, 0.0f);

  ggo::vec3f light_dir(0.f, -0.05f, -1.f);
  light_dir.normalize();

  for (int y = 0; y < get_height(); ++y)
  {
    for (int x = 0; x < get_width(); ++x)
    {
      // Compute the normal by computing 4 point around the current pixel.
      ggo::pos3f p_bl(0.f, 0.f, 0.45f * _life(x, y) + 0.2f * (_life.at_loop(x - 1, y) + _life.at_loop(x, y - 1)) + 0.15f * _life.at_loop(x - 1, y - 1)); // Bottom left
      ggo::pos3f p_tr(1.f, 1.f, 0.45f * _life(x, y) + 0.2f * (_life.at_loop(x + 1, y) + _life.at_loop(x, y + 1)) + 0.15f * _life.at_loop(x + 1, y + 1)); // Top right
      ggo::vec3f v1(p_tr - p_bl);

      ggo::pos3f p_tl(0.f, 1.f, 0.45f * _life(x, y) + 0.2f * (_life.at_loop(x - 1, y) + _life.at_loop(x, y + 1)) + 0.15f * _life.at_loop(x - 1, y + 1)); // Top left.
      ggo::pos3f p_br(1.f, 0.f, 0.45f * _life(x, y) + 0.2f * (_life.at_loop(x + 1, y) + _life.at_loop(x, y - 1)) + 0.15f * _life.at_loop(x + 1, y - 1)); // Bottom right.
      ggo::vec3f v2(p_tl - p_br);

      ggo::vec3f normal = ggo::cross(v1, v2);
      float dot = -ggo::dot(normal.get_normalized(), light_dir);

      // Apply lighting.
      float sat = sat_map.evaluate(_life(x, y));
      float val = val_map.evaluate(_life(x, y));

      ggo::color_32f pixel = ggo::from_hsv<ggo::color_32f>(_hue, sat, val);
      pixel += 0.8f * std::pow(dot, 1200.f) * ggo::white<ggo::color_32f>();

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), ggo::convert_color_to<ggo::color_8u>(pixel));
    }
  }
}
