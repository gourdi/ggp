#ifndef __GGO_VORTEX2_ARTIST__
#define __GGO_VORTEX2_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_array.h>

namespace ggo
{
  class vortex2_artist
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

    static	void	render(void * buffer, int render_width, int render_height, const ggo::vortex2_artist::params & params);
  };
}

#endif
