#include "ggo_bubbles_artist.h"
#include <2d/ggo_color.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo::bubbles_artist::bubbles_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
void ggo::bubbles_artist::render_bitmap(void * buffer) const
{
  // Render background.
	ggo::linear_curve<float, ggo::rgb_32f> bkgd_gradient;
  bkgd_gradient.push_point(0, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
  bkgd_gradient.push_point(1, { ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>() });
  switch (format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_color_curve<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(), bkgd_gradient);
    break;
  case ggo::bgrx_8u_yd:
    ggo::fill_color_curve<ggo::bgrx_8u_yd>(buffer, width(), height(), line_step(), bkgd_gradient);
    break;
  default:
    GGO_FAIL();
    break;
  }

  const int bubbles_count = 100;
  for (int i = 0; i < bubbles_count; ++i)
  {
    // Init.
    float dx			    = ggo::rand(-0.01f, 0.01f) * min_size();
    float dy			    = ggo::rand(0.03f, 0.06f) * min_size();
    float wavelength	= ggo::rand(5.f, 10.f) * min_size();
    float amplitude	  = ggo::rand(0.01f, 0.02f) * min_size();
    
    auto generate_color = []()
    {
      return ggo::rgb_32f(ggo::rand(-0.1f, 0.1f), ggo::rand(-0.1f, 0.1f), ggo::rand(-0.1f, 0.1f));
    };

    ggo::linear_curve<float, ggo::rgb_32f> curve;
    curve.push_point(0.0f * height(), generate_color());
    curve.push_point(0.5f * height(), generate_color());
    curve.push_point(1.0f * height(), generate_color());

    ggo::disc_float bubble;
    bubble.set_radius(ggo::rand<float>(0.003f, 0.006f) * min_size());
    bubble.set_center(ggo::rand<float>(-0.1f, 1.1f) * width(), -0.1f * height());

    // Move the bubble up.
    while (bubble.get_center().y() < height() + bubble.get_radius())
    {
      bubble.center().get<0>() += dx + amplitude * std::sin(wavelength * bubble.center().y());
      bubble.center().get<1>() += dy;
      
      auto brush = [&](int x, int y) { return curve.evaluate(bubble.center().get<0>()); };
      auto blend = [&](int x, int y, const rgb_8u & bkgd_color, const rgb_32f & brush_color) {
        const rgb_32f bkgd_color_32f = convert_color_to<rgb_32f>(bkgd_color);
        return convert_color_to<rgb_8u>(bkgd_color_32f + brush_color);
      };

      switch (format())
      {
      case ggo::rgb_8u_yu:
        paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, width(), height(), line_step(), bubble, brush, blend);
        break;
      case ggo::bgrx_8u_yd:
        paint<ggo::bgrx_8u_yd, ggo::sampling_4x4>(buffer, width(), height(), line_step(), bubble, brush, blend);
        break;
      default:
        GGO_FAIL();
        break;
      }
    }
  }
}
