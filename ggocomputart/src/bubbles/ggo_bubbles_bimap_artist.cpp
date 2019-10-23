#include "ggo_bubbles_bitmap_artist.h"
#include <2d/ggo_color.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <ggo_artist.h>

namespace
{
  //////////////////////////////////////////////////////////////
  class bubbles_artist : public ggo::artist
  {
  public:

    bubbles_artist(int width, int height) : artist(width, height) {}

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_bitmap(void * buffer, int line_byte_step) const
    {
      ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step);

      // Render background.
      ggo::linear_curve<float, ggo::rgb_32f> bkgd_gradient;
      bkgd_gradient.push_point(0, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
      bkgd_gradient.push_point(1, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
      ggo::fill_color_curve(img, bkgd_gradient);

      const int bubbles_count = 100;
      for (int i = 0; i < bubbles_count; ++i)
      {
        // Init.
        float dx = ggo::rand(-0.01f, 0.01f) * min_size();
        float dy = ggo::rand(0.03f, 0.06f) * min_size();
        float wavelength = ggo::rand(5.f, 10.f) * min_size();
        float amplitude = ggo::rand(0.01f, 0.02f) * min_size();

        auto generate_color = []()
        {
          return ggo::rgb_32f(ggo::rand(-0.1f, 0.1f), ggo::rand(-0.1f, 0.1f), ggo::rand(-0.1f, 0.1f));
        };

        ggo::linear_curve<float, ggo::rgb_32f> curve;
        curve.push_point(0.0f * height(), generate_color());
        curve.push_point(0.5f * height(), generate_color());
        curve.push_point(1.0f * height(), generate_color());

        ggo::disc_f bubble;
        bubble.set_radius(ggo::rand<float>(0.003f, 0.006f) * min_size());
        bubble.set_center({ ggo::rand<float>(-0.1f, 1.1f) * width(), -0.1f * height() });

        // Move the bubble up.
        while (bubble.get_center().y() < height() + bubble.get_radius())
        {
          bubble.center().x() += dx + amplitude * std::sin(wavelength * bubble.center().y());
          bubble.center().y() += dy;

          auto brush = [&](int x, int y) { return curve.evaluate(bubble.center().x()); };
          auto blend = [&](int x, int y, const ggo::rgb_8u & bkgd_color, const ggo::rgb_32f & brush_color) {
            const ggo::rgb_32f bkgd_color_32f = ggo::convert_color_to<ggo::rgb_32f>(bkgd_color);
            return ggo::convert_color_to<ggo::rgb_8u>(bkgd_color_32f + brush_color);
          };

          ggo::paint<ggo::sampling_4x4>(img, bubble, brush, blend);
        }
      }
    }
  };
}

//////////////////////////////////////////////////////////////
void ggo::bubbles_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  bubbles_artist artist(width, height);

  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    artist.render_bitmap<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, line_byte_step);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    artist.render_bitmap<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, line_byte_step);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    artist.render_bitmap<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, line_byte_step);
  }
  else
  {
    GGO_FAIL();
  }
}
