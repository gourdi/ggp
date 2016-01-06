#ifndef __GGO_BRUTE_FORCE_RAYCASTER__
#define __GGO_BRUTE_FORCE_RAYCASTER__

#include <ggo_raycaster_abc.h>

namespace ggo
{
  class brute_force_raycaster : public raycaster_abc
  {
  public:

                          brute_force_raycaster(const std::vector<std::shared_ptr<const ggo::object3d>> & objects) : _objects(objects) {}

    const ggo::object3d * hit_test(const ggo::ray3d_float & ray,
                                   float & dist,
                                   ggo::ray3d_float & local_normal,
                                   ggo::ray3d_float & world_normal,
                                   const ggo::object3d * exclude_object1,
                                   const ggo::object3d * exclude_object2) const override;

    bool                  hit_test(const ggo::ray3d_float & ray,
                                   float dist_max,
                                   const ggo::object3d * exclude_object1,
                                   const ggo::object3d * exclude_object2) const override;

  private:

    const std::vector<std::shared_ptr<const ggo::object3d>> & _objects;
  };
}

#endif
