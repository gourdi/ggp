#ifndef __GGO_RAYCASTER_ABC__
#define __GGO_RAYCASTER_ABC__

#include <ggo_object3d.h>
#include <vector>

// Important note : a raycaster MUST be thread safe.

namespace ggo
{
  class raycaster_abc
  {
  public:

    virtual		                   ~raycaster_abc() {}
    
    virtual	const ggo::object3d * hit_test(const ggo::ray3d_float & ray,
                                           float & dist,
                                           ggo::ray3d_float & local_normal,
                                           ggo::ray3d_float & world_normal,
                                           const ggo::object3d * exclude_object1 = nullptr,
                                           const ggo::object3d * exclude_object2 = nullptr) const = 0;
          
    virtual	bool                  hit_test(const ggo::ray3d_float & ray,
                                           float dist_max,
                                           const ggo::object3d * exclude_object1 = nullptr,
                                           const ggo::object3d * exclude_object2 = nullptr) const = 0;
  };
}

#endif
