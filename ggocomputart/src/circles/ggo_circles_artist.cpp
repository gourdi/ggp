#include "ggo_circles_artist.h"
#include <ggo_pbf_paint.h>

//////////////////////////////////////////////////////////////
std::vector<std::vector<ggo::circles_artist::colored_disc>> ggo::circles_artist::generate_discs(int width, int height)
{
	const ggo::color_32f start_color1(rand<float>(), rand<float>(), rand<float>());
  const ggo::color_32f start_color2(rand<float>(), rand<float>(), rand<float>());
  const ggo::color_32f start_color3(rand<float>(), rand<float>(), rand<float>());
  const ggo::color_32f start_color4(rand<float>(), rand<float>(), rand<float>());
  
  std::vector<std::vector<colored_disc>> result;
  
  for (int i = 0; i < 300; ++i)
  {
    float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    float angle_inc = ggo::rand<float>(0.02f, 0.03f) * (ggo::rand_bool() ? -1 : 1);

    float radius = ggo::rand<float>(0.2f, 0.8f) * std::max(width, height);
    
    ggo::pos2f center(ggo::rand<float>(0.f, static_cast<float>(width)), ggo::rand<float>(0.f, static_cast<float>(height)));

    float circle_radius_delta = ggo::rand<float>(0.002f, 0.003f) * std::max(width, height);
    int circles_count = ggo::rand<int>(25, 30);
    
    std::vector<colored_disc> discs;

    for (int j = 0; j < circles_count; ++j)
    {
      colored_disc colored_disc;
      
      // Set up the circle.
      colored_disc._disc.center().x() = center.x() + radius * std::cos(angle);
      colored_disc._disc.center().y() = center.y() + radius * std::sin(angle);
      colored_disc._disc.radius() = (circles_count - j) * circle_radius_delta;
      
      // Set the current circle's colour.
      float mapped_y = colored_disc._disc.center().y() / height;
      ggo::color_32f output_color;
      if (j % 2)
      {
        output_color = start_color1 * mapped_y + start_color2 * (1 - mapped_y);
      }
      else
      {
        output_color = start_color3 * mapped_y + start_color4 * (1 - mapped_y);
      }
      colored_disc._color = convert_color_to<ggo::color_8u>(output_color);
      
      angle += angle_inc;
      
      discs.push_back(colored_disc);
    }
    
    result.push_back(discs);
  }
  
  return result;
}

//////////////////////////////////////////////////////////////
void ggo::circles_artist::paint_disc(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const ggo::pos2f & pos, float radius, const ggo::color_8u & color)
{
  ggo::paint_shape<rgb_8u_yu, sampling_8x8>(buffer, width, height, 3 * width,
    disc_float(pos, radius + 0.002f * std::min(width, height)), color / static_cast<uint8_t>(4));
  ggo::paint_shape<rgb_8u_yu, sampling_8x8>(buffer, width, height, 3 * width,
    ggo::disc_float(pos, radius), color);
}

//////////////////////////////////////////////////////////////
void ggo::circles_artist::paint_disc(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const colored_disc & colored_disc)
{
  paint_disc(buffer, width, height, line_step, pbf, colored_disc._disc.center(), colored_disc._disc.radius(), colored_disc._color);
}


