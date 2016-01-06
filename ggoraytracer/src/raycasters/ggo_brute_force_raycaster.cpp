#include "ggo_brute_force_raycaster.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  const ggo::object3d * brute_force_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                        float & dist,
                                                        ggo::ray3d_float & local_normal,
                                                        ggo::ray3d_float & world_normal,
                                                        const ggo::object3d * exclude_object1,
                                                        const ggo::object3d * exclude_object2) const
  {
    const ggo::object3d * hit_object = nullptr;
    dist = std::numeric_limits<float>::max();

    for (const auto & object : _objects)
    {
      if (object.get() == exclude_object1 || object.get() == exclude_object2)
      {
        continue;
      }

      float dist_cur = 0;
      ggo::ray3d_float local_normal_cur, world_normal_cur;

      if ((object->intersect_ray(ray, dist_cur, local_normal_cur, world_normal_cur) == true) && (dist_cur < dist))
      {
        hit_object = object.get();
        dist = dist_cur;
        local_normal = local_normal_cur;
        world_normal = world_normal_cur;
      }
    }

    return hit_object;
  }

  //////////////////////////////////////////////////////////////
  bool brute_force_raycaster::hit_test(const ggo::ray3d_float & ray,
                                       float dist_max,
                                       const ggo::object3d * exclude_object1,
                                       const ggo::object3d * exclude_object2) const
  {
    for (const auto & object : _objects)
    {
      if (object.get() == exclude_object1 || object.get() == exclude_object2)
      {
        continue;
      }

      float dist = 0;
      ggo::ray3d_float local_normal, world_normal;

      if ((object->intersect_ray(ray, dist, local_normal, world_normal) == true) && (dist < dist_max))
      {
        return true;
      }
    }

    return false;
  }
}
