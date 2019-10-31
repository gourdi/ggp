#ifndef __GGO_TILING__
#define __GGO_TILING__

#include <kernel/ggo_rect_int.h>
#include <vector>

namespace ggo
{
  struct vertical_tiling
  {
    static std::vector<ggo::rect_int> make_tiles(const ggo::rect_int & clipping, int tiles_count)
    {
      int bottom = clipping.bottom();
      int height = clipping.height();

      std::vector<ggo::rect_int> tiles;

      for (int tile = 0; tile < tiles_count; ++tile)
      {
        int tile_bottom = bottom + height * tile / tiles_count;
        int tile_top = bottom + height * (tile + 1) / tiles_count - 1;

        if (tile_top >= tile_bottom)
        {
          tiles.push_back(ggo::rect_int::from_left_right_bottom_top(clipping.left(), clipping.right(), tile_bottom, tile_top));
        }
      }

      return tiles;
    }
  };
}

#endif

