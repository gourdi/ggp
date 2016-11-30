#ifndef __GGO_CRYSTAL_ARTIST__
#define __GGO_CRYSTAL_ARTIST__

#include <ggo_artist.h>
#include <ggo_array.h>

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

    static void render_bitmap(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const params & params);
    static void randomize_params(params & params);

  private:

    static void render_transform(float * buffer, int width, int height, const params & params);
  };
}

#endif
