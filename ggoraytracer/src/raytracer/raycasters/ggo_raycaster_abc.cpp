#include "ggo_raycaster_abc.h"
#include <raytracer/objects3d/ggo_object3d_abc.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  std::optional<hit_data> raycaster_abc::hit_test(const ggo::ray3d_float & ray, const ggo::object3d_abc * exclude_object1, const ggo::object3d_abc * exclude_object2) const
  {
    const ggo::object3d_abc * hit_object = nullptr;
    float dist = std::numeric_limits<float>::max();
    ggo::ray3d_float local_normal, world_normal;

    auto func = [&](const ggo::object3d_abc * object)
    {
      auto intersection = object->intersect_ray(ray);

      if (intersection && intersection->_dist < dist)
      {
        hit_object = object;
        dist = intersection->_dist;
        local_normal = intersection->_local_normal;
        world_normal = intersection->_world_normal;
      }

      return true;
    };

    process_ray(ray, func, exclude_object1, exclude_object2);

    return hit_object ? hit_data(hit_object, dist, local_normal, world_normal) : std::optional<hit_data>();
  }

  //////////////////////////////////////////////////////////////
  bool raycaster_abc::check_visibility(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::object3d_abc * exclude_object1, const ggo::object3d_abc * exclude_object2) const
  {
    ggo::vec3f diff = p2 - p1;
    float length = ggo::length(diff);
    ggo::vec3f dir = diff / length;

    bool visible = true;

    auto func = [&](const ggo::object3d_abc * object)
    {
      if (object->intersect_segment(p1, dir, length) == true)
      {
        visible = false;
        return false;
      }

      return true;
    };

    process_segment(p1, dir, length, func, exclude_object1, exclude_object2);

    return visible;
  }
}
