#ifndef __GGO_CRYSTAL_ARTIST__
#define __GGO_CRYSTAL_ARTIST__

#include <2d/ggo_image.h>

namespace ggo
{
  namespace crystal_artist
  {
    constexpr static int transforms_cout = 6;

    struct params
    {
      float	_coefs[transforms_cout][6];
      float	_dr[transforms_cout], _dg[transforms_cout], _db[transforms_cout];
    };

    void render_bitmap(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img, const params & params);
    void randomize_params(params & params);
  };
}

#endif
