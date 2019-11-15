#include "ggo_poupette_artist.h"
#include <kernel/ggo_ease.h>
#include <2d/ggo_image.h>
#include <array>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
  void render_tile_t(const std::array<ggo::rgb_32f, 4> & colors,
    void * buffer, int width, int height, int line_byte_step, int frame_index, const ggo::rect_int & clipping)
  {
    const float h = static_cast<float>(height);
    const float w = static_cast<float>(width);

    ggo::image_t<pixel_type, memory_lines_order> img(buffer, { width, height }, line_byte_step);

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
          (y1 * colors[0] + (h - y1) * colors[1]) / h,
          (y2 * colors[0] + (h - y2) * colors[1]) / h };

        const std::array<ggo::rgb_32f, 4> colors_right{
          (y1 * colors[2] + (h - y1) * colors[3]) / h,
          (y2 * colors[2] + (h - y2) * colors[3]) / h };

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

          img.write_pixel(x, y, { uint8_t(r), uint8_t(g), uint8_t(b) });
        }
      }
    }
    else
    {
      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        for (int x = clipping.left(); x <= clipping.right(); ++x)
        {
          ggo::rgb_32f pixel_color{ 0.f, 0.f, 0.f };

          ggo::sampler<sampling>::sample_pixel<float>(x, y, [&](float x_f, float y_f)
          {
            const ggo::rgb_32f c5 = (y_f * colors[0] + (h - y_f) * colors[1]) / h;
            const ggo::rgb_32f c6 = (y_f * colors[2] + (h - y_f) * colors[3]) / h;

            pixel_color += color_mod((x_f * c5 + (w - x_f) * c6) / w);
          });

          pixel_color /= ggo::sampler<sampling>::samples_count;

          int r = ggo::round_to<int>(pixel_color.r());
          int g = ggo::round_to<int>(pixel_color.g());
          int b = ggo::round_to<int>(pixel_color.b());

          img.write_pixel(x, y, { uint8_t(r), uint8_t(g), uint8_t(b) });
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::poupette_artist::poupette_artist(float color_max)
{
  const float hue = ggo::rand<float>();

  auto create_color = [&]()
  {
    ggo::rgb_32f c = ggo::from_hsv<ggo::rgb_32f>(hue, ggo::rand<float>(), ggo::rand<float>());

    auto stretch = [&](float v)
    {
      return color_max * v;
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
void ggo::poupette_artist::render_tile(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
                                       int frame_index, const ggo::rect_int & clipping, ggo::pixel_sampling sampling) const
{
  auto colors = interpolate_colors(static_cast<float>(frame_index) / static_cast<float>(_frames_count - 1));

  if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_1>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_2x2>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_4x4>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_8x8>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_16x16>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    }
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down, ggo::sampling_1>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down, ggo::sampling_2x2>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down, ggo::sampling_4x4>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down, ggo::sampling_8x8>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down, ggo::sampling_16x16>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    }
  }
  else if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    switch (sampling)
    {
    case ggo::sampling_1:
      render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_1>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_2x2:
      render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_2x2>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_4x4:
      render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_4x4>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_8x8:
      render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_8x8>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    case ggo::sampling_16x16:
      render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_16x16>(colors, buffer, width, height, line_byte_step, frame_index, clipping);
      break;
    }
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
bool ggo::poupette_artist::finished(int frame_index) const
{
  return frame_index > _frames_count;
}

//////////////////////////////////////////////////////////////
std::array<ggo::rgb_32f, 4> ggo::poupette_artist::interpolate_colors(float t) const
{
  t = ggo::ease_inout(t);

  return { {
      linerp(_c1_start, _c1_end, t),
      linerp(_c2_start, _c2_end, t),
      linerp(_c3_start, _c3_end, t),
      linerp(_c4_start, _c4_end, t) } };
}



