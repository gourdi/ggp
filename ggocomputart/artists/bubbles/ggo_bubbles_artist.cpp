#include "ggo_bubbles_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

//////////////////////////////////////////////////////////////
ggo::bubbles_artist::bubbles_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo::bubbles_artist::render_bitmap(void * buffer) const
{
  // Render background.
	ggo::linear_curve<float, ggo::color_32f> bkgd_gradient;
  bkgd_gradient.push_point(0, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
  bkgd_gradient.push_point(1, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
	ggo::fill_color_curve<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), bkgd_gradient);

  const int bubbles_count = 100;
  for (int i = 0; i < bubbles_count; ++i)
  {
    // Init.
    float dx			    = ggo::rand<float>(-0.01f, 0.01f) * get_render_min_size();
    float dy			    = ggo::rand<float>(0.03f, 0.06f) * get_render_min_size();
    float wavelength	= ggo::rand<float>(5.f, 10.f) * get_render_min_size();
    float amplitude	  = ggo::rand<float>(0.01f, 0.02f) * get_render_min_size();
    
    auto generate_color = []()
    {
      return ggo::color_32f(ggo::rand<float>(-0.1f, 0.1f), ggo::rand<float>(-0.1f, 0.1f), ggo::rand<float>(-0.1f, 0.1f));
    };

    ggo::linear_curve<float, ggo::color_32f> curve;
    curve.push_point(0.0f * get_render_height(), generate_color());
    curve.push_point(0.5f * get_render_height(), generate_color());
    curve.push_point(1.0f * get_render_height(), generate_color());

    ggo::disc_float bubble;
    bubble.set_radius(ggo::rand<float>(0.003f, 0.006f) * get_render_min_size());
    bubble.set_center(ggo::rand<float>(-0.1f, 1.1f) * get_render_width(), -0.1f * get_render_height());

    // Move the bubble up.
    while (bubble.get_center().get<1>() < get_render_height() + bubble.get_radius())
    {
      bubble.center().get<0>() += dx + amplitude * std::sin(wavelength * bubble.center().get<1>());
      bubble.center().get<1>() += dy;
      
      auto brush = [&](int x, int y) { return curve.evaluate(bubble.center().get<0>()); };
      auto blend = [&](int x, int y, const color_8u & bkgd_color, const color_32f & brush_color) {
        const color_32f bkgd_color_32f = convert_color_to<color_32f>(bkgd_color);
        return convert_color_to<color_8u>(bkgd_color_32f + brush_color);
      };

      paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
        bubble, brush, blend);
    }
  }
}
