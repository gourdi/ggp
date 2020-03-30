#pragma once

#include <vector>
#include <cmath>
#include <kernel/math/interpolation/ggo_interpolation_polynom.h>

namespace ggo
{
  template <typename scalar_t_, typename data_t_>
  class curve_abc
  {
  public:

    using scalar_t = scalar_t_;
    using data_t = data_t_;

    void		                            reset();
    void		                            push_point(scalar_t x, data_t y);

    const std::pair<scalar_t, data_t> & back() const { return _points.back(); }

    virtual	data_t	                    evaluate(scalar_t x) const = 0;

  protected:

    std::vector<std::pair<scalar_t, data_t>>	_points;
  };

  template <typename scalar_t, typename data_t>
  class linear_curve : public curve_abc<scalar_t, data_t>
  {
  public:

    data_t evaluate(scalar_t x) const override;
  };

  template <typename scalar_t, typename data_t>
  class cubic_curve : public curve_abc<scalar_t, data_t>
  {
  public:

    data_t evaluate(scalar_t x) const override;
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
  template <typename scalar_t, typename data_t>
  void curve_abc<scalar_t, data_t>::reset()
  {
    _points.clear();
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename data_t>
  void curve_abc<scalar_t, data_t>::push_point(scalar_t x, data_t y)
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
  template <typename scalar_t, typename data_t>
  data_t linear_curve<scalar_t, data_t>::evaluate(scalar_t x) const
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
        auto interpolation = make_interpolation_linear(
          this->_points[i - 1].first,
          this->_points[i - 1].second,
          this->_points[i].first,
          this->_points[i].second);

        return ggo::evaluate(interpolation, x);
      }
    }

    return this->_points.back().second;
  }

  //////////////////////////////////////////////////////////////
  template <typename scalar_t, typename data_t>
  data_t cubic_curve<scalar_t, data_t>::evaluate(scalar_t x) const
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
        scalar_t x1 = this->_points[i - 1].first;
        scalar_t x2 = this->_points[i].first;
        const auto & y1 = this->_points[i - 1].second;
        const auto & y2 = this->_points[i].second;

        scalar_t x0 = i - 2 < 0 ? 2 * x1 - x2 : this->_points[i - 2].first;
        const auto & y0 = i - 2 < 0 ? y2 : this->_points[i - 2].second;

        scalar_t x3 = i + 1 >= this->_points.size() ? 2 * x2 - x1 : this->_points[i + 1].first;
        const auto & y3 = i + 1 >= this->_points.size() ? y1 : this->_points[i + 1].second;

        auto interpolation = make_interpolation_cubic(x0, y0, x1, y1, x2, y2, x3, y3);

        return ggo::evaluate(interpolation, x);
      }
    }

    return this->_points.back().second;
  }
}

