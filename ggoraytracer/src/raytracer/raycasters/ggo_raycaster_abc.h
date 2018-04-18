#ifndef __GGO_RAYCASTER_ABC__
#define __GGO_RAYCASTER_ABC__

#include <optional>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <raytracer/ggo_raytracer_global.h>

// Important note : a raycaster MUST be thread safe.

namespace ggo
{
  class object3d_abc;

  class raycaster_abc
  {
  public:

    virtual ~raycaster_abc() {}
    
    virtual	std::optional<hit_data> hit_test(const ggo::ray3d_float & ray,
                                             const ggo::object3d_abc * exclude_object1 = nullptr,
                                             const ggo::object3d_abc * exclude_object2 = nullptr) const = 0;
          
    virtual	bool                    check_visibility(const ggo::ray3d_float & ray,
                                                     float dist_max,
                                                     const ggo::object3d_abc * exclude_object1 = nullptr,
                                                     const ggo::object3d_abc * exclude_object2 = nullptr) const = 0;
  };
}

#endif
