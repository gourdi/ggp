#ifndef __GGO_CURVE__
#define __GGO_CURVE__

#include <vector>
#include <cmath>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

namespace ggo
{
  template <typename t_x, typename t_y>
  class curve_abc
  {
  public:

    using coord_t = t_x;
    using data_t = t_y;

    void		                      reset();
    void		                      push_point(t_x x, t_y y);

    const std::pair<t_x, t_y> &   back() const { return _points.back(); }

    virtual	t_y	                  evaluate(t_x x) const = 0;

  protected:

    std::vector<std::pair<t_x, t_y>>	_points;
  };

  template <typename t_x, typename t_y>
  class linear_curve : public curve_abc<t_x, t_y>
  {
  public:

    t_y evaluate(t_x x) const override;
  };

  template <typename t_x, typename t_y>
  class cubic_curve : public curve_abc<t_x, t_y>
  {
  public:

    t_y evaluate(t_x x) const override;
  };
}

//////////////////////////////////////////////////////////////////
// Type aliases.
namespace ggo
{
  using linear_curve_double = linear_curve<double, double>;
  using linear_curve_float	= linear_curve<float, float>;
  using cubic_curve_double	= cubic_curve<double, double>;
  using cubic_curve_float		= cubic_curve<float, float>;
}

//////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  void curve_abc<t_x, t_y>::reset()
  {
    _points.clear();
  }

  //////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  void curve_abc<t_x, t_y>::push_point(t_x x, t_y y)
  {
    auto it = _points.begin();
    
    for (; it != _points.end(); ++it)
    {
      if (it->first > x)
      {
        break;
      }
    }
    
    _points.insert(it, std::make_pair(x, y));
  }

  //////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  t_y linear_curve<t_x, t_y>::evaluate(t_x x) const
  {
    if (this->_points.size() < 2)
    {
      throw std::runtime_error("not enough points in the curve");
    }

    if (x <= this->_points.front().first)
    {
      return this->_points.front().second;
    }

    for (int i = 1; i < this->_points.size(); ++i)
    {
      if (x <= this->_points[i].first)
      {
        return linear_interpolation(this->_points[i - 1].first,
                                    this->_points[i - 1].second,
                                    this->_points[i].first,
                                    this->_points[i].second, x);
      }
    }

    return this->_points.back().second;
  }

  //////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  t_y cubic_curve<t_x, t_y>::evaluate(t_x x) const
  {
    if (this->_points.size() < 2)
    {
      throw std::runtime_error("not enough points in the curve");
    }

    if (x <= this->_points.front().first)
    {
      return this->_points.front().second;
    }

    for (int i = 1; i < this->_points.size(); ++i)
    {
      if (x <= this->_points[i].first)
      {
        t_x x1 = this->_points[i - 1].first;
        t_x x2 = this->_points[i].first;
        const auto & y1 = this->_points[i - 1].second;
        const auto & y2 = this->_points[i].second;

        t_x x0 = i - 2 < 0 ? 2 * x1 - x2 : this->_points[i - 2].first;
        const auto & y0 = i - 2 < 0 ? y2 : this->_points[i - 2].second;

        t_x x3 = i + 1 >= this->_points.size() ? 2 * x2 - x1 : this->_points[i + 1].first;
        const auto & y3 = i + 1 >= this->_points.size() ? y1 : this->_points[i + 1].second;

        return cubic_interpolation(x0, y0, x1, y1, x2, y2, x3, y3, x);
      }
    }

    return this->_points.back().second;
  }
}

#endif

