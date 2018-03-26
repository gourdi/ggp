#ifndef __GGO_VORTEX_ARTIST__
#define __GGO_VORTEX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_array.h>

namespace ggo
{
  class vortex_artist
  {
  public:

    struct vortex
    {
      ggo::pos2f  _pos;
      float				_angle;
      float				_speed;
      float				_power;
    };

    struct params
    {
      ggo::array<vortex, 1> _vortices;
      ggo::color_32f  	    _color1;
      ggo::color_32f  	    _color2;
      ggo::color_32f  	    _color3;
      ggo::color_32f  	    _color4;
      float 					      _split_horz;
      float 					      _split_vert;
    };

    static	void	render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const ggo::vortex_artist::params & params);
  };
}

#endif