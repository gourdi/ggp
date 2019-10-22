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

                                julia_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

            void					      render_bitmap(void * buffer, const std::complex<float> & seed, float range = 1.7f) const;

    static	std::complex<float> pickup_seed();

  private:

            template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
            void					      render_bitmap_t(void * buffer, const std::complex<float> & seed, float range = 1.7f) const;

            int						      iterate(float x, float y, const std::complex<float> & seed) const;

  private:

    std::array<ggo::rgb_8u, 1000>	_palette;
  };
}

#endif