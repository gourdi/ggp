#ifndef __GGO_KDTREE__
#define __GGO_KDTREE__

#include <vector>
#include <memory>
#include <kernel/ggo_vec.h>
#include <kernel/math/ggo_distance.h>
#include <kernel/ggo_kernel.h>

namespace ggo
{
  template <typename data_t, typename vec_t>
  class kdtree
  {
  public:

    struct data_point
    {
      vec_t   _pos;
      data_t  _data;
    };

                            kdtree(const std::vector<data_point> & points);

    std::vector<data_point> find_points(const vec_t & p, float radius) const;

  private:

    void                    find_points_aux(const vec_t & p, float radius, float hypot, std::vector<data_point> & result) const;

  private:

    data_point                              _data_point;
    int                                     _split_axis;
    std::unique_ptr<kdtree<data_t, vec_t>>  _inf_child;
    std::unique_ptr<kdtree<data_t, vec_t>>  _sup_child;
  };
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename vec_t>
  kdtree<data_t, vec_t>::kdtree(const std::vector<data_point> & points)
  {
    _split_axis = -1;

    if (points.empty() == true)
    {
      return;
    }

    float size_max = 0.f;
    for (int cur_dim = 0; cur_dim < vec_t::_n_dims; ++cur_dim)
    {
      auto sort_func = [&](const data_point & p1, const data_point & p2) { return p1._pos._coefs[cur_dim] < p2._pos._coefs[cur_dim]; };

      auto range = std::minmax_element(points.begin(), points.end(), sort_func);

      float size_cur = range.second->_pos._coefs[cur_dim] - range.first->_pos._coefs[cur_dim];
      GGO_ASSERT_GE(size_cur, 0.f);

      if (size_cur >= size_max)
      {
        size_max = size_cur;
        _split_axis = cur_dim;
      }
    }
    GGO_ASSERT(_split_axis >= 0 && _split_axis < vec_t::_n_dims);

    std::vector<data_point> sorted_points(points);

    auto sort_func = [&](const data_point & p1, const data_point & p2) { return p1._pos._coefs[_split_axis] < p2._pos._coefs[_split_axis]; };

    std::sort(sorted_points.begin(), sorted_points.end(), sort_func);

    auto middle = sorted_points.begin() + sorted_points.size() / 2;
    _data_point = *middle;

    std::vector<data_point> inf_points(sorted_points.begin(), middle);
    std::vector<data_point> sup_points(middle + 1, sorted_points.end());
    GGO_ASSERT_EQ(inf_points.size() + sup_points.size() + 1, points.size());

    if (inf_points.empty() == false)
    {
      _inf_child.reset(new kdtree<data_t, vec_t>(inf_points));
    }

    if (sup_points.empty() == false)
    {
      _sup_child.reset(new kdtree<data_t, vec_t>(sup_points));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename vec_t>
  std::vector<typename ggo::kdtree<data_t, vec_t>::data_point> kdtree<data_t, vec_t>::find_points(const vec_t & p, float radius) const
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
  template <typename data_t, typename vec_t>
  void kdtree<data_t, vec_t>::find_points_aux(const vec_t & p, float radius, float hypot, std::vector<data_point> & result) const
  {
    // First check current point.
    if (ggo::hypot(_data_point._pos, p) < hypot)
    {
      result.push_back(_data_point);
    }

    // Process children only if needed.
    if (_inf_child && p._coefs[_split_axis] - radius <= _data_point._pos._coefs[_split_axis])
    {
      _inf_child->find_points_aux(p, radius, hypot, result);
    }
    if (_sup_child && p._coefs[_split_axis] + radius >= _data_point._pos._coefs[_split_axis])
    {
      _sup_child->find_points_aux(p, radius, hypot, result);
    }
  }
}

#endif
