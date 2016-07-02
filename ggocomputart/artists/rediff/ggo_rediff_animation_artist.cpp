#include "ggo_rediff_animation_artist.h"
#include <ggo_buffer_access.h>
#include <ggo_gaussian_blur.h>
#include <ggo_fill.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo_rediff_animation_artist::ggo_rediff_animation_artist(int render_width, int render_height)
:
ggo_accumulation_animation_artist_abc(render_width, render_height),
_feed_map(render_width, render_height),
_kill_map(render_width, render_height),
_food(render_width, render_height),
_life(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_rediff_animation_artist::init_sub()
{
  _hue = ggo::rand_float();

  _food_stddev = 0.005f * get_render_min_size();
  _life_stddev = ggo::rand_float(0.4f, 0.6f) * _food_stddev;

  float feed_rate1 = ggo::rand_float(0.02f, 0.06f);
  float feed_rate2 = ggo::rand_float(0.02f, 0.06f);
  float kill_rate1 = ggo::rand_float(0.05f, 0.07f);
  float kill_rate2 = ggo::rand_float(0.05f, 0.07f);

  for_each_pixel([&](int x, int y)
  {
    float dist = ggo::distance(get_center(), ggo::pos2f(ggo::to<float>(x), ggo::to<float>(y))) / get_render_min_size();

    _feed_map(x, y) = ggo::map(dist, 0.f, 1.f, feed_rate1, feed_rate2);
    _kill_map(x, y) = ggo::map(dist, 0.f, 1.f, kill_rate1, kill_rate2);
  });

  _food.fill(1.0f);
  _life.fill(0.0f);

  for (int i = 0; i < 5; ++i)
  {
    ggo::paint(_life, std::make_shared<ggo::disc_float>(get_random_point(), 0.01f * get_render_min_size()), 1.0f);
  }
}

//////////////////////////////////////////////////////////////
void ggo_rediff_animation_artist::init_output_buffer(uint8_t * buffer)
{
  ggo::fill_solid_rgb(buffer, get_pixels_count(), ggo::color::BLACK);
}

//////////////////////////////////////////////////////////////
void ggo_rediff_animation_artist::diffuse(ggo::gray_image_buffer_float & food_output, ggo::gray_image_buffer_float & life_output) const
{
  ggo::gaussian_blur_2d_loop(_food.data(), food_output.data(), get_render_width(), get_render_height(), _food_stddev, 0.01f);
  ggo::gaussian_blur_2d_loop(_life.data(), life_output.data(), get_render_width(), get_render_height(), _life_stddev, 0.01f);
}

//////////////////////////////////////////////////////////////
void ggo_rediff_animation_artist::update()
{
  ggo::gray_image_buffer_float food_diffused(get_render_width(), get_render_height());
  ggo::gray_image_buffer_float life_diffused(get_render_width(), get_render_height());

  for (int i = 0; i < 16; ++i)
  {
    diffuse(food_diffused, life_diffused);

    for_each_pixel([&](int x, int y)
    {
      const float feed_rate = _feed_map(x, y);
      const float kill_rate = _kill_map(x, y);
      const float reaction = _food(x, y) * ggo::square(_life(x, y));
      const float food = food_diffused.get(x, y) - reaction + feed_rate * (1.f - _food(x, y));
      const float life = life_diffused.get(x, y) + reaction - (kill_rate + feed_rate) * _life(x, y);

      _food(x, y) = food;
      _life(x, y) = life;
    });
  }
}

//////////////////////////////////////////////////////////////
void ggo_rediff_animation_artist::render(ggo::rgb_image_abc & image) const
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
    ggo::pos3f p_bl(0.f, 0.f, 0.45f * _life(x, y) + 0.2f * (_life.get_loop(x - 1, y) + _life.get_loop(x, y - 1)) + 0.15f * _life.get_loop(x - 1, y - 1)); // Bottom left
    ggo::pos3f p_tr(1.f, 1.f, 0.45f * _life(x, y) + 0.2f * (_life.get_loop(x + 1, y) + _life.get_loop(x, y + 1)) + 0.15f * _life.get_loop(x + 1, y + 1)); // Top right
    ggo::vec3f v1(p_tr - p_bl);

    ggo::pos3f p_tl(0.f, 1.f, 0.45f * _life(x, y) + 0.2f * (_life.get_loop(x - 1, y) + _life.get_loop(x, y + 1)) + 0.15f * _life.get_loop(x - 1, y + 1)); // Top left.
    ggo::pos3f p_br(1.f, 0.f, 0.45f * _life(x, y) + 0.2f * (_life.get_loop(x + 1, y) + _life.get_loop(x, y - 1)) + 0.15f * _life.get_loop(x + 1, y - 1)); // Bottom right.
    ggo::vec3f v2(p_tl - p_br);

    ggo::vec3f normal = ggo::cross(v1, v2);
    float dot = -ggo::dot(normal.get_normalized(), light_dir);

    // Apply lighting.
    float sat = sat_map.evaluate(_life(x, y));
    float val = val_map.evaluate(_life(x, y));

    ggo::color pixel = ggo::color::from_hsv(_hue, sat, val);
    pixel += 0.8f * std::pow(dot, 1200.f) * ggo::color::WHITE;
    image.set(x, y, pixel);
  });
}

//////////////////////////////////////////////////////////////
bool ggo_rediff_animation_artist::render_next_frame_acc(uint8_t * buffer, int frame_index)
{
  if (frame_index > 350)
  {
    return false;
  }

  update();
  
  if (buffer != nullptr)
  {
    auto image = make_image_buffer(buffer);
    render(image);
  }

  return true;
}
