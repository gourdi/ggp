#ifndef __GGO_FILLING_SQUARES_ARTIST__
#define __GGO_FILLING_SQUARES_ARTIST__

#include <vector>
#include <kernel/ggo_vec2.h>
#include <2d/ggo_color.h>

namespace ggo
{
  namespace filling_squares_artist
  {
    struct colored_square
    {
      float		    _size;
      ggo::rgb_8u _color;
    };

    struct multi_square
    {
      std::vector<colored_square> _squares;
      ggo::pos2_f				          _pos;
    };

    std::vector<multi_square>	build_squares(int width, int height, float & hue);
  };
}

#endif

