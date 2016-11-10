#ifndef __GGO_CURVE__
#define __GGO_CURVE__

#include <vector>
#include <cmath>
#include <ggo_interpolation1d.h>

namespace ggo
{
  class invalid_points_number_exception : public std::exception { };

  template <typename T_X, typename T_Y>
  class curve_abc
  {
  public:

    using coord_t = T_X;
    using data_t = T_Y;

    void		                      reset();
    void		                      push_point(T_X x, T_Y y);

    const std::pair<T_X, T_Y> &   back() const { return _points.back(); }

    virtual	T_Y	                  evaluate(T_X x) const = 0;

  protected:

    std::vector<std::pair<T_X, T_Y>>	_points;
  };

  template <typename T_X, typename T_Y>
  class linear_curve : public curve_abc<T_X, T_Y>
  {
  public:

    T_Y evaluate(T_X x) const override;
  };

  template <typename T_X, typename T_Y>
  class cubic_curve : public curve_abc<T_X, T_Y>
  {
  public:

    T_Y evaluate(T_X x) const override;
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
  template <typename T_X, typename T_Y>
  void curve_abc<T_X, T_Y>::reset()
  {
    _points.clear();
  }

  //////////////////////////////////////////////////////////////
  template <typename T_X, typename T_Y>
  void curve_abc<T_X, T_Y>::push_point(T_X x, T_Y y)
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
  template <typename T_X, typename T_Y>
  T_Y linear_curve<T_X, T_Y>::evaluate(T_X x) const
  {
    if (this->_points.size() < 2)
    {
      throw ggo::invalid_points_number_exception();
    }
    
    int inf = -1;
    
    if ((this->_points.size() == 2) || (x <= this->_points[1].first))
    {
      inf = 0;
    }
    else
    if (x >= this->_points[this->_points.size() - 2].first)
    {
      inf = static_cast<int>(this->_points.size()) - 2;
    }
    else
    {
      for (int i = 1; i <= this->_points.size() - 2; ++i)
      {
        const auto & p_inf = this->_points[i];
        const auto & p_sup = this->_points[i + 1];

        if ((p_inf.first <= x) && (x <= p_sup.first))
        {
          inf = i;
          break;
        }
      }
    }
    
    const auto & p0 = this->_points[inf];
    const auto & p1 = this->_points[inf + 1];

    return linear_interpolation(p0.first, p0.second, p1.first, p1.second, x);
  }

  //////////////////////////////////////////////////////////////
  template <typename T_X, typename T_Y>
  T_Y cubic_curve<T_X, T_Y>::evaluate(T_X x) const
  {
    if (this->_points.size() < 4)
    {
      throw ggo::invalid_points_number_exception();
    }

    int inf = -1;
    
    if ((this->_points.size() == 4) || (x <= this->_points[2].first))
    {
      inf = 0;
    }
    else
    if (x >= this->_points[this->_points.size() - 3].first)
    {
      inf = static_cast<int>(this->_points.size()) - 4;
    }
    else
    {
      for (int i = 2; i <= this->_points.size() - 4; ++i)
      {
        const auto & p_inf = this->_points[i];
        const auto & p_sup = this->_points[i + 1];

        if ((p_inf.first <= x) && (x <= p_sup.first))
        {
          inf = i - 1;
          break;
        }
      }
    }
    
    const auto & p0 = this->_points[inf];
    const auto & p1 = this->_points[inf + 1];
    const auto & p2 = this->_points[inf + 2];
    const auto & p3 = this->_points[inf + 3];

    return cubic_interpolation(p0.first, p0.second, p1.first, p1.second, p2.first, p2.second, p3.first, p3.second, x);
  }
}

#endif

