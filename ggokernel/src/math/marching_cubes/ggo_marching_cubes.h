#ifndef __GGO_MARCHING_CUBES__
#define __GGO_MARCHING_CUBES__

#include <functional>
#include <vector>
#include <ggo_shapes3d.h>

namespace ggo
{
  using func3d = std::function<float(float, float, float)>;

  struct marching_cube_cell
  {
    int                                 _x;
    int                                 _y;
    int                                 _z;
    ggo::aabox3d_float                  _bounding_box;
    std::vector<ggo::triangle3d_float>  _triangles;
  };

  std::vector<ggo::marching_cube_cell> marching_cubes(ggo::func3d func, ggo::point3d_float inf, int steps, float step_size);
}

#endif
