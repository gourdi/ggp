#include "ggo_wakenda_animation_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  constexpr int frames_count = 300;
  constexpr int post_frames_count = frames_count + 200;
}

//////////////////////////////////////////////////////////////
ggo::wakenda_animation_artist::wakenda_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
animation_artist_abc(width, height, line_step, format),
_bkgd_image(width, height, line_step, ggo::rgb_8u_yu)
{
  auto create_transforms = [](auto & transforms)
  {
    for (auto & transform : transforms)
    {
      transform._start._scale = ggo::rand(-2.0f, 2.0f);
      transform._start._dx    = ggo::rand(-0.5f, 0.5f);
      transform._start._dy    = ggo::rand(-0.5f, 0.5f);
      transform._start._powx  = ggo::rand(0.5f, 2.0f);
      transform._start._powy  = ggo::rand(0.5f, 2.0f);

      transform._end._scale   = transform._start._scale * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._dx      = ggo::rand(-0.5f, 0.5f);
      transform._end._dy      = ggo::rand(-0.5f, 0.5f);
      transform._end._powx    = transform._start._powx * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._powy    = transform._start._powy * ggo::rand(0.8f, 1 / 0.8f);
    }
  };

  create_transforms(_transforms_x);
  create_transforms(_transforms_y);

  _hue = ggo::rand<float>();

  auto bkgd_color1 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color2 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color3 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  auto bkgd_color4 = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

  ggo::fill_4_colors<ggo::rgb_8u_yd>(_bkgd_image.data(), _bkgd_image.width(), _bkgd_image.height(), _bkgd_image.line_byte_step(),
    bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
}

//////////////////////////////////////////////////////////////
void ggo::wakenda_animation_artist::process_bkgd(void * buffer) const
{
  // Fade/blend.
  for (int y = 0; y < height(); ++y)
  {
    const uint8_t * bkgd_ptr = static_cast<const uint8_t *>(_bkgd_image.line_ptr(y));
    uint8_t * output_ptr = static_cast<uint8_t *>(ggo::ptr_offset(buffer, y * line_step()));

    for (int i = 0; i < 3 * width(); ++i)
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
void ggo::wakenda_animation_artist::process_points(void * buffer, int frame_index)
{
  constexpr int substeps_count = 10;
  constexpr int points_creations_per_subframe = 1;
  constexpr int point_fadein_timer = 100;
  constexpr int point_fadeout_timer = 800;
  constexpr float point_clipping_dist = 20.f;
  constexpr float velocity_scaling = 0.0025f;
  constexpr float opacity_inc = 0.001f;
  constexpr float opacity_dec = 0.0001f;

  const float radius = min_size() / 250.f;

  auto eval = [&](const ggo::pos2f & p, const auto & transforms)
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
        const auto center = map_fit(p._pos, -1.f, 1.f);

        paint<ggo::rgb_8u_yd, ggo::sampling_8x8>(buffer, ggo::disc_float(center, radius), p._color, p._opacity);
      }
    }

    ggo::remove_if(_points, [&](const auto & p) {
      return p._opacity <= 0.f || p._pos.get_length() > point_clipping_dist;
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
void ggo::wakenda_animation_artist::render_frame(void * buffer, int frame_index, bool & finished)
{
  if (frame_index == 0)
  {
    GGO_ASSERT(line_step() == _bkgd_image.line_byte_step());
    memcpy(buffer, _bkgd_image.data(), height() * line_step());
  }

  process_bkgd(buffer);
  process_points(buffer, frame_index);

  finished = frame_index > post_frames_count;
}

