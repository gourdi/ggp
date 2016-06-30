#include "ggo_circles_artist.h"
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo_circles_artist::ggo_circles_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
std::vector<std::vector<ggo_circles_artist::ggo_colored_disc>> ggo_circles_artist::generate_discs() const
{
	ggo::color start_color1 = ggo::color::get_random();
  ggo::color start_color2 = ggo::color::get_random();
  ggo::color start_color3 = ggo::color::get_random();
  ggo::color start_color4 = ggo::color::get_random();
  
  std::vector<std::vector<ggo_colored_disc>> result;
  
  for (int i = 0; i < 300; ++i)
  {
    float angle = ggo::rand_float(0, 2 * ggo::PI<float>());
    float angle_inc = ggo::rand_float(0.02f, 0.03f) * (ggo::rand_bool() ? -1 : 1);

    float radius = ggo::rand_float(0.2f, 0.8f) * get_render_max_size();
    
    ggo::pos2f center(ggo::rand_float(0.f, static_cast<float>(get_render_width())), ggo::rand_float(0.f, static_cast<float>(get_render_height())));

    float circle_radius_delta = ggo::rand_float(0.002f, 0.003f) * get_render_max_size();
    int circles_count = ggo::rand_int(25, 30);
    
    std::vector<ggo_colored_disc> discs;

    for (int j = 0; j < circles_count; ++j)
    {
      ggo_colored_disc colored_disc;
      
      // Set up the circle.
      colored_disc._disc.center().get<0>() = center.get<0>() + radius * std::cos(angle);
      colored_disc._disc.center().get<1>() = center.get<1>() + radius * std::sin(angle);
      colored_disc._disc.radius() = (circles_count - j) * circle_radius_delta;
      
      // Set the current circle's colour.
      float mapped_y = colored_disc._disc.center().get<1>() / get_render_height();
      if (j % 2)
      {
        colored_disc._color.r() = start_color1.r() * mapped_y + start_color2.r() * (1 - mapped_y);
        colored_disc._color.g() = start_color1.g() * mapped_y + start_color2.g() * (1 - mapped_y);
        colored_disc._color.b() = start_color1.b() * mapped_y + start_color2.b() * (1 - mapped_y);
      }
      else
      {
        colored_disc._color.r() = start_color3.r() * mapped_y + start_color4.r() * (1 - mapped_y);
        colored_disc._color.g() = start_color3.g() * mapped_y + start_color4.g() * (1 - mapped_y);
        colored_disc._color.b() = start_color3.b() * mapped_y + start_color4.b() * (1 - mapped_y);
      }
      
      angle += angle_inc;
      
      discs.push_back(colored_disc);
    }
    
    result.push_back(discs);
  }
  
  return result;
}

//////////////////////////////////////////////////////////////
void ggo_circles_artist::paint_disc(uint8_t * buffer, int width, int height, const ggo::pos2f & pos, float radius, const ggo::color & color)
{
  ggo::paint(buffer, width, height, 
             std::make_shared<ggo::disc_float>(pos, radius + 0.002f * std::min(width, height)),
             color / 4);
  ggo::paint(buffer, width, height, 
             std::make_shared<ggo::disc_float>(pos, radius),
             color);
}

//////////////////////////////////////////////////////////////
void ggo_circles_artist::paint_disc(uint8_t * buffer, int width, int height, const ggo_colored_disc & colored_disc)
{
  paint_disc(buffer, width, height, colored_disc._disc.center(), colored_disc._disc.radius(), colored_disc._color);
}


