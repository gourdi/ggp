#pragma once

#include <kernel/math/interpolation/ggo_interpolation_polynom.h>
#include <kernel/math/ggo_discretization.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename make_polynom_t, typename scalar_t>
  auto interpolation1d(in_t && in, make_polynom_t && make_polynom, scalar_t x)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_sample_index(x);

    auto polynom = make_polynom(x_i);

    return evaluate(polynom, x - sample_coordinate<scalar_t>(x_i));
  }
}

// Linear sampling.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto linear_interpolation1d(in_t && in, scalar_t x)
  {
    auto make_polynom = [&](int lower_sample_index)
    {
      return make_interpolation_linear(in(lower_sample_index), in(lower_sample_index + 1));
    };

    return interpolation1d(in, make_polynom, x);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t linear_interpolation1d(const data_t * input, scalar_t x)
  {
    return linear_interpolation1d([&](int i) { return input[i]; }, x);
  }
}

// Cubic sampling.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto cubic_interpolation1d(in_t && in, scalar_t x)
  {
    auto make_polynom = [&](int lower_sample_index)
    {
      return make_interpolation_cubic(in(lower_sample_index - 1), in(lower_sample_index), in(lower_sample_index + 1), in(lower_sample_index + 2));
    };

    return interpolation1d(in, make_polynom, x);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t cubic_interpolation1d(const data_t * in, int size, scalar_t x)
  {
    return cubic_interpolation1d([&](int i) { return in[ggo::clamp(i, 0, size - 1)]; }, x);
  }
}

