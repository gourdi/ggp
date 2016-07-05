#ifndef __GGO_MARCHING_SQUARES__
#define __GGO_MARCHING_SQUARES__

#include <functional>
#include <vector>
#include <ggo_shapes2d.h>

namespace ggo
{
  using func2d = std::function<float(float, float)>;

  struct marching_square_cell
  {
    int                             _x;
    int                             _y;
    std::vector<ggo::segment_float> _segments;
  };

  std::vector<ggo::marching_square_cell> marching_squares(ggo::func2d func, ggo::pos2f inf, int steps, float step_size);
}

#endif
