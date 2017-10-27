#ifndef __GGO_CURVE__
#define __GGO_CURVE__

#include <vector>
#include <cmath>
#include <ggo_interpolation1d.h>

namespace ggo
{
  class invalid_points_number_exception : public std::exception { };

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
  template <typename t_x, typename t_y>
  t_y cubic_curve<t_x, t_y>::evaluate(t_x x) const
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

