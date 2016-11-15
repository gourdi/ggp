#include "ggo_rediff_animation_artist.h"
#include <ggo_buffer_access.h>
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::rediff_animation_artist::rediff_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
accumulation_animation_artist_abc(width, height, line_step, pbf),
_feed_map(width * height),
_kill_map(width * height),
_food(width * height),
_life(width * height)
{

}

//////////////////////////////////////////////////////////////
void ggo::rediff_animation_artist::init_sub()
{
  _hue = ggo::rand<float>();

  _food_stddev = 0.005f * get_min_size();
  _life_stddev = ggo::rand<float>(0.4f, 0.6f) * _food_stddev;

  float feed_rate1 = ggo::rand<float>(0.02f, 0.06f);
  float feed_rate2 = ggo::rand<float>(0.02f, 0.06f);
  float kill_rate1 = ggo::rand<float>(0.05f, 0.07f);
  float kill_rate2 = ggo::rand<float>(0.05f, 0.07f);

  for_each_pixel([&](int x, int y)
  {
    float dist = ggo::distance(get_center(), ggo::pos2f(ggo::to<float>(x), ggo::to<float>(y))) / get_min_size();

    set2d(_feed_map.data(), x, y, ggo::map(dist, 0.f, 1.f, feed_rate1, feed_rate2));
    set2d(_kill_map.data(), x, y, ggo::map(dist, 0.f, 1.f, kill_rate1, kill_rate2));
  });

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
void ggo::rediff_animation_artist::init_output_buffer(void * buffer) const
{
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());
}

//////////////////////////////////////////////////////////////
void ggo::rediff_animation_artist::update()
{
  std::vector<float> diffused_food;
  std::vector<float> diffused_life;

  for (int i = 0; i < 16; ++i)
  {
    diffused_food = _food;
    diffused_life = _life;
    ggo::gaussian_blur2d_loop<ggo::y_32f_yu>(_food.data(), get_width(), get_height(), sizeof(float) * get_width(), _food_stddev);
    ggo::gaussian_blur2d_loop<ggo::y_32f_yu>(_life.data(), get_width(), get_height(), sizeof(float) * get_width(), _life_stddev);

    for_each_pixel([&](int x, int y)
    {
      const float feed_rate = get2d(_feed_map.data(), x, y);
      const float kill_rate = get2d(_kill_map.data(), x, y);
      const float reaction = get2d(_food.data(), x, y) * ggo::square(get2d(_life.data(), x, y));
      const float food = get2d(diffused_food.data(), x, y) - reaction + feed_rate * (1.f - get2d(_food.data(), x, y));
      const float life = get2d(diffused_life.data(), x, y) + reaction - (kill_rate + feed_rate) * get2d(_life.data(), x, y);

      set2d(_food.data(), x, y, food);
      set2d(_life.data(), x, y, life);
    });
  }
}

//////////////////////////////////////////////////////////////
void ggo::rediff_animation_artist::render(void * buffer) const
{
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

  for_each_pixel([&](int x, int y)
  {
    // Compute the normal by computing 4 point around the current pixel.
    ggo::pos3f p_bl(0.f, 0.f, 0.45f * get2d(_life.data(), x, y) + 0.2f * (get2d_loop(_life.data(), x - 1, y) + get2d_loop(_life.data(), x, y - 1)) + 0.15f * get2d_loop(_life.data(), x - 1, y - 1)); // Bottom left
    ggo::pos3f p_tr(1.f, 1.f, 0.45f * get2d(_life.data(), x, y) + 0.2f * (get2d_loop(_life.data(), x + 1, y) + get2d_loop(_life.data(), x, y + 1)) + 0.15f * get2d_loop(_life.data(), x + 1, y + 1)); // Top right
    ggo::vec3f v1(p_tr - p_bl);

    ggo::pos3f p_tl(0.f, 1.f, 0.45f * get2d(_life.data(), x, y) + 0.2f * (get2d_loop(_life.data(), x - 1, y) + get2d_loop(_life.data(), x, y + 1)) + 0.15f * get2d_loop(_life.data(), x - 1, y + 1)); // Top left.
    ggo::pos3f p_br(1.f, 0.f, 0.45f * get2d(_life.data(), x, y) + 0.2f * (get2d_loop(_life.data(), x + 1, y) + get2d_loop(_life.data(), x, y - 1)) + 0.15f * get2d_loop(_life.data(), x + 1, y - 1)); // Bottom right.
    ggo::vec3f v2(p_tl - p_br);

    ggo::vec3f normal = ggo::cross(v1, v2);
    float dot = -ggo::dot(normal.get_normalized(), light_dir);

    // Apply lighting.
    float sat = sat_map.evaluate(get2d(_life.data(), x, y));
    float val = val_map.evaluate(get2d(_life.data(), x, y));

    ggo::color_32f pixel = ggo::from_hsv<ggo::color_32f>(_hue, sat, val);
    pixel += 0.8f * std::pow(dot, 1200.f) * ggo::white<ggo::color_32f>();

    ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), ggo::convert_color_to<ggo::color_8u>(pixel));
  });
}

//////////////////////////////////////////////////////////////
bool ggo::rediff_animation_artist::render_next_frame_acc(void * buffer, int frame_index)
{
  if (frame_index > 350)
  {
    return false;
  }

  update();
  
  if (buffer != nullptr)
  {
    render(buffer);
  }

  return true;
}
