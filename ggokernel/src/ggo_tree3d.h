#ifndef __GGO_TREE3D__
#define __GGO_TREE3D__

#include <ggo_set3.h>
#include <ggo_distance3d.h>
#include <memory>
#include <algorithm>

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  class tree3d
  {
  public:

                                    tree3d(const std::vector<ggo::point3d_float> & points);

    void                            dump(std::ostream & os) const;

    std::vector<ggo::point3d_float> find_points(const ggo::point3d_float & p, float radius) const;

  private:

    void                            dump_aux(const std::string & indent, std::ostream & os) const;
    void                            find_points_aux(const ggo::point3d_float & p, float radius, float hypot, std::vector<ggo::point3d_float> & result) const;

  private:

    enum class split_axis
    {
      SPLIT_X,
      SPLIT_Y,
      SPLIT_Z
    };

    ggo::point3d_float      _pos;
    split_axis              _split_axis;
    std::unique_ptr<tree3d> _inf_child;
    std::unique_ptr<tree3d> _sup_child;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  tree3d::tree3d(const std::vector<ggo::point3d_float> & points)
  {
    auto sort_x = [](const ggo::point3d_float& p1, const ggo::point3d_float& p2) { return p1.x() < p2.x(); };
    auto sort_y = [](const ggo::point3d_float& p1, const ggo::point3d_float& p2) { return p1.y() < p2.y(); };
    auto sort_z = [](const ggo::point3d_float& p1, const ggo::point3d_float& p2) { return p1.z() < p2.z(); };

    auto range_x = std::minmax_element(points.begin(), points.end(), sort_x);
    auto range_y = std::minmax_element(points.begin(), points.end(), sort_y);
    auto range_z = std::minmax_element(points.begin(), points.end(), sort_z);
    GGO_ASSERT_LE(range_x.first, range_x.second);
    GGO_ASSERT_LE(range_y.first, range_y.second);
    GGO_ASSERT_LE(range_z.first, range_z.second);

    float size_x = range_x.second->x() - range_x.first->x();
    float size_y = range_y.second->y() - range_y.first->y();
    float size_z = range_z.second->z() - range_z.first->z();

    std::vector<ggo::point3d_float> sorted_points(points);

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
    _pos = *middle;

    std::vector<ggo::point3d_float> inf_points(sorted_points.begin(), middle);
    std::vector<ggo::point3d_float> sup_points(middle + 1, sorted_points.end());
    GGO_ASSERT_EQ(inf_points.size() + sup_points.size() + 1, points.size());

    if (inf_points.empty() == false)
    {
      _inf_child.reset(new tree3d(inf_points));
    }

    if (sup_points.empty() == false)
    {
      _sup_child.reset(new tree3d(sup_points));
    }
  }

  /////////////////////////////////////////////////////////////////////
  void tree3d::dump(std::ostream & os) const
  {
    dump_aux("", os);
  }

  /////////////////////////////////////////////////////////////////////
  void tree3d::dump_aux(const std::string & indent, std::ostream & os) const
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
  std::vector<ggo::point3d_float> tree3d::find_points(const ggo::point3d_float & p, float radius) const
  {
    std::vector<ggo::point3d_float> result;

    find_points_aux(p, radius, radius * radius, result);

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  void tree3d::find_points_aux(const ggo::point3d_float & p, float radius, float hypot, std::vector<ggo::point3d_float> & result) const
  {
    // First check current point.
    if (ggo::hypot(_pos, p) < hypot)
    {
      result.push_back(_pos);
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


