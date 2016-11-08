#ifndef __GGO_CRYSTAL_ARTIST__
#define __GGO_CRYSTAL_ARTIST__

#include <ggo_artist.h>
#include <ggo_array.h>

namespace ggo
{
  class crystal_artist : public artist
  {
  public:

    constexpr static int transforms_cout = 6;

    struct params
    {
      float	_coefs[transforms_cout][6];
      float	_dr[transforms_cout], _dg[transforms_cout], _db[transforms_cout];
    };

  public:

    crystal_artist(int render_width, int render_height);

    void render_bitmap(uint8_t * buffer, const params & params, const ggo::color_8u bkgd_colors[4]);

    static void randomize_params(params & params);

  private:

    void process_transform(const params & params);

  private:

    ggo::array_float	_render_buffer;
  };
}

#endif
