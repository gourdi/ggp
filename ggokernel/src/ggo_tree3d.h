#ifndef __GGO_TREE3D__
#define __GGO_TREE3D__

#include <ggo_set3.h>
#include <ggo_distance3d.h>
#include <memory>
#include <algorithm>

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class tree3d
  {
  public:

                                                  tree3d(const std::vector<std::pair<ggo::point3d_float, T>> & points);

    void                                          dump(std::ostream & os) const;

    std::vector<std::pair<ggo::point3d_float, T>> find_points(const ggo::point3d_float & p, float radius) const;

  private:

    void                                          dump_aux(const std::string & indent, std::ostream & os) const;
    void                                          find_points_aux(const ggo::point3d_float & p, float radius, float hypot, std::vector<std::pair<ggo::point3d_float, T>> & result) const;

  private:

    enum class split_axis
    {
      SPLIT_X,
      SPLIT_Y,
      SPLIT_Z
    };

    ggo::point3d_float      _pos;
    T                       _data;
    split_axis              _split_axis;
    std::unique_ptr<tree3d> _inf_child;
    std::unique_ptr<tree3d> _sup_child;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  tree3d<T>::tree3d(const std::vector<std::pair<ggo::point3d_float, T>> & points)
  {
    auto sort_x = [](const std::pair<ggo::point3d_float, T> & p1, const std::pair<ggo::point3d_float, T> & p2) { return p1.first.x() < p2.first.x(); };
    auto sort_y = [](const std::pair<ggo::point3d_float, T> & p1, const std::pair<ggo::point3d_float, T> & p2) { return p1.first.y() < p2.first.y(); };
    auto sort_z = [](const std::pair<ggo::point3d_float, T> & p1, const std::pair<ggo::point3d_float, T> & p2) { return p1.first.z() < p2.first.z(); };

    auto range_x = std::minmax_element(points.begin(), points.end(), sort_x);
    auto range_y = std::minmax_element(points.begin(), points.end(), sort_y);
    auto range_z = std::minmax_element(points.begin(), points.end(), sort_z);

    float size_x = range_x.second->first.x() - range_x.first->first.x();
    float size_y = range_y.second->first.y() - range_y.first->first.y();
    float size_z = range_z.second->first.z() - range_z.first->first.z();
    GGO_ASSERT_GE(size_x, 0.f);
    GGO_ASSERT_GE(size_y, 0.f);
    GGO_ASSERT_GE(size_z, 0.f);

    std::vector<std::pair<ggo::point3d_float, T>> sorted_points(points);

    if (size_x >= size_y && size_x >= size_z)
    {
      _split_axis = split_axis::SPLIT_X;
      std::sort(sorted_points.begin(), sorted_points.end(), sort_x);
    }
    else
    if (size_y >= size_x && size_y >= size_z)
    {
      _split_axis = split_axis::SPLIT_Y;
      std::sort(sorted_points.begin(), sorted_points.end(), sort_y);
    }
    else
    {
      _split_axis = split_axis::SPLIT_Z;
      std::sort(sorted_points.begin(), sorted_points.end(), sort_z);
    }

    auto middle = sorted_points.begin() + sorted_points.size() / 2;
    _pos = middle->first;
    _data = middle->second;

    std::vector<std::pair<ggo::point3d_float, T>> inf_points(sorted_points.begin(), middle);
    std::vector<std::pair<ggo::point3d_float, T>> sup_points(middle + 1, sorted_points.end());
    GGO_ASSERT_EQ(inf_points.size() + sup_points.size() + 1, points.size());

    if (inf_points.empty() == false)
    {
      _inf_child.reset(new tree3d<T>(inf_points));
    }

    if (sup_points.empty() == false)
    {
      _sup_child.reset(new tree3d<T>(sup_points));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void tree3d<T>::dump(std::ostream & os) const
  {
    dump_aux("", os);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void tree3d<T>::dump_aux(const std::string & indent, std::ostream & os) const
  {
    std::cout << indent;

    switch (_split_axis)
    {
    case split_axis::SPLIT_X:
      os << "x ";
      break;
    case split_axis::SPLIT_Y:
      os << "y ";
      break;
    case split_axis::SPLIT_Z:
      os << "z ";
      break;
    }

    os << _pos << std::endl;

    if (_inf_child)
    {
      _inf_child->dump_aux(indent + " ", os);
    }

    if (_sup_child)
    {
      _sup_child->dump_aux(indent + " ", os);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<std::pair<ggo::point3d_float, T>> tree3d<T>::find_points(const ggo::point3d_float & p, float radius) const
  {
    std::vector<std::pair<ggo::point3d_float, T>> result;

    find_points_aux(p, radius, radius * radius, result);

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void tree3d<T>::find_points_aux(const ggo::point3d_float & p, float radius, float hypot, std::vector<std::pair<ggo::point3d_float, T>> & result) const
  {
    // First check current point.
    if (ggo::hypot(_pos, p) < hypot)
    {
      result.push_back(std::make_pair(_pos, _data));
    }

    // Process children only if needed.
    switch (_split_axis)
    {
    case split_axis::SPLIT_X:
      if (_inf_child && p.x() - radius <= _pos.x())
      {
        _inf_child->find_points_aux(p, radius, hypot, result);
      }
      if (_sup_child && p.x() + radius >= _pos.x())
      {
        _sup_child->find_points_aux(p, radius, hypot, result);
      }
      break;
    case split_axis::SPLIT_Y:
      if (_inf_child && p.y() - radius <= _pos.y())
      {
        _inf_child->find_points_aux(p, radius, hypot, result);
      }
      if (_sup_child && p.y() + radius >= _pos.y())
      {
        _sup_child->find_points_aux(p, radius, hypot, result);
      }
      break;
    case split_axis::SPLIT_Z:
      if (_inf_child && p.z() - radius <= _pos.z())
      {
        _inf_child->find_points_aux(p, radius, hypot, result);
      }
      if (_sup_child && p.z() + radius >= _pos.z())
      {
        _sup_child->find_points_aux(p, radius, hypot, result);
      }
      break;
    }
  }
}

#endif


