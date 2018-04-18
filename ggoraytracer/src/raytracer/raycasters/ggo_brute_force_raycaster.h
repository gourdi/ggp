#ifndef __GGO_BRUTE_FORCE_RAYCASTER__
#define __GGO_BRUTE_FORCE_RAYCASTER__

#include <raytracer/raycasters/ggo_raycaster_abc.h>

namespace ggo
{
  class scene;

  class brute_force_raycaster : public raycaster_abc
  {
  public:

    brute_force_raycaster(const scene & scene);
    brute_force_raycaster(std::vector<const ggo::object3d_abc *> visible_objects, std::vector<const ggo::object3d_abc *> casting_shadows_objects);

    std::optional<hit_data> hit_test(const ggo::ray3d_float & ray,
                                     const ggo::object3d_abc * exclude_object1,
                                     const ggo::object3d_abc * exclude_object2) const override;

    bool check_visibility(const ggo::ray3d_float & ray,
                          float dist_max,
                          const ggo::object3d_abc * exclude_object1,
                          const ggo::object3d_abc * exclude_object2) const override;

  private:

    std::vector<const ggo::object3d_abc *> _visible_objects;
    std::vector<const ggo::object3d_abc *> _casting_shadows_objects;
  };
}

#endif
