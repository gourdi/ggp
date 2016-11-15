#ifndef __GGO_JULIA_ARTIST__
#define __GGO_JULIA_ARTIST__

#include <ggo_artist.h>
#include <complex>
#include <array>

namespace ggo
{
  class julia_artist : public artist
  {
  public:

                                julia_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

            void					      render_bitmap(void * buffer, const std::complex<float> & seed, float range = 1.7f);

    static	std::complex<float> pickup_seed();

  private:

            int						      iterate(float x, float y, const std::complex<float> & seed) const;

  private:

    std::array<ggo::color_8u, 1000>	_palette;
  };
}

#endif