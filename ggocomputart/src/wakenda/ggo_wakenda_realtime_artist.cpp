#include "ggo_wakenda_realtime_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  constexpr int frames_count = 300;
  constexpr int post_frames_count = frames_count + 200;
}

//////////////////////////////////////////////////////////////
ggo::wakenda_realtime_artist::wakenda_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
realtime_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order),
_bkgd_image({ width, height }, pixel_type, memory_lines_order, line_byte_step)
{
  auto create_transforms = [](auto & transforms)
  {
    for (auto & transform : transforms)
    {
      transform._start._scale = ggo::rand(-2.0f, 2.0f);
      transform._start._dx = ggo::rand(-0.5f, 0.5f);
      transform._start._dy = ggo::rand(-0.5f, 0.5f);
      transform._start._powx = ggo::rand(0.5f, 2.0f);
      transform._start._powy = ggo::rand(0.5f, 2.0f);

      transform._end._scale = transform._start._scale * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._dx = ggo::rand(-0.5f, 0.5f);
      transform._end._dy = ggo::rand(-0.5f, 0.5f);
      transform._end._powx = transform._start._powx * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._powy = transform._start._powy * ggo::rand(0.8f, 1 / 0.8f);
    }
  };

  create_transforms(_transforms_x);
  create_transforms(_transforms_y);

  _hue = ggo::rand<float>();

  auto bkgd_color1 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color2 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color3 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color4 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    fill_4_colors(ggo::image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(_bkgd_image.data(), _bkgd_image.size(), _bkgd_image.line_byte_step()),
      bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    fill_4_colors(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(_bkgd_image.data(), _bkgd_image.size(), _bkgd_image.line_byte_step()),
      bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    fill_4_colors(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(_bkgd_image.data(), _bkgd_image.size(), _bkgd_image.line_byte_step()),
      bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::wakenda_realtime_artist::copy_bkgd(void * buffer, const ggo::rect_int & clipping) const
{
  constexpr int pixel_byte_size = pixel_type_traits<pixel_type>::pixel_byte_size;

  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * src = _bkgd_image.pixel_ptr(clipping.left(), y);
    void * dst = ggo::get_pixel_ptr<memory_lines_order, pixel_byte_size>(buffer, clipping.left(), y, height(), line_byte_step());

    memcpy(dst, src, pixel_byte_size * clipping.width());
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::wakenda_realtime_artist::process_bkgd(void * buffer, const ggo::rect_int & clipping) const
{
  constexpr int pixel_byte_size = pixel_type_traits<pixel_type>::pixel_byte_size;

  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const uint8_t * bkgd_ptr = static_cast<const uint8_t *>(_bkgd_image.pixel_ptr(clipping.left(), y));
    uint8_t * output_ptr = static_cast<uint8_t *>(ggo::get_pixel_ptr<memory_lines_order, pixel_byte_size>(buffer, clipping.left(), y, height(), line_byte_step()));

    for (int i = 0; i < pixel_byte_size * clipping.width(); ++i)
    {
      int d = static_cast<int>(*output_ptr) - static_cast<int>(*bkgd_ptr);

      d = (d * 63) / 64;
      *output_ptr = *bkgd_ptr + d;

      ++bkgd_ptr;
      ++output_ptr;
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::wakenda_realtime_artist::paint_points(void * buffer, const ggo::rect_int & clipping) const
{
  image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  const float radius = min_size() / 250.f;

  for (const auto & p : _paint_points)
  {
    paint<ggo::sampling_8x8>(img, ggo::disc_f(p._pos, radius), p._color, p._opacity, clipping);
  }
}

//////////////////////////////////////////////////////////////
void ggo::wakenda_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
{
  constexpr int substeps_count = 10;
  constexpr int points_creations_per_subframe = 1;
  constexpr int point_fadein_timer = 100;
  constexpr int point_fadeout_timer = 800;
  constexpr float point_clipping_dist = 20.f;
  constexpr float velocity_scaling = 0.0025f;
  constexpr float opacity_inc = 0.001f;
  constexpr float opacity_dec = 0.0001f;

  _paint_points.clear();

  auto eval = [&](const ggo::pos2_f & p, const auto & transforms)
  {
    auto safe_pow = [](float v, float exp)
    {
      return v < 0.f ? -std::pow(-v, exp) : std::pow(v, exp);
    };

    float d = 0.f;

    for (const auto & transform : transforms)
    {
      float scale = ggo::ease_inout(frame_index, frames_count, transform._start._scale, transform._end._scale);
      float dx = ggo::ease_inout(frame_index, frames_count, transform._start._dx, transform._end._dx);
      float dy = ggo::ease_inout(frame_index, frames_count, transform._start._dy, transform._end._dy);
      float powx = ggo::ease_inout(frame_index, frames_count, transform._start._powx, transform._end._powx);
      float powy = ggo::ease_inout(frame_index, frames_count, transform._start._powy, transform._end._powy);

      d += scale * safe_pow(p.x() - dx, powx) * safe_pow(p.y() - dy, powy);
    }

    return d;
  };

  for (int substep = 0; substep < substeps_count; ++substep)
  {
    for (auto & p : _points)
    {
      float dx = velocity_scaling * eval(p._pos, _transforms_x);
      float dy = velocity_scaling * eval(p._pos, _transforms_y);
      p._pos.x() += dx;
      p._pos.y() += dy;
      p._counter += 1;

      if (frame_index >= frames_count)
      {
        p._opacity -= opacity_dec;
      }
      else
      {
        static_assert(point_fadein_timer < point_fadeout_timer);

        if (p._counter < point_fadein_timer)
        {
          p._opacity += opacity_inc;
        }
        else if (p._counter > point_fadeout_timer)
        {
          p._opacity -= opacity_dec;
        }
      }

      if (p._opacity > 0.f)
      {
        _paint_points.emplace_back(map_fit(p._pos, -1.f, 1.f), p._color, p._opacity);
      }
    }

    ggo::remove_if(_points, [&](const auto & p) {
      return p._opacity <= 0.f || ggo::length(p._pos) > point_clipping_dist;
    });

    if (frame_index < frames_count)
    {
      for (int i = 0; i < points_creations_per_subframe; ++i)
      {
        auto color = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
        _points.emplace_back(ggo::rand(-2.f, 2.f), ggo::rand(-2.f, 2.f), color);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::wakenda_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  if (frame_index == 0)
  {
    if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
    {
      copy_bkgd<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
    }
    else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
    {
      copy_bkgd<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
    }
    else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
    {
      copy_bkgd<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
    }
    else
    {
      GGO_FAIL();
    }
  }

  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    process_bkgd<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
    paint_points<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    process_bkgd<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
    paint_points<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    process_bkgd<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
    paint_points<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
bool ggo::wakenda_realtime_artist::finished(int frame_index) const
{
  return frame_index > post_frames_count;
}


