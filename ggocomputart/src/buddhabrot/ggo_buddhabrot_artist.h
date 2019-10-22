#ifndef __GGO_BUDDHABROT_ARTIST__
#define __GGO_BUDDHABROT_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <complex>
#include <kernel/memory/ggo_array.h>

namespace ggo
{
  class buddhabrot_artist : public bitmap_artist_abc
  {
  public:

          buddhabrot_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;

    static bool discard_point(const std::complex<double> & p);
    static bool iterate(const std::complex<double> & p, int escape_threshold);
    static bool iterate(const std::complex<double> & p, std::vector<std::complex<double> > & points, int escape_threshold);
    static void setup_seed_area(int escape_threshold_r, int escape_threshold_g, int escape_threshold_b, std::complex<double> & center, double & range);
    
           void process(int escape_threshold, ggo::array<int, 2> & accumulation, const std::complex<double> & center, double range) const;
  };
}

#endif

