#pragma once

#include <stdexcept>
#include <kernel/ggo_kernel.h>

// Uniform bspline.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t uniform_bspline_linear(std::vector<data_t> control_points, scalar_t t)
  {
    t *= control_points.size() - 1;
    
    scalar_t t_inf = 0;
    scalar_t t_sup = 1;
    for (size_t i = 0; i < control_points.size() - 1; ++i)
    {
      if (t >= t_inf && t <= t_sup)
      {
        t -= t_inf;
        
        return (scalar_t(1) - t) * control_points[i] + t * control_points[i + 1];
      }
      
      t_inf += 1;
      t_sup += 1;
    }
    
    throw std::runtime_error("invalid linear uniform bspline parameters");
  }
    
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t uniform_bspline_quadratic(std::vector<data_t> control_points, scalar_t t)
  {
    t *= control_points.size() - 2;
    
    scalar_t t_inf = 0;
    scalar_t t_sup = 1;
    for (size_t i = 0; i < control_points.size() - 2; ++i)
    {
      if (t >= t_inf && t <= t_sup)
      {
        t -= t_inf;

        scalar_t w0 = ggo::square(1 - t) / 2;
        scalar_t w1 = -t * t + t + scalar_t(1) / scalar_t(2);
        scalar_t w2 = t * t / 2;

        return w0 * control_points[i] + w1 * control_points[i + 1] + w2 * control_points[i + 2];
      }
      
      t_inf += 1;
      t_sup += 1;
    }
    
    throw std::runtime_error("invalid quadratic uniform bspline parameters");
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t uniform_bspline_cubic(std::vector<data_t> control_points, scalar_t t)
  {
    t *= control_points.size() - 3;
    
    scalar_t t_inf = 0;
    scalar_t t_sup = 1;
    for (size_t i = 0; i < control_points.size() - 3; ++i)
    {
      if (t >= t_inf && t <= t_sup)
      {
        t -= t_inf;

        scalar_t w0 = (1 - t) * (1 - t) * (1 - t) / 6;
        scalar_t w1 = (4 - 6 * t * t + 3 * t * t * t) / 6;
        scalar_t w2 = (1 + 3 * (t + t * t - t * t * t)) / 6;
        scalar_t w3 = t * t * t / 6;

        return w0 * control_points[i] +
               w1 * control_points[i + 1] +
               w2 * control_points[i + 2] +
               w3 * control_points[i + 3];
      }
      
      t_inf += 1;
      t_sup += 1;
    }
    
    throw std::runtime_error("invalid cubic uniform bspline parameters");
  }
}

// Generic bspline.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t bspline_base(std::vector<data_t> control_points, std::vector<scalar_t> knots, scalar_t t)
  {
    GGO_ASSERT_EQ(2 * (control_points.size() - 1), knots.size());

    while (control_points.size() > 1)
    {
      std::vector<data_t> new_control_points;

      for (int i = 0; i < control_points.size() - 1; ++i)
      {
        size_t index1 = i + knots.size() / 2;
        size_t index2 = i;
        GGO_ASSERT_GT(index1, index2);

        scalar_t t1 = knots[index1];
        scalar_t t2 = knots[index2];
        GGO_ASSERT_GT(t1, t2);

        scalar_t w1 = t1 - t;
        scalar_t w2 = t - t2;
        GGO_ASSERT_GT(w1 + w2, scalar_t(0));

        data_t control_point = (w1 * control_points[i] + w2 * control_points[i + 1]) / (w1 + w2);

        new_control_points.push_back(control_point);
      }

      control_points = new_control_points;

      knots.erase(knots.begin());
      knots.pop_back();
    }

    return control_points.front();
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t bspline(int degree, std::vector<data_t> control_points, std::vector<scalar_t> knots, scalar_t t)
  {
    GGO_ASSERT_EQ(knots.size(), control_points.size() + degree - 1);

    size_t begin  = degree - 1;
    size_t end    = knots.size() - degree + 1;
    size_t offset = 0;
    for (size_t i = begin; i < end; ++i)
    {
      if (t >= knots[i] && t <= knots[i + 1])
      {
        break;
      }
      ++offset;
    }

    std::vector<data_t> sub_control_points(control_points.begin() + offset, control_points.begin() + offset + degree + 1);
    std::vector<scalar_t> sub_knots(knots.begin() + offset, knots.begin() + offset + 2 * degree);

    return bspline_base<data_t>(sub_control_points, sub_knots, t);
  }
}
