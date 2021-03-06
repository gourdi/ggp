#include "ggo_brute_force_raycaster.h"
#include <raytracer/objects3d/ggo_object3d_abc.h>
#include <raytracer/ggo_scene.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  brute_force_raycaster::brute_force_raycaster(std::vector<const ggo::object3d_abc *> objects)
  :
  _objects(std::move(objects))
  {

  }

  //////////////////////////////////////////////////////////////
  brute_force_raycaster::brute_force_raycaster(const std::vector<std::shared_ptr<const ggo::object3d_abc>> & objects)
  {
    for (auto & ptr : objects)
    {
      _objects.push_back(ptr.get());
    }
  }

  //////////////////////////////////////////////////////////////
  void brute_force_raycaster::process_ray(const ggo::ray3d_f & ray,
                                          std::function<bool(const ggo::object3d_abc *)> func,
                                          const ggo::object3d_abc * exclude_object1,
                                          const ggo::object3d_abc * exclude_object2) const
  {
    for (const auto * object : _objects)
    {
      if (object == exclude_object1 || object == exclude_object2)
      {
        continue;
      }

      if (func(object) == false)
      {
        break;
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void brute_force_raycaster::process_segment(const ggo::pos3_f & pos, const ggo::vec3_f & dir, float length,
                                              std::function<bool(const ggo::object3d_abc *)> func,
                                              const ggo::object3d_abc * exclude_object1,
                                              const ggo::object3d_abc * exclude_object2) const
  {
    for (const auto * object : _objects)
    {
      if (object == exclude_object1 || object == exclude_object2)
      {
        continue;
      }

      if (func(object) == false)
      {
        break;
      }
    }
  }
}

#if 0
  //////////////////////////////////////////////////////////////
  std::optional<hit_data> brute_force_raycaster::hit_test(const ggo::ray3d_f & ray,
                                                          const ggo::object3d_abc * exclude_object1,
                                                          const ggo::object3d_abc * exclude_object2) const
  {
    const ggo::object3d_abc * hit_object = nullptr;
    float dist = std::numeric_limits<float>::max();
    ggo::ray3d_f local_normal, world_normal;

    for (const auto * object : _visible_objects)
    {
      if (object == exclude_object1 || object == exclude_object2)
      {
        continue;
      }

      auto intersection = object->intersect_ray(ray);

      if (intersection && intersection->_dist < dist)
      {
        hit_object = object;
        dist = intersection->_dist;
        local_normal = intersection->_local_normal;
        world_normal = intersection->_world_normal;
      }
    }

    return hit_object ? hit_data(hit_object, dist, local_normal, world_normal) : std::optional<hit_data>();
  }

  //////////////////////////////////////////////////////////////
  bool brute_force_raycaster::check_visibility(const ggo::ray3d_f & ray,
                                               float dist_max,
                                               const ggo::object3d_abc * exclude_object1,
                                               const ggo::object3d_abc * exclude_object2) const
  {
    for (const auto * object : _casting_shadows_objects)
    {
      if (object == exclude_object1 || object == exclude_object2)
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
#endif
