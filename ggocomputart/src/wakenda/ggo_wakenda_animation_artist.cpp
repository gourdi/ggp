#include "ggo_wakenda_animation_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::wakenda_animation_artist::wakenda_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
animation_artist_abc(width, height, line_step, format)
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
      transform._end._dx      = transform._start._dx * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._dy      = transform._start._dy * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._powx    = transform._start._powx * ggo::rand(0.8f, 1 / 0.8f);
      transform._end._powy    = transform._start._powy * ggo::rand(0.8f, 1 / 0.8f);
    }
  };

  create_transforms(_transforms_x);
  create_transforms(_transforms_y);

  _hue = ggo::rand<float>();
  for (auto & color : _bkgd_colors)
  {
    color = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  }
}

namespace
{
  //////////////////////////////////////////////////////////////
  void process_moving_points(std::vector<ggo::wakenda_animation_artist::moving_point> & moving_points,
    std::vector<ggo::wakenda_animation_artist::fading_point> & fading_points,
    int frame_index, float hue,
    const std::array<ggo::wakenda_animation_artist::transform_interpolation, 4> & transforms_x,
    const std::array<ggo::wakenda_animation_artist::transform_interpolation, 4> & transforms_y)
  {
    constexpr int frames_count = 300;
    constexpr int substeps_count = 10;
    constexpr int moving_points_creations_per_subframe = 1;
    constexpr int moving_point_fadein_timer = 100;
    constexpr int moving_point_fadeout_timer = 800;
    constexpr float moving_point_clipping_dist = 20.f;
    constexpr float velocity_scaling = 0.0025f;
    constexpr float opacity_inc = 0.001f;
    constexpr float opacity_dec = 0.0001f;

    auto f = [&](const ggo::pos2f & p, const auto & transforms)
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
      for (auto & p : moving_points)
      {
        float dx = velocity_scaling * f(p._pos, transforms_x);
        float dy = velocity_scaling * f(p._pos, transforms_y);
        p._pos.x() += dx;
        p._pos.y() += dy;
        p._counter += 1;

        if (frame_index >= frames_count)
        {
          p._opacity -= opacity_dec;
        }
        else
        {
          static_assert(moving_point_fadein_timer < moving_point_fadeout_timer);

          if (p._counter < moving_point_fadein_timer)
          {
            p._opacity += opacity_inc;
          }
          else if (p._counter > moving_point_fadeout_timer)
          {
            p._opacity -= opacity_dec;
          }
        }

        if (p._opacity > 0.f)
        {
          fading_points.emplace_back(p._pos, p._color, p._opacity);
        }
      }

      ggo::remove_if(moving_points, [&](const auto & p) {
        return p._opacity <= 0.f || p._pos.get_length() > moving_point_clipping_dist;
      });

      if (frame_index < frames_count)
      {
        for (int i = 0; i < moving_points_creations_per_subframe; ++i)
        {
          auto color = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>());
          moving_points.emplace_back(ggo::rand(-2.f, 2.f), ggo::rand(-2.f, 2.f), color);
        }
      }
      printf("%zu\n", moving_points.size());
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::wakenda_animation_artist::render_frame(void * buffer, int frame_index, bool & finished)
{
  constexpr float opacity_decay = 0.005f;

  ggo::fill_4_colors<ggo::rgb_8u_yd>(buffer, width(), height(), line_step(),
    _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);

  process_moving_points(_moving_points, _fading_points, frame_index, _hue, _transforms_x, _transforms_y);

  const float radius = min_size() / 250.f;
  for (auto & p : _fading_points)
  {
    const auto center = map_fit(p._pos, -1.f, 1.f);

    paint<ggo::rgb_8u_yd, ggo::sampling_8x8>(buffer, ggo::disc_float(center, radius), p._color, p._opacity);

    p._opacity -= opacity_decay;
  }

  ggo::remove_if(_fading_points, [](const auto & p) { 
    return p._opacity <= 0.f;
  });

  finished = _moving_points.empty() && _fading_points.empty();
}

