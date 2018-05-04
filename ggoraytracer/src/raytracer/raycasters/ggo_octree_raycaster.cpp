#include "ggo_octree_raycaster.h"
#include <kernel/ggo_kernel.h>
#include <raytracer/ggo_scene.h>
#include <raytracer/objects3d/ggo_object3d_abc.h>
#include <map>

namespace
{
  std::pair<std::vector<const ggo::object3d_abc *>, const ggo::octree_raycaster::octree_t *> build_octree(const std::vector<const ggo::object3d_abc *> & shapes, int depth)
  {
    std::vector<const ggo::object3d_abc *> unbounded_shapes;
    std::map<const ggo::object3d_abc *, ggo::box3d_data_float> bounded_shapes;

    // Compute shapes' bounding boxes, and filter the ones that don't have one.
    for (const auto & shape : shapes)
    {
      auto bounding_box = shape->get_bounding_box();
      if (bounding_box.has_value() == true)
      {
        bounded_shapes.emplace(shape, *bounding_box);
      }
      else
      {
        unbounded_shapes.emplace_back(shape);
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

    // The octree constructor expects iterators on "const ggo::object3d_abc *".
    auto adaptor = ggo::make_adaptor(bounded_shapes, [](const auto & it) { return it.first; });

    return { unbounded_shapes, new ggo::octree_raycaster::octree_t(adaptor.begin(), adaptor.end(), get_bounding_box, depth) };
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  octree_raycaster::octree_raycaster(const std::vector<const ggo::object3d_abc *> & objects, int depth)
  {
    auto result = build_octree(objects, depth);

    _objects_tree.reset(result.second);

    _brute_force_raycaster.reset(new brute_force_raycaster(result.first));
  }

  //////////////////////////////////////////////////////////////
  void octree_raycaster::process_ray(const ggo::ray3d_float & ray,
                                     std::function<bool(const ggo::object3d_abc *)> func,
                                     const ggo::object3d_abc * exclude_object1,
                                     const ggo::object3d_abc * exclude_object2) const
  {
    if (process_ray(*_objects_tree, ray, func, exclude_object1, exclude_object2) == false)
    {
      return;
    }

    _brute_force_raycaster->process_ray(ray, func, exclude_object1, exclude_object2);
  }

  //////////////////////////////////////////////////////////////
  void octree_raycaster::process_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                                         std::function<bool(const ggo::object3d_abc *)> func,
                                         const ggo::object3d_abc * exclude_object1,
                                         const ggo::object3d_abc * exclude_object2) const
  {
    if (process_segment(*_objects_tree, pos, dir, length, func, exclude_object1, exclude_object2) == false)
    {
      return;
    }

    _brute_force_raycaster->process_segment(pos, dir, length, func, exclude_object1, exclude_object2);
  }

  //////////////////////////////////////////////////////////////
  bool octree_raycaster::process_ray(const ggo::octree<const ggo::object3d_abc *, float> & node,
                                     const ggo::ray3d_float & ray,
                                     const std::function<bool(const ggo::object3d_abc *)> & func,
                                     const ggo::object3d_abc * exclude_object1,
                                     const ggo::object3d_abc * exclude_object2) const
  {
    ggo::box3d<float> bounding_box(node.bounding_box());

    if (bounding_box.intersect_ray(ray) == true)
    {
      for (const ggo::object3d_abc * object : node.data())
      {
        if (func(object) == false)
        {
          return false;
        }
      }

      for (const auto & child_node : node.children())
      {
        if (process_ray(child_node, ray, func, exclude_object1, exclude_object2) == false)
        {
          return false;
        }
      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  bool octree_raycaster::process_segment(const ggo::octree<const ggo::object3d_abc *, float> & node,
                                         const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                                         const std::function<bool(const ggo::object3d_abc *)> & func,
                                         const ggo::object3d_abc * exclude_object1,
                                         const ggo::object3d_abc * exclude_object2) const
  {
    ggo::box3d<float> bounding_box(node.bounding_box());

    if (bounding_box.intersect_segment(pos, dir, length) == true)
    {
      for (const ggo::object3d_abc * object : node.data())
      {
        if (func(object) == false)
        {
          return false;
        }
      }

      for (const auto & child_node : node.children())
      {
        if (process_segment(child_node, pos, dir, length, func, exclude_object1, exclude_object2) == false)
        {
          return false;
        }
      }
    }

    return true;
  }

#if 0
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
#endif
}
