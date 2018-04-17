#ifndef __GGO_OCTREE__
#define __GGO_OCTREE__

#include <vector>
#include <iostream>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

namespace ggo
{
  template <typename data_t, typename floating_point_t>
  class octree
  {
  public:

    template <typename iterator_t, typename get_bounding_box_func>
    octree(iterator_t object_begin, iterator_t object_end, get_bounding_box_func get_bounding_box, int depth);

    void dump(const std::string & indent = "") const;

    int get_depth() const;

    const ggo::box3d_data<floating_point_t> & bounding_box() const { return _aabb; }
    const std::vector<data_t> &               data() const { return _data; }
    const auto &                              children() const { return _children; }
    const auto &                              operator[](size_t i) const { return _children[i]; }

  private:

    template <typename iterator_t, typename get_bounding_box_func>
    static ggo::box3d_data<floating_point_t> merge_bounding_boxes(iterator_t object_begin, iterator_t object_end, get_bounding_box_func get_bounding_box)
    {
      auto it = object_begin;
      auto aabb = get_bounding_box(*it++);
      for (; it != object_end; ++it)
      {
        aabb = get_union(aabb, get_bounding_box(*it));
      }
      return aabb;
    };

  private:

    std::vector<data_t>                             _data;
    ggo::box3d_data<floating_point_t>               _aabb;
    std::vector<octree<data_t, floating_point_t>> _children;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t, typename floating_point_t>
  template <typename iterator_t, typename get_bounding_box_func>
  octree<data_t, floating_point_t>::octree(iterator_t object_begin, iterator_t object_end, get_bounding_box_func get_bounding_box, int depth)
    :
    _aabb(merge_bounding_boxes(object_begin, object_end, get_bounding_box))
  {
    // Compute the 8 children.
    using child_t = std::tuple<box3d_data<floating_point_t>, std::vector<data_t>>;

    auto center = _aabb.get_center();

    std::vector<child_t> children;
    children.push_back(child_t({ center.x(), _aabb._x_max, center.y(), _aabb._y_max, center.z(), _aabb._z_max }, {}));
    children.push_back(child_t({ center.x(), _aabb._x_max, center.y(), _aabb._y_max, _aabb._z_min, center.z() }, {}));
    children.push_back(child_t({ center.x(), _aabb._x_max, _aabb._y_min, center.y(), center.z(), _aabb._z_max }, {}));
    children.push_back(child_t({ center.x(), _aabb._x_max, _aabb._y_min, center.y(), _aabb._z_min, center.z() }, {}));
    children.push_back(child_t({ _aabb._x_min, center.x(), center.y(), _aabb._y_max, center.z(), _aabb._z_max }, {}));
    children.push_back(child_t({ _aabb._x_min, center.x(), center.y(), _aabb._y_max, _aabb._z_min, center.z() }, {}));
    children.push_back(child_t({ _aabb._x_min, center.x(), _aabb._y_min, center.y(), center.z(), _aabb._z_max }, {}));
    children.push_back(child_t({ _aabb._x_min, center.x(), _aabb._y_min, center.y(), _aabb._z_min, center.z() }, {}));

    // Find objects intersecting each child box.
    auto get_intersecting_shapes = [&](const box3d_data<floating_point_t> & box)
    {
      std::vector<data_t> objects;
      for (auto it = object_begin; it != object_end; ++it)
      {
        if (test_intersection(box, get_bounding_box(*it)) == true)
        {
          objects.push_back(*it);
        }
      }
      return objects;
    };

    for (auto & child : children)
    {
      std::get<1>(child) = get_intersecting_shapes(std::get<0>(child));
    }

    // Remove empty child nodes.
    ggo::remove_if(children, [](const auto & child) { return std::get<1>(child).empty(); });

    // Update children's bounding box according to their objects.
    for (auto & child : children)
    {
      std::get<0>(child) = merge_bounding_boxes(std::get<1>(child).begin(), std::get<1>(child).end(), get_bounding_box);
    }

    // Remove children that have the same bounding box.
    remove_duplicates(children, [](const auto & obj1, const auto & obj2) { return std::get<0>(obj1) == std::get<0>(obj2); });

    // Recursion?
    if (depth == 0 || children.size() == 1)
    {
      for (auto it = object_begin; it != object_end; ++it)
      {
        _data.emplace_back(*it);
      }

      //_data = std::vector<data_t>(object_begin, object_end);
    }
    else
    {
      // Recursion: create child nodes.
      for (const auto & child : children)
      {
        const auto & objects = std::get<1>(child);
        if (objects.empty() == false)
        {
          _children.emplace_back(objects.begin(), objects.end(), get_bounding_box, depth - 1);
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, typename floating_point_t>
  void octree<data_t, floating_point_t>::dump(const std::string & indent) const
  {
    std::cout << indent << "box: " << _aabb << "; " << _data.size() << " object(s)" << std::endl;

    for (const auto & child : _children)
    {
      child.dump(indent + "  ");
    }
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, typename floating_point_t>
  int octree<data_t, floating_point_t>::get_depth() const
  {
    int sub_depth = 0;
    for (const auto & child : _children)
    {
      sub_depth = std::max(sub_depth, child.get_depth());
    }
    return 1 + sub_depth;
  }
}

#endif
