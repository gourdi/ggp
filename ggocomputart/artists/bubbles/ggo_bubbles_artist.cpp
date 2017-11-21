#include "ggo_bubbles_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

//////////////////////////////////////////////////////////////
ggo::bubbles_artist::bubbles_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::bubbles_artist::render_bitmap(void * buffer) const
{
  // Render background.
	ggo::linear_curve<float, ggo::color_32f> bkgd_gradient;
  bkgd_gradient.push_point(0, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
  bkgd_gradient.push_point(1, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_color_curve<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), bkgd_gradient);
    break;
  case ggo::bgra_8u_yd:
    ggo::fill_color_curve<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), bkgd_gradient);
    break;
  default:
    GGO_FAIL();
    break;
  }

  const int bubbles_count = 100;
  for (int i = 0; i < bubbles_count; ++i)
  {
    // Init.
    float dx			    = ggo::rand<float>(-0.01f, 0.01f) * get_min_size();
    float dy			    = ggo::rand<float>(0.03f, 0.06f) * get_min_size();
    float wavelength	= ggo::rand<float>(5.f, 10.f) * get_min_size();
    float amplitude	  = ggo::rand<float>(0.01f, 0.02f) * get_min_size();
    
    auto generate_color = []()
    {
      return ggo::color_32f(ggo::rand<float>(-0.1f, 0.1f), ggo::rand<float>(-0.1f, 0.1f), ggo::rand<float>(-0.1f, 0.1f));
    };

    ggo::linear_curve<float, ggo::color_32f> curve;
    curve.push_point(0.0f * get_height(), generate_color());
    curve.push_point(0.5f * get_height(), generate_color());
    curve.push_point(1.0f * get_height(), generate_color());

    ggo::disc_float bubble;
    bubble.set_radius(ggo::rand<float>(0.003f, 0.006f) * get_min_size());
    bubble.set_center(ggo::rand<float>(-0.1f, 1.1f) * get_width(), -0.1f * get_height());

    // Move the bubble up.
    while (bubble.get_center().y() < get_height() + bubble.get_radius())
    {
      bubble.center().get<0>() += dx + amplitude * std::sin(wavelength * bubble.center().y());
      bubble.center().get<1>() += dy;
      
      auto brush = [&](int x, int y) { return curve.evaluate(bubble.center().get<0>()); };
      auto blend = [&](int x, int y, const color_8u & bkgd_color, const color_32f & brush_color) {
        const color_32f bkgd_color_32f = convert_color_to<color_32f>(bkgd_color);
        return convert_color_to<color_8u>(bkgd_color_32f + brush_color);
      };

      switch (get_pixel_buffer_format())
      {
      case ggo::rgb_8u_yu:
        paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, get_width(), get_height(), get_line_step(), bubble, brush, blend);
        break;
      case ggo::bgra_8u_yd:
        paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(buffer, get_width(), get_height(), get_line_step(), bubble, brush, blend);
        break;
      default:
        GGO_FAIL();
        break;
      }
    }
  }
}
