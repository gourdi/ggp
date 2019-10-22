#ifndef __GGO_CRYSTAL_ARTIST__
#define __GGO_CRYSTAL_ARTIST__

#include <ggo_artist.h>

namespace ggo
{
  class crystal_artist
  {
  public:

    constexpr static int transforms_cout = 6;

    struct params
    {
      float	_coefs[transforms_cout][6];
      float	_dr[transforms_cout], _dg[transforms_cout], _db[transforms_cout];
    };

  public:

    static void render_bitmap(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img, const params & params);
    static void randomize_params(params & params);

  private:

    static void render_transform(image_t<ggo::pixel_type::rgb_32f, ggo::lines_order::up> & img, const params & params);
  };
}

#endif
