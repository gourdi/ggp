#ifndef __GGO_BRUTE_FORCE_RAYCASTER__
#define __GGO_BRUTE_FORCE_RAYCASTER__

#include <ggo_raycaster_abc.h>

namespace ggo
{
  class scene;

  class brute_force_raycaster : public raycaster_abc
  {
  public:

    brute_force_raycaster(const scene & scene) : _scene(scene) {}

    const ggo::object3d_abc * hit_test(const ggo::ray3d_float & ray,
                                       float & dist,
                                       ggo::ray3d_float & local_normal,
                                       ggo::ray3d_float & world_normal,
                                       const ggo::object3d_abc * exclude_object1,
                                       const ggo::object3d_abc * exclude_object2) const override;

    bool check_visibility(const ggo::ray3d_float & ray,
                          float dist_max,
                          const ggo::object3d_abc * exclude_object1,
                          const ggo::object3d_abc * exclude_object2) const override;

  private:

    const scene & _scene;
  };
}

#endif
