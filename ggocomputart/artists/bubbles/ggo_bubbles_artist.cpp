#include "ggo_bubbles_artist.h"
#include <ggo_color.h>
#include <ggo_fill.h>

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
    
  int bubbles_count = 100;
  for (int i = 0; i < bubbles_count; ++i)
  {
    // Init.
    float dx			    = ggo::rand_float(-0.01f, 0.01f) * get_render_min_size();
    float dy			    = ggo::rand_float(0.03f, 0.06f) * get_render_min_size();
    float wavelength	= ggo::rand_float(5.f, 10.f) * get_render_min_size();
    float amplitude	  = ggo::rand_float(0.01f, 0.02f) * get_render_min_size();
    
    ggo::cubic_curve_float curve_r;
    curve_r.push_point(0, ggo::rand_float());
    curve_r.push_point(1, ggo::rand_float());
    curve_r.push_point(0.5, ggo::rand_float());
    
    ggo::cubic_curve_float curve_g;
    curve_g.push_point(0, ggo::rand_float());
    curve_g.push_point(1, ggo::rand_float());
    curve_g.push_point(0.5, ggo::rand_float());
    
    ggo::cubic_curve_float curve_b;
    curve_b.push_point(0, ggo::rand_float());
    curve_b.push_point(1, ggo::rand_float());
    curve_b.push_point(0.5, ggo::rand_float());

    ggo::disc_float	bubble;
    bubble.set_radius(ggo::rand_float(0.03f, 0.06f) * get_render_min_size());
    bubble.set_center(ggo::rand_float(-0.1f, 1.1f) * get_render_width(), -0.1f * get_render_height());

    // Move the bubble up.
    while (bubble.get_center().y() < get_render_height() + bubble.get_radius())
    {
      bubble.center().x() += dx + amplitude * std::sin(wavelength * bubble.center().y());
      bubble.center().y() += dy;
      
      int dr = ggo::to<int>((curve_r.evaluate(bubble.center().x() / get_render_width()) - 0.5) * 10);
      int dg = ggo::to<int>((curve_g.evaluate(bubble.center().x() / get_render_width()) - 0.5) * 10);
      int db = ggo::to<int>((curve_b.evaluate(bubble.center().x() / get_render_width()) - 0.5) * 10);
      
      // Custom painting of the bubble. Not optimized, but I don't care.
      for (int y = -static_cast<int>(bubble.radius()); y <= static_cast<int>(bubble.radius()); ++y )
      {
        for (int x = -static_cast<int>(bubble.radius()); x <= static_cast<int>(bubble.radius()); ++x )
        {
          float hypot = static_cast<float>(x * x + y * y);
          if (hypot < bubble.radius() * bubble.radius())
          {
            int render_x = ggo::to<int>(bubble.center().x()) + x;
            int render_y = ggo::to<int>(bubble.center().y()) + y;
  
            if (render_x >= 0 && render_x < get_render_width() &&
                render_y >= 0 && render_y < get_render_height())
            {
              int index = 3 * (render_y * get_render_width() + render_x);
  
              buffer[index + 0] = ggo::clamp(buffer[index + 0] + dr, 0, 0xFF);
              buffer[index + 1] = ggo::clamp(buffer[index + 1] + dg, 0, 0xFF);
              buffer[index + 2] = ggo::clamp(buffer[index + 2] + db, 0, 0xFF);
            }
          }
        }
      }
    }
  }
}
