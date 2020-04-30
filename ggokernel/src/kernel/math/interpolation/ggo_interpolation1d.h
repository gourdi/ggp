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

// Linear sampling, fixed step.
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

// Linear sampling, variable step.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename iterator_t, typename scalar_t>
  auto linear_interpolation1d(iterator_t begin, iterator_t end, scalar_t x)
  {
    auto get_prv = [](auto it) { auto prv = it; --prv; return prv; };

    if (std::distance(begin, end) < 2)
    {
      throw std::runtime_error("not enough points in the curve");
    }

    if (x <= begin->first)
    {
      return begin->second;
    }

    for (auto cur = begin; cur != end; ++cur)
    {
      if (x <= cur->first)
      {
        auto prv = get_prv(cur);
        auto interpolation = make_interpolation_linear(prv->first, prv->second, cur->first, cur->second);

        return ggo::evaluate(interpolation, x);
      }
    }

    return get_prv(end)->second;
  }
}

// Cubic sampling, fixed step.
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

// Cubic sampling, variable step.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename iterator_t, typename scalar_t>
  auto cubic_interpolation1d(iterator_t begin, iterator_t end, scalar_t x)
  {
    if (std::distance(begin, end) < 2)
    {
      throw std::runtime_error("not enough points in the curve");
    }

    auto get_prv = [](auto it) { auto prv = it; --prv; return prv; };
    auto get_nxt = [](auto it) { auto nxt = it; ++nxt; return nxt; };

    if (x <= begin->first)
    {
      return begin->second;
    }

    for (auto cur = get_nxt(begin); cur != end; ++cur)
    {
      if (x <= cur->first)
      {
        auto prv = get_prv(cur);

        scalar_t x1 = prv->first;
        scalar_t x2 = cur->first;
        const auto& y1 = prv->second;
        const auto& y2 = cur->second;

        scalar_t x0    = prv == begin ? 2 * x1 - x2 : get_prv(prv)->first;
        const auto& y0 = prv == begin ? y2 : get_prv(prv)->second;

        auto nxt = get_nxt(cur);
        scalar_t x3    = nxt == end ? 2 * x2 - x1 : nxt->first;
        const auto& y3 = nxt == end ? y1 : nxt->second;

        auto interpolation = make_interpolation_cubic(x0, y0, x1, y1, x2, y2, x3, y3);

        return ggo::evaluate(interpolation, x);
      }
    }

    return get_prv(end)->second;
  }
}

