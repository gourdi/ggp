#include "ggo_brute_force_raycaster.h"
#include <ggo_object3d_abc.h>
#include <ggo_scene.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  const ggo::object3d_abc * brute_force_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                            float & dist,
                                                            ggo::ray3d_float & local_normal,
                                                            ggo::ray3d_float & world_normal,
                                                            const ggo::object3d_abc * exclude_object1,
                                                            const ggo::object3d_abc * exclude_object2) const
  {
    const ggo::object3d_abc * hit_object = nullptr;
    dist = std::numeric_limits<float>::max();

    for (const auto & object : _scene.visible_objects())
    {
      if (object.get() == exclude_object1 || object.get() == exclude_object2)
      {
        continue;
      }

      auto intersection = object->intersect_ray(ray);

      if (intersection && intersection->_dist < dist)
      {
        hit_object = object.get();
        dist = intersection->_dist;
        local_normal = intersection->_local_normal;
        world_normal = intersection->_world_normal;
      }
    }

    return hit_object;
  }

  //////////////////////////////////////////////////////////////
  bool brute_force_raycaster::check_visibility(const ggo::ray3d_float & ray,
                                               float dist_max,
                                               const ggo::object3d_abc * exclude_object1,
                                               const ggo::object3d_abc * exclude_object2) const
  {
    for (const auto & object : _scene.casting_shadows_objects())
    {
      if (object.get() == exclude_object1 || object.get() == exclude_object2)
      {
        continue;
      }

      auto intersection = object->intersect_ray(ray);

      if (intersection && intersection->_dist < dist_max)
      {
        return true;
      }
    }

    return false;
  }
}
