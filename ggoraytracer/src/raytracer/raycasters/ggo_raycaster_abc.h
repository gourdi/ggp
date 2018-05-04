#ifndef __GGO_RAYCASTER_ABC__
#define __GGO_RAYCASTER_ABC__

#include <functional>
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

    std::optional<hit_data> hit_test(const ggo::ray3d_float & ray, const ggo::object3d_abc * exclude_object1 = nullptr, const ggo::object3d_abc * exclude_object2 = nullptr) const;
    bool                    raycaster_abc::check_visibility(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::object3d_abc * exclude_object1 = nullptr, const ggo::object3d_abc * exclude_object2 = nullptr) const;

    virtual void  process_ray(const ggo::ray3d_float & ray,
                              std::function<bool(const ggo::object3d_abc *)> func,
                              const ggo::object3d_abc * exclude_object1 = nullptr,
                              const ggo::object3d_abc * exclude_object2 = nullptr) const = 0;

    virtual void  process_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                                  std::function<bool(const ggo::object3d_abc *)> func,
                                  const ggo::object3d_abc * exclude_object1 = nullptr,
                                  const ggo::object3d_abc * exclude_object2 = nullptr) const = 0;
  };
}

#endif
