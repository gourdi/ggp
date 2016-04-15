#include "ggo_sonson_animation_artist.h"
#include <ggo_fill.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo_sonson_animation_artist::ggo_sonson_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_mask(8 * render_width, 8 * render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_sonson_animation_artist::init_sub()
{
  ggo::fill(_mask, 0);
  _background_color = ggo::color::from_hsv(ggo::rand_float(), 1.f, 1.f);

  _hue_map.push_point(0.9f, ggo::rand_float());
  _hue_map.push_point(1.0f, _hue_map.back().second + ggo::rand_float(-0.01f, 0.01f));

  _sat_map.push_point(0.9f, 0.f);
  _sat_map.push_point(1.0f, ggo::rand_float(0.0f, 0.5f));

  _val_map.push_point(0.98f, 0.f);
  _val_map.push_point(1.0f, 1.0f);

  _pos.x() = 0.5f * static_cast<float>(_mask.get_width());
  _pos.y() = 0.5f * static_cast<float>(_mask.get_height());

  _vel = ggo::vector2d_float::from_polar(ggo::rand_float(0, 2.f * ggo::PI<float>()), 0.1f * static_cast<float>(get_render_min_size()));
}

//////////////////////////////////////////////////////////////
bool ggo_sonson_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index > 300)
  {
    return false;
  }

  ggo::rgb_image_buffer_uint8 image_buffer(buffer, get_render_width(), get_render_height());

  // Update the mask.

  ggo::paint(_mask, std::make_shared<ggo::extended_segment_float>(_pos, _pos + _vel, 0.01f * get_render_min_size()), 1.0f, 1.0f, std::make_shared<ggo::gray_alpha_blender>(), ggo::pixel_sampler_1());
  _pos += _vel;

  // Render the mask.
  for (int y = 0; y < get_render_height(); ++y)
  {
    for (int x = 0; x < get_render_width(); ++x)
    {
      image_buffer.pack(x, y, get_color(x, y));
    }
  }

  // Update mask.
  for (uint8_t & v : _mask)
  {
    if (v > 1)
    {
      v -= 1;
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////
ggo::color ggo_sonson_animation_artist::get_color(const int x, const int y) const
{
  ggo::color pixel(ggo::color::BLACK);

  for (int y_mask = 8 * y; y_mask < 8 * y + 8; ++y_mask)
  {
    for (int x_mask = 8 * x; x_mask < 8 * x + 8; ++x_mask)
    {
      uint8_t v = _mask[y_mask * _mask.get_width() + x_mask];

      if (v == 0)
      {
        pixel += _background_color;
      }
      else
      {
        pixel += map_color(v);
      }
    }
  }

  return pixel / static_cast<float>(8 * 8);
}

//////////////////////////////////////////////////////////////
ggo::color ggo_sonson_animation_artist::map_color(const uint8_t v) const
{
  // To get rid for numerical errors, we map pixel value before interpolating.
  float mapped = ggo::map(static_cast<float>(v), 1.f, 255.f, 0.f, 1.f);
  return ggo::color::from_hsv(_hue_map.evaluate(mapped), _sat_map.evaluate(mapped), _val_map.evaluate(mapped));
}
