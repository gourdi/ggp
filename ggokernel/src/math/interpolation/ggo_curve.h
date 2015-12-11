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

    void		    reset();
    void		    push_point(T_X x, T_Y y);

    virtual	T_Y	evaluate(T_X x) const = 0;

  protected:
      
    struct curve_point
    {
      T_X _x;
      T_Y _y;
    };

    std::vector<curve_point>	_points;
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
      if (it->_x > x)
      {
        break;
      }
    }
    
    curve_point point;
    point._x = x;
    point._y = y;
    
    _points.insert(it, point);
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
    
    if ((this->_points.size() == 2) || (x <= this->_points[1]._x))
    {
      inf = 0;
    }
    else
    if (x >= this->_points[this->_points.size() - 2]._x)
    {
      inf = static_cast<int>(this->_points.size()) - 2;
    }
    else
    {
      for (int i = 1; i <= this->_points.size() - 2; ++i)
      {
        const auto & p_inf = this->_points[i];
        const auto & p_sup = this->_points[i + 1];

        if ((p_inf._x <= x) && (x <= p_sup._x))
        {
          inf = i;
          break;
        }
      }
    }
    
    const auto & p0 = this->_points[inf];
    const auto & p1 = this->_points[inf + 1];

    return linear_interpolation(p0._x, p0._y, p1._x, p1._y, x);
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
    
    if ((this->_points.size() == 4) || (x <= this->_points[2]._x))
    {
      inf = 0;
    }
    else
    if (x >= this->_points[this->_points.size() - 3]._x)
    {
      inf = static_cast<int>(this->_points.size()) - 4;
    }
    else
    {
      for (int i = 2; i <= this->_points.size() - 4; ++i)
      {
        const auto & p_inf = this->_points[i];
        const auto & p_sup = this->_points[i + 1];

        if ((p_inf._x <= x) && (x <= p_sup._x))
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

    return cubic_interpolation(p0._x, p0._y, p1._x, p1._y, p2._x, p2._y, p3._x, p3._y, x);
  }
}

#endif

