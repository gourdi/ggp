#ifndef __GGO_VORTEX_ARTIST__
#define __GGO_VORTEX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/ggo_vec2.h>

namespace ggo
{
  namespace vortex_artist
  {
    struct vortex
    {
      ggo::pos2_f _pos;
      float				_angle;
      float				_speed;
      float				_power;
    };

    struct params
    {
      ggo::array<vortex, 1> _vortices;
      ggo::rgb_32f  	      _color1;
      ggo::rgb_32f  	      _color2;
      ggo::rgb_32f  	      _color3;
      ggo::rgb_32f  	      _color4;
      float 					      _split_horz;
      float 					      _split_vert;
    };

    void	render(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, const ggo::vortex_artist::params & params);
  };
}

#endif
