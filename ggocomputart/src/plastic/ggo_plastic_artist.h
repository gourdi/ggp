#ifndef __GGO_PLASTIC_ARTIST__
#define __GGO_PLASTIC_ARTIST__

#include <2d/ggo_pixel_type.h>

namespace ggo
{
  namespace plastic_artist
  {
    struct params
    {
      float _pos_x;
      float _pos_y;
      float _den;
      float _mult;
      float _power;
    };

    void	render(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor);
  };
}

#endif