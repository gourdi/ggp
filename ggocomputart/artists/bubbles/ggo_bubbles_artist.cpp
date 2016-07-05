#include "ggo_bubbles_artist.h"
#include <ggo_color.h>
#include <ggo_fill.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo_bubbles_artist::ggo_bubbles_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_bubbles_artist::render_bitmap(uint8_t * buffer)
{
  auto image = make_image_buffer(buffer);
  
  // Render background.
	ggo::linear_curve<float, ggo::color> bkgd_gradient;
  bkgd_gradient.push_point(0, ggo::color::get_random());
  bkgd_gradient.push_point(1, ggo::color::get_random());
	ggo::fill_color_curve(image, bkgd_gradient);

  const int bubbles_count = 100;
  for (int i = 0; i < bubbles_count; ++i)
  {
    // Init.
    float dx			    = ggo::rand_float(-0.01f, 0.01f) * get_render_min_size();
    float dy			    = ggo::rand_float(0.03f, 0.06f) * get_render_min_size();
    float wavelength	= ggo::rand_float(5.f, 10.f) * get_render_min_size();
    float amplitude	  = ggo::rand_float(0.01f, 0.02f) * get_render_min_size();
    
    auto generate_color = []()
    {
      return ggo::color(ggo::rand_float(-0.1f, 0.1f), ggo::rand_float(-0.1f, 0.1f), ggo::rand_float(-0.1f, 0.1f));
    };

    ggo::linear_curve<float, ggo::color> curve;
    curve.push_point(0.0f * get_render_height(), generate_color());
    curve.push_point(0.5f * get_render_height(), generate_color());
    curve.push_point(1.0f * get_render_height(), generate_color());

    auto bubble = std::make_shared<ggo::disc_float>();
    bubble->set_radius(ggo::rand_float(0.003f, 0.006f) * get_render_min_size());
    bubble->set_center(ggo::rand_float(-0.1f, 1.1f) * get_render_width(), -0.1f * get_render_height());

    // Move the bubble up.
    while (bubble->get_center().get<1>() < get_render_height() + bubble->get_radius())
    {
      bubble->center().get<0>() += dx + amplitude * std::sin(wavelength * bubble->center().get<1>());
      bubble->center().get<1>() += dy;
      
      ggo::color c = curve.evaluate(bubble->center().get<0>());
      
      ggo::paint(image, bubble, c, 1.f, std::make_shared<ggo::rgb_additive_blender>());
    }
  }
}
