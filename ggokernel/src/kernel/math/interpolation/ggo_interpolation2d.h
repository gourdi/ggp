#pragma once

#include <kernel/ggo_borders.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/interpolation/ggo_interpolation_polynom.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

// Linear interpolation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto linear_interpolation2d(in_t && in, scalar_t x, scalar_t y)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_sample_index(x);
    int y_i = lower_sample_index(y);

    // Y interpolation.
    scalar_t dy = y - sample_coordinate<scalar_t>(y_i);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);

    auto v00 = in(x_i, y_i);
    auto v01 = in(x_i, y_i + 1);
    auto v0 = (1 - dy) * v00 + dy * v01;

    auto v10 = in(x_i + 1, y_i);
    auto v11 = in(x_i + 1, y_i + 1);
    auto v1 = (1 - dy) * v10 + dy * v11;

    // X interpolation.
    scalar_t dx = x - sample_coordinate<scalar_t>(x_i);
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);

    return (1 - dx) * v0 + dx * v1;
  }
}

// Cubic interpolation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto cubic_interpolation2d(in_t && in, scalar_t x, scalar_t y)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_sample_index(x);
    int y_i = lower_sample_index(y);

    scalar_t x_zero = static_cast<scalar_t>(x_i);
    scalar_t y_zero = static_cast<scalar_t>(y_i);
    GGO_ASSERT_LE(x_zero, x);
    GGO_ASSERT_LE(y_zero, y);

    auto v00 = in(x_i - 1, y_i - 1);
    auto v10 = in(x_i + 0, y_i - 1);
    auto v20 = in(x_i + 1, y_i - 1);
    auto v30 = in(x_i + 2, y_i - 1);
    auto v0 = evaluate(make_interpolation_cubic(v00, v10, v20, v30), x - x_zero);

    auto v01 = in(x_i - 1, y_i);
    auto v11 = in(x_i + 0, y_i);
    auto v21 = in(x_i + 1, y_i);
    auto v31 = in(x_i + 2, y_i);
    auto v1 = evaluate(make_interpolation_cubic(v01, v11, v21, v31), x - x_zero);

    auto v02 = in(x_i - 1, y_i + 1);
    auto v12 = in(x_i + 0, y_i + 1);
    auto v22 = in(x_i + 1, y_i + 1);
    auto v32 = in(x_i + 2, y_i + 1);
    auto v2 = evaluate(make_interpolation_cubic(v02, v12, v22, v32), x - x_zero);

    auto v03 = in(x_i - 1, y_i + 2);
    auto v13 = in(x_i + 0, y_i + 2);
    auto v23 = in(x_i + 1, y_i + 2);
    auto v33 = in(x_i + 2, y_i + 2);
    auto v3 = evaluate(make_interpolation_cubic(v03, v13, v23, v33), x - x_zero);

    return evaluate(make_interpolation_cubic(v0, v1, v2, v3), y - y_zero);
  }
}

