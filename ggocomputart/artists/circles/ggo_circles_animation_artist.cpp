#include "ggo_circles_animation_artist.h"
#include "ggo_circles_artist.h"
#include <math.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
bool ggo::circles_animation_artist::circle_animate::update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos)
{    
  float radius = _radius * (1 - std::cos(_bounding_factor * counter) * std::exp(-_attenuation_factor * counter));
  
  circles_artist::paint_disc(buffer, width, height, pos, radius, _color);

  return true;
}

//////////////////////////////////////////////////////////////
ggo::circles_animation_artist::circles_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo::circles_animation_artist::init_sub()
{
  _bkgd_color.set(rand<uint8_t>(), rand<uint8_t>(), rand<uint8_t>());

  auto all_discs = circles_artist::generate_discs(get_render_width(), get_render_height());

  int start_offset = 0;
  for (const auto & discs : all_discs)
  {
    float attenuation_factor = ggo::rand<float>(0.02f, 0.025f);
    float bounding_factor = ggo::rand<float>(0.1f, 0.12f);
      
    for (const auto & colored_disc : discs)
    {
      int disc_start_offset = start_offset + ggo::rand<int>(0, 2 * static_cast<int>(discs.size()));
     
      circle_animate * circle_animate = new ggo::circles_animation_artist::circle_animate(colored_disc._disc.center(), disc_start_offset);
      circle_animate->_radius = colored_disc._disc.radius();
      circle_animate->_color = colored_disc._color;
      circle_animate->_attenuation_factor = attenuation_factor;
      circle_animate->_bounding_factor = bounding_factor;
          
      _animator.add_animate(circle_animate);
    }
            
    start_offset += 2;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::circles_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  if (frame_index > 900)
  {
    return false;
  }

  ggo::fill_solid<rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), _bkgd_color);

  _animator.update(buffer, get_render_width(), get_render_height());

	return true;
}

