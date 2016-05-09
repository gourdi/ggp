#ifndef __GGO_KDTREE__
#define __GGO_KDTREE__

#include <array>
#include <vector>
#include <memory>
#include <ggo_distance2d.h>
#include <ggo_distance3d.h>
#include <ggo_kernel.h>

namespace ggo
{
  template <int dim, typename data_type>
  class kdtree
  {
  public:

    struct data_point
    {
      std::array<float, dim>  _pos;
      data_type               _data;
    };

                            kdtree(const std::vector<data_point> & points);

    std::vector<data_point> find_points(const std::array<float, dim> & p, float radius) const;

  private:

    void                    find_points_aux(const std::array<float, dim> & p, float radius, float hypot, std::vector<data_point> & result) const;

  private:

    data_point                              _data_point;
    int                                     _split_axis;
    std::unique_ptr<kdtree<dim, data_type>> _inf_child;
    std::unique_ptr<kdtree<dim, data_type>> _sup_child;
  };
}


/////////////////////////////////////////////////////////////////////
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <int dim, typename data_type>
  kdtree<dim, data_type>::kdtree(const std::vector<data_point> & points)
  {
    _split_axis = -1;

    if (points.empty() == true)
    {
      return;
    }

    float size_max = 0.f;
    for (int cur_dim = 0; cur_dim < dim; ++cur_dim)
    {
      auto sort_func = [&](const data_point & p1, const data_point & p2) { return p1._pos[cur_dim] < p2._pos[cur_dim]; };

      auto range = std::minmax_element(points.begin(), points.end(), sort_func);

      float size_cur = range.second->_pos[cur_dim] - range.first->_pos[cur_dim];
      GGO_ASSERT_GE(size_cur, 0.f);

      if (size_cur >= size_max)
      {
        size_max = size_cur;
        _split_axis = cur_dim;
      }
    }
    GGO_ASSERT(_split_axis >= 0 && _split_axis < dim);

    std::vector<data_point> sorted_points(points);

    auto sort_func = [&](const data_point & p1, const data_point & p2) { return p1._pos[_split_axis] < p2._pos[_split_axis]; };

    std::sort(sorted_points.begin(), sorted_points.end(), sort_func);

    auto middle = sorted_points.begin() + sorted_points.size() / 2;
    _data_point = *middle;

    std::vector<data_point> inf_points(sorted_points.begin(), middle);
    std::vector<data_point> sup_points(middle + 1, sorted_points.end());
    GGO_ASSERT_EQ(inf_points.size() + sup_points.size() + 1, points.size());

    if (inf_points.empty() == false)
    {
      _inf_child.reset(new kdtree<dim, data_type>(inf_points));
    }

    if (sup_points.empty() == false)
    {
      _sup_child.reset(new kdtree<dim, data_type>(sup_points));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <int dim, typename data_type>
  std::vector<typename ggo::kdtree<dim, data_type>::data_point> kdtree<dim, data_type>::find_points(const std::array<float, dim> & p, float radius) const
  {
    std::vector<data_point> result;

    if (_split_axis < 0)
    {
      return result;
    }

    find_points_aux(p, radius, radius * radius, result);

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <int dim, typename data_type>
  void kdtree<dim, data_type>::find_points_aux(const std::array<float, dim> & p, float radius, float hypot, std::vector<data_point> & result) const
  {
    // First check current point.
    if (ggo::hypot(_data_point._pos, p) < hypot)
    {
      result.push_back(_data_point);
    }

    // Process children only if needed.
    if (_inf_child && p[_split_axis] - radius <= _data_point._pos[_split_axis])
    {
      _inf_child->find_points_aux(p, radius, hypot, result);
    }
    if (_sup_child && p[_split_axis] + radius >= _data_point._pos[_split_axis])
    {
      _sup_child->find_points_aux(p, radius, hypot, result);
    }
  }
}

#endif
