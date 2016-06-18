#ifndef __GGO_RLE_RGB8_IMAGE__
#define __GGO_RLE_RGB8_IMAGE__

#include <ggo_image_abc.h>
#include <ggo_rle.h>
#include <ggo_array.h>
#include <tuple>

namespace ggo
{
  class rle_rgb8_image : public image_abc<ggo::color>
  {
  public:

                              rle_rgb8_image(int width, int height, const ggo::color & fill_value = ggo::color::BLACK, int cache_size = 8);

    void                      set(int x, int y, const ggo::color & value) override;
    ggo::color                get(int x, int y) const override;

  private:

    ggo::array<uint8_t, 1> &  get_cache_line(int y) const;

  private:

    mutable ggo::array<std::vector<std::pair<std::tuple<uint8_t, uint8_t, uint8_t>, int>>, 1> _rle_lines;
    mutable std::vector<std::pair<int, ggo::array<uint8_t, 1>>>                               _cache_lines;
    const int                                                                                 _cache_size;
  };
}

#endif
