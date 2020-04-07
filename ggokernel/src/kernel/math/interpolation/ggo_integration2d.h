#pragma once

#include <kernel/math/ggo_discretization.h>

namespace ggo
{
  template <typename data_t>
  struct integration2d_surface
  {
    data_t k;
    data_t k_x;
    data_t k_y;
    data_t k_xy;

    template <typename scalar_t>
    data_t eval(scalar_t x, scalar_t y) const
    {
      static_assert(std::is_floating_point_v<scalar_t>);

      return k_xy * x * y + k_x * x + k_y * y + k;
    }

    template <typename scalar_t>
    data_t integrate(scalar_t x_inf, scalar_t x_sup, scalar_t y_inf, scalar_t y_sup) const
    {
      static_assert(std::is_floating_point_v<scalar_t>);

      // OctaveOnline:
      // syms kxy kx ky k x y xinf xsup yinf ysup
      // f = kxy * x * y + kx * x + ky * y + k
      // int(int(f, x, xinf, xsup), y, yinf, ysup)

      auto dx = x_sup - x_inf;
      auto dy = y_sup - y_inf;
      auto dx2 = x_sup * x_sup - x_inf * x_inf;
      auto dy2 = y_sup * y_sup - y_inf * y_inf;

      return
        k_xy * dx2 * dy2 / scalar_t(4) +
        k_y * dy2 * dx / scalar_t(2) +
        k_x * dx2 * dy / scalar_t(2) +
        k * dx * dy;
    }

    template <typename scalar_t>
    static integration2d_surface<data_t> make(scalar_t x_inf, scalar_t x_sup, scalar_t y_inf, scalar_t y_sup,
      data_t v_x_inf_y_inf, data_t v_x_sup_y_inf, data_t v_x_inf_y_sup, data_t v_x_sup_y_sup)
    {
      static_assert(std::is_floating_point_v<scalar_t>);

      // OctaveOnline:
      // syms kxy kx ky k x0 x1 y0 y1 v00 v01 v10 v11;
      // eqn1 = kxy * x0 * y0 + kx * x0 + ky * y0 + k == v00;
      // eqn2 = kxy * x1 * y0 + kx * x1 + ky * y0 + k == v10;
      // eqn3 = kxy * x0 * y1 + kx * x0 + ky * y1 + k == v01;
      // eqn4 = kxy * x1 * y1 + kx * x1 + ky * y1 + k == v11;
      // eqns = [eqn1 eqn2 eqn3 eqn4];
      // S = solve(eqns, [kxy kx ky k]);

      integration2d_surface result;

      auto inv = 1 / (x_inf * y_inf - x_inf * y_sup - x_sup * y_inf + x_sup * y_sup);

      result.k_xy = (v_x_inf_y_inf - v_x_inf_y_sup - v_x_sup_y_inf + v_x_sup_y_sup) * inv;
      result.k_x = (-v_x_inf_y_inf * y_sup + v_x_inf_y_sup * y_inf + v_x_sup_y_inf * y_sup - v_x_sup_y_sup * y_inf) * inv;
      result.k_y = (-v_x_inf_y_inf * x_sup + v_x_inf_y_sup * x_sup + v_x_sup_y_inf * x_inf - v_x_sup_y_sup * x_inf) * inv;
      result.k = (v_x_inf_y_inf * x_sup * y_sup - v_x_inf_y_sup * x_sup * y_inf - v_x_sup_y_inf * x_inf * y_sup + v_x_sup_y_sup * x_inf * y_inf) * inv;

      return result;
    }
  };

  /////////////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto linear_integration2d(in_t && in, scalar_t x_inf, scalar_t x_sup, scalar_t y_inf, scalar_t y_sup)
  {
    int xs_inf = lower_sample_index(x_inf);
    int xs_sup = upper_sample_index(x_sup);
    int ys_inf = lower_sample_index(y_inf);
    int ys_sup = upper_sample_index(y_sup);
    GGO_ASSERT_LT(xs_inf, xs_sup);
    GGO_ASSERT_LT(ys_inf, ys_sup);

    using data_t = std::decay_t<std::invoke_result_t<in_t, int, int>>;

    data_t r(0);

    for (int ys = ys_inf; ys < ys_sup; ++ys)
    {
      for (int xs = xs_inf; xs < xs_sup; ++xs)
      {
        scalar_t x_inf_cur = ggo::sample_coordinate<scalar_t>(xs);
        scalar_t y_inf_cur = ggo::sample_coordinate<scalar_t>(ys);

        scalar_t x_sup_cur = ggo::sample_coordinate<scalar_t>(xs + 1);
        scalar_t y_sup_cur = ggo::sample_coordinate<scalar_t>(ys + 1);

        auto surface = integration2d_surface<data_t>::make(
          x_inf_cur, x_sup_cur, y_inf_cur, y_sup_cur,
          in(xs, ys), in(xs + 1, ys), in(xs, ys + 1), in(xs + 1, ys + 1));

        x_inf_cur = std::max(x_inf_cur, x_inf);
        y_inf_cur = std::max(y_inf_cur, y_inf);

        x_sup_cur = std::min(x_sup_cur, x_sup);
        y_sup_cur = std::min(y_sup_cur, y_sup);

        r += surface.integrate(x_inf_cur, x_sup_cur, y_inf_cur, y_sup_cur);
      }
    }

    return r;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  auto linear_integration2d(const data_t * input, int width, int height, scalar_t x_inf, scalar_t x_sup, scalar_t y_inf, scalar_t y_sup)
  {
    return linear_integration2d([&](int x, int y) { return input[y * width + x]; }, x_inf, x_sup, y_inf, y_sup);
  }
}


