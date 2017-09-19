#include "ggo_circles_animation_artist.h"
#include "ggo_circles_artist.h"
#include <math.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
bool ggo::circles_animation_artist::circle_animate::update(int frame_index, const ggo::pos2f & pos)
{
  return true;
}

//////////////////////////////////////////////////////////////
void ggo::circles_animation_artist::circle_animate::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const
{
  float radius = _radius * (1 - std::cos(_bounding_factor * frame_index) * std::exp(-_attenuation_factor * frame_index));

  circles_artist::paint_disc(buffer, width, height, line_step, pbf, pos, radius, _color);
}

//////////////////////////////////////////////////////////////
ggo::circles_animation_artist::circles_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::circles_animation_artist::init()
{
  _frame_index = -1;

  _bkgd_color.set(rand<uint8_t>(), rand<uint8_t>(), rand<uint8_t>());

  auto all_discs = circles_artist::generate_discs(get_width(), get_height());

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
bool ggo::circles_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index > 900)
  {
    return false;
  }

  _animator.update();

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::circles_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  ggo::fill_solid<rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), _bkgd_color, clipping);

  _animator.render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format());
}

