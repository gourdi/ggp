#ifndef __GGO_RAYTRACER_GLOBAL__
#define __GGO_RAYTRACER_GLOBAL__

#include <ggo_raycaster_abc.h>

namespace ggo
{
  class fog_abc;
  class background3d_abc;
  class indirect_lighting_abc;

  //////////////////////////////////////////////////////////////
  struct raytrace_params
  {
    int						                      _depth = 3;
    int                                 _threads_count = 1;
    const ggo::raycaster_abc *          _raycaster = nullptr;
    const ggo::indirect_lighting_abc *  _indirect_lighting = nullptr;
  };
}



#endif
