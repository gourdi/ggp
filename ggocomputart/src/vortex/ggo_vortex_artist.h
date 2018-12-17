#ifndef __GGO_VORTEX_ARTIST__
#define __GGO_VORTEX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class vortex_artist
  {
  public:

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

    static	void	render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::vortex_artist::params & params);
  };
}

#endif
