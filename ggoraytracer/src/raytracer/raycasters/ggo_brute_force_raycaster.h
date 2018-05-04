#ifndef __GGO_BRUTE_FORCE_RAYCASTER__
#define __GGO_BRUTE_FORCE_RAYCASTER__

#include <raytracer/raycasters/ggo_raycaster_abc.h>

namespace ggo
{
  class scene;

  class brute_force_raycaster : public raycaster_abc
  {
  public:

    brute_force_raycaster(std::vector<const ggo::object3d_abc *> objects);

    void  process_ray(const ggo::ray3d_float & ray,
                      std::function<bool(const ggo::object3d_abc *)> func,
                      const ggo::object3d_abc * exclude_object1 = nullptr,
                      const ggo::object3d_abc * exclude_object2 = nullptr) const override;

    void  process_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                          std::function<bool(const ggo::object3d_abc *)> func,
                          const ggo::object3d_abc * exclude_object1 = nullptr,
                          const ggo::object3d_abc * exclude_object2 = nullptr) const override;

  private:

    std::vector<const ggo::object3d_abc *> _objects;
  };
}

#endif
