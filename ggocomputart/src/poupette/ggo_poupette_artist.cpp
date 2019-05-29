#include "ggo_poupette_artist.h"
#include <kernel/ggo_ease.h>
#include <2d/ggo_color.h>
#include <array>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::image_format format, ggo::pixel_sampling sampling>
  void render_tile_t(
    const ggo::rgb_32f & c1,
    const ggo::rgb_32f & c2,
    const ggo::rgb_32f & c3,
    const ggo::rgb_32f & c4,
    void * buffer, int width, int height, int line_step, int frame_index, const ggo::rect_int & clipping)
  {
    const float h = static_cast<float>(height);
    const float w = static_cast<float>(width);

    auto color_mod = [](const ggo::rgb_32f & c)
    {
      return ggo::rgb_32f(ggo::pos_mod(c.r(), 255.f), ggo::pos_mod(c.g(), 255.f), ggo::pos_mod(c.b(), 255.f));
    };

    // Optimized version of the 2x2 sampling used for the real time rendering.
    if constexpr (sampling == ggo::sampling_2x2)
    {
      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        const float y1 = y + 1.f / 4.f;
        const float y2 = y + 3.f / 4.f;

        const std::array<ggo::rgb_32f, 4> colors_left{
          (y1 * c1 + (h - y1) * c2) / h,
          (y2 * c1 + (h - y2) * c2) / h };

        const std::array<ggo::rgb_32f, 4> colors_right{
          (y1 * c3 + (h - y1) * c4) / h,
          (y2 * c3 + (h - y2) * c4) / h };

        uint8_t * ptr = static_cast<uint8_t *>(ggo::get_pixel_ptr<format>(buffer, clipping.left(), y, height, line_step));

        for (int x = clipping.left(); x <= clipping.right(); ++x)
        {
          const float w1 = (x + 1.f / 4.f) / w;
          const float w2 = (x + 3.f / 4.f) / w;

          const float ww1 = 1.f - w1;
          const float ww2 = 1.f - w2;

          ggo::rgb_32f pixel_color = 
            color_mod(w1 * colors_left[0] + ww1 * colors_right[0]) +
            color_mod(w2 * colors_left[0] + ww2 * colors_right[0]) +
            color_mod(w1 * colors_left[1] + ww1 * colors_right[1]) +
            color_mod(w2 * colors_left[1] + ww2 * colors_right[1]);
          pixel_color /= 4.f;

          int r = ggo::round_to<int>(pixel_color.r());
          int g = ggo::round_to<int>(pixel_color.g());
          int b = ggo::round_to<int>(pixel_color.b());
          ggo::write_pixel<format>(ptr, { uint8_t(r), uint8_t(g), uint8_t(b) });

          ptr += ggo::image_format_traits<format>::pixel_byte_size;
        }
      }
    }
    else
    {
      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        uint8_t * ptr = static_cast<uint8_t *>(ggo::get_pixel_ptr<format>(buffer, clipping.left(), y, height, line_step));

        for (int x = clipping.left(); x <= clipping.right(); ++x)
        {
          ggo::rgb_32f pixel_color{ 0.f, 0.f, 0.f };

          ggo::sampler<sampling>::sample_pixel<float>(x, y, [&](float x_f, float y_f)
          {
            const ggo::rgb_32f c5 = (y_f * c1 + (h - y_f) * c2) / h;
            const ggo::rgb_32f c6 = (y_f * c3 + (h - y_f) * c4) / h;

            pixel_color += color_mod((x_f * c5 + (w - x_f) * c6) / w);
          });

          pixel_color /= ggo::sampler<sampling>::samples_count;

          int r = ggo::round_to<int>(pixel_color.r());
          int g = ggo::round_to<int>(pixel_color.g());
          int b = ggo::round_to<int>(pixel_color.b());
          ggo::write_pixel<format>(ptr, { uint8_t(r), uint8_t(g), uint8_t(b) });

          ptr += ggo::image_format_traits<format>::pixel_byte_size;
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::poupette_artist::poupette_artist()
{
  constexpr float inf = 0.f;
  constexpr float sup = 2000.f;

  const float hue = ggo::rand<float>();

  auto create_color = [&]()
  {
    ggo::rgb_32f c = ggo::from_hsv<ggo::rgb_32f>(hue, ggo::rand<float>(), ggo::rand<float>());

    auto stretch = [&](float v)
    {
      return (sup - inf) * v + inf;
    };

    c.r() = stretch(c.r());
    c.g() = stretch(c.g());
    c.b() = stretch(c.b());

    return c;
  };

  _c1_start = create_color();
  _c2_start = create_color();
  _c3_start = create_color();
  _c4_start = create_color();

  _c1_end = create_color();
  _c2_end = create_color();
  _c3_end = create_color();
  _c4_end = create_color();
}

//////////////////////////////////////////////////////////////
void ggo::poupette_artist::render_tile(void * buffer, int width, int height, int line_step, ggo::image_format format, int frame_index, const ggo::rect_int & clipping, ggo::pixel_sampling sampling) const
{
  const rgb_32f c1 = ggo::ease_inout(frame_index, _frames_count, _c1_start, _c1_end);
  const rgb_32f c2 = ggo::ease_inout(frame_index, _frames_count, _c2_start, _c2_end);
  const rgb_32f c3 = ggo::ease_inout(frame_index, _frames_count, _c3_start, _c3_end);
  const rgb_32f c4 = ggo::ease_inout(frame_index, _frames_count, _c4_start, _c4_end);

  switch (format)
  {
  case ggo::rgb_8u_yu:
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::rgb_8u_yu, ggo::sampling_1>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::rgb_8u_yu, ggo::sampling_2x2>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::rgb_8u_yu, ggo::sampling_4x4>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::rgb_8u_yu, ggo::sampling_8x8>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::rgb_8u_yu, ggo::sampling_16x16>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;

    }
    break;

  case ggo::rgb_8u_yd:
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::rgb_8u_yd, ggo::sampling_1>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::rgb_8u_yd, ggo::sampling_2x2>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::rgb_8u_yd, ggo::sampling_4x4>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::rgb_8u_yd, ggo::sampling_8x8>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::rgb_8u_yd, ggo::sampling_16x16>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    }
    break;

  case ggo::bgrx_8u_yd:
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::bgrx_8u_yd, ggo::sampling_1>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::bgrx_8u_yd, ggo::sampling_2x2>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::bgrx_8u_yd, ggo::sampling_4x4>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::bgrx_8u_yd, ggo::sampling_8x8>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::bgrx_8u_yd, ggo::sampling_16x16>(c1, c2, c3, c4, buffer, width, height, line_step, frame_index, clipping);
      break;
    }
    break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::poupette_artist::finished(int frame_index) const
{
  return frame_index > _frames_count;
}


