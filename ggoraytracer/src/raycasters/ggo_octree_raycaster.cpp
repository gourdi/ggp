#include "ggo_octree_raycaster.h"
#include <ggo_object3d_abc.h>
#include <ggo_scene.h>
#include <ggo_kernel.h>
#include <map>

namespace
{
  std::pair<std::vector<const ggo::object3d_abc *>, const ggo::octree_raycaster::octree_t *> build_octree(const std::vector<std::shared_ptr<const ggo::object3d_abc>> & shapes, int depth)
  {
    std::vector<const ggo::object3d_abc *> unbounded_shapes;
    std::map<const ggo::object3d_abc *, ggo::box3d_data_float> bounded_shapes;

    // Compute shapes' bounding boxes, and filter the ones that don't have one.
    for (const auto & shape : shapes)
    {
      auto bounding_box = shape->get_bounding_box();
      if (bounding_box.has_value() == true)
      {
        bounded_shapes.emplace(shape.get(), *bounding_box);
      }
      else
      {
        unbounded_shapes.emplace_back(shape.get());
      }
    }

    // Build the octree.
    auto get_bounding_box = [&](const ggo::object3d_abc * shape)
    {
      auto it = bounded_shapes.find(shape);
      if (it == bounded_shapes.end())
      {
        throw std::runtime_error("shape not found");
      }
      return it->second;
    };

    // The octree constructor expects interators on "const ggo::object3d_abc *".
    auto adaptor = ggo::make_adaptor(bounded_shapes, [](const auto & it) { return it.first; });

    return { unbounded_shapes, new ggo::octree_raycaster::octree_t(adaptor.begin(), adaptor.end(), get_bounding_box, depth) };
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  octree_raycaster::octree_raycaster(const scene & scene, int depth)
  {
    auto visible = build_octree(scene.visible_objects(), depth);
    auto casting_shadows = build_octree(scene.casting_shadows_objects(), depth);

    _visible_objects_tree.reset(visible.second);
    _casting_shadows_objects_tree.reset(casting_shadows.second);

    _brute_force_raycaster.reset(new brute_force_raycaster(visible.first, casting_shadows.first));
  }

  //////////////////////////////////////////////////////////////
  const ggo::object3d_abc * octree_raycaster::hit_test(const ggo::object3d_abc * hit_object,
                                                       const ggo::octree<const ggo::object3d_abc *, float> & node,
                                                       const ggo::ray3d_float & ray,
                                                       float & dist,
                                                       ggo::ray3d_float & local_normal,
                                                       ggo::ray3d_float & world_normal,
                                                       const ggo::object3d_abc * exclude_object1,
                                                       const ggo::object3d_abc * exclude_object2) const
  {
    ggo::box3d<float> bounding_box(node.bounding_box());

    if (bounding_box.intersect_ray(ray) == true)
    {
      for (const ggo::object3d_abc * object : node.data())
      {
        if (object != exclude_object1 && object != exclude_object2)
        {
          auto intersection = object->intersect_ray(ray);
          if (intersection.has_value() == true)
          {
            if (hit_object == nullptr || intersection->_dist < dist)
            {
              hit_object = object;
              dist = intersection->_dist;
              local_normal = intersection->_local_normal;
              world_normal = intersection->_world_normal;
            }
          }
        }
      }

      for (const auto & child_node : node.children())
      {
        hit_object = hit_test(hit_object, child_node, ray, dist, local_normal, world_normal, exclude_object1, exclude_object2);
      }
    }

    return hit_object;
  }

  //////////////////////////////////////////////////////////////
  std::optional<hit_data> octree_raycaster::hit_test(const ggo::ray3d_float & ray,
                                                     const ggo::object3d_abc * exclude_object1,
                                                     const ggo::object3d_abc * exclude_object2) const
  {
    float dist = 0.f;
    ggo::ray3d_float local_normal, world_normal;
    const ggo::object3d_abc * hit_object = hit_test(nullptr, *_visible_objects_tree, ray, dist, local_normal, world_normal, exclude_object1, exclude_object2);

    auto hit = _brute_force_raycaster->hit_test(ray, exclude_object1, exclude_object2);

    if (hit_object == nullptr && hit.has_value() == false)
    {
      return {};
    }
    else if (hit_object == nullptr && hit.has_value() == true)
    {
      return hit;
    }
    else if (hit_object != nullptr && hit.has_value() == false)
    {
      return hit_data(hit_object, dist, local_normal, world_normal);
    }
    else
    {
      GGO_ASSERT(hit_object != nullptr && hit.has_value() == true);

      return dist < hit->_intersection._dist ? hit_data(hit_object, dist, local_normal, world_normal) : hit;
    }
  }

  //////////////////////////////////////////////////////////////
  bool octree_raycaster::check_visibility(const ggo::octree<const ggo::object3d_abc *, float> & node,
                                          const ggo::ray3d_float & ray,
                                          float dist_max,
                                          const ggo::object3d_abc * exclude_object1,
                                          const ggo::object3d_abc * exclude_object2) const
  {
    ggo::box3d<float> bounding_box(node.bounding_box());

    if (bounding_box.intersect_ray(ray) == false)
    {
      return false;
    }

    for (const ggo::object3d_abc * object : node.data())
    {
      if (object != exclude_object1 && object != exclude_object2)
      {
        auto intersection = object->intersect_ray(ray);
        if (intersection.has_value() == true && intersection->_dist <= dist_max)
        {
          return true;
        }
      }
    }

    for (const auto & child_node : node.children())
    {
      if (check_visibility(child_node, ray, dist_max, exclude_object1, exclude_object2) == true)
      {
        return true;
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  bool octree_raycaster::check_visibility(const ggo::ray3d_float & ray,
                                          float dist_max,
                                          const ggo::object3d_abc * exclude_object1,
                                          const ggo::object3d_abc * exclude_object2) const
  {
    if (check_visibility(*_casting_shadows_objects_tree, ray, dist_max, exclude_object1, exclude_object2) == true)
    {
      return true;
    }

    return _brute_force_raycaster->check_visibility(ray, dist_max, exclude_object1, exclude_object2);
  }
}
