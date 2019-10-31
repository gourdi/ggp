#ifndef __GGO_INTERPOLATION_2D__
#define __GGO_INTERPOLATION_2D__

#include <kernel/ggo_borders.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  enum class interpolation2d_type
  {
    bilinear,
    bicublic
  };

  template <typename data_t, typename getter, typename scalar_t>
  data_t linear_interpolation2d(const getter & in, scalar_t x, scalar_t y);

  template <typename data_t, typename getter, typename scalar_t>
  data_t cubic_interpolation2d(const getter & in, scalar_t x, scalar_t y);

  template <interpolation2d_type interp, typename data_t, typename getter, typename scalar_t>
  data_t interpolation2d(const getter & in, scalar_t x, scalar_t y);
}

/////////////////////////////////////////////////////////////////////
// Some usefull specializations.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <ggo::lines_order memory_lines_order, typename data_t>
  data_t linear_interpolation2d_mirror(const data_t * input, int width, int height, data_t x, data_t y)
  {
    auto in = [&](int x, int y) { return ggo::get2d_mirror<memory_lines_order>(input, x, y, width, height); };

    return linear_interpolation2d<data_t>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::lines_order memory_lines_order>
  uint8_t linear_interpolation2d_mirror(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_mirror<memory_lines_order>(input, x, y, width, height)); };

    return ggo::round_to<uint8_t>(linear_interpolation2d<float>(in, x, y));
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::lines_order memory_lines_order, typename data_t>
  inline data_t linear_interpolation2d_loop(const data_t * input, int width, int height, data_t x, data_t y)
  {
    auto in = [&](int x, int y) { return ggo::get2d_loop<lines_order>(input, x, y, width, height); };

    return linear_interpolation2d<data_t>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::lines_order memory_lines_order, typename data_t>
  data_t cubic_interpolation2d_mirror(const data_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return ggo::get2d_mirror<memory_lines_order>(input, x, y, width, height); };

    return cubic_interpolation2d<data_t>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::lines_order memory_lines_order>
  uint8_t cubic_interpolation2d_mirror(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_mirror<memory_lines_order>(input, x, y, width, height)); };

    return ggo::round_to<uint8_t>(cubic_interpolation2d<float>(in, x, y));
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename in_t, typename scalar_t>
  data_t linear_interpolation2d(in_t && in, scalar_t x, scalar_t y)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_bound(x);
    int y_i = lower_bound(y);

    GGO_ASSERT_GE(x, ggo::from_discrete_to_continuous<scalar_t>(x_i));
    GGO_ASSERT_LE(x, ggo::from_discrete_to_continuous<scalar_t>(x_i + 1));
    GGO_ASSERT_GE(y, ggo::from_discrete_to_continuous<scalar_t>(y_i));
    GGO_ASSERT_LE(y, ggo::from_discrete_to_continuous<scalar_t>(y_i + 1));

    // Interpolate.
    scalar_t dx = x - from_discrete_to_continuous<scalar_t>(x_i);
    scalar_t dy = y - from_discrete_to_continuous<scalar_t>(y_i);
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    data_t v00 = in(x_i, y_i);
    data_t v01 = in(x_i, y_i + 1);
    data_t v10 = in(x_i + 1, y_i);
    data_t v11 = in(x_i + 1, y_i + 1);
    
    data_t v0 = (1 - dy) * v00 + dy * v01;
    data_t v1 = (1 - dy) * v10 + dy * v11;

    return (1 - dx) * v0 + dx * v1;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename in_t, typename scalar_t>
  data_t cubic_interpolation2d(in_t && in, scalar_t x, scalar_t y)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_bound(x);
    int y_i = lower_bound(y);
    GGO_ASSERT_GE(x, ggo::from_discrete_to_continuous<scalar_t>(x_i));
    GGO_ASSERT_LE(x, ggo::from_discrete_to_continuous<scalar_t>(x_i + 1));
    GGO_ASSERT_GE(y, ggo::from_discrete_to_continuous<scalar_t>(y_i));
    GGO_ASSERT_LE(y, ggo::from_discrete_to_continuous<scalar_t>(y_i + 1));

    scalar_t x_zero = from_discrete_to_continuous<scalar_t>(x_i);
    scalar_t y_zero = from_discrete_to_continuous<scalar_t>(y_i);
    GGO_ASSERT_LE(x_zero, x);
    GGO_ASSERT_LE(y_zero, y);

    // Interpolate.
    data_t v00 = in(x_i - 1, y_i - 1);
    data_t v10 = in(x_i + 0, y_i - 1);
    data_t v20 = in(x_i + 1, y_i - 1);
    data_t v30 = in(x_i + 2, y_i - 1);
    data_t v0 = cubic_interpolation(v00, v10, v20, v30, x - x_zero);
    
    data_t v01 = in(x_i - 1, y_i);
    data_t v11 = in(x_i + 0, y_i);
    data_t v21 = in(x_i + 1, y_i);
    data_t v31 = in(x_i + 2, y_i);
    data_t v1 = cubic_interpolation(v01, v11, v21, v31, x - x_zero);
    
    data_t v02 = in(x_i - 1, y_i + 1);
    data_t v12 = in(x_i + 0, y_i + 1);
    data_t v22 = in(x_i + 1, y_i + 1);
    data_t v32 = in(x_i + 2, y_i + 1);
    data_t v2 = cubic_interpolation(v02, v12, v22, v32, x - x_zero);
    
    data_t v03 = in(x_i - 1, y_i + 2);
    data_t v13 = in(x_i + 0, y_i + 2);
    data_t v23 = in(x_i + 1, y_i + 2);
    data_t v33 = in(x_i + 2, y_i + 2);
    data_t v3 = cubic_interpolation(v03, v13, v23, v33, x - x_zero);
    
    return cubic_interpolation(v0, v1, v2, v3, y - y_zero);
  }

  /////////////////////////////////////////////////////////////////////
  template <interpolation2d_type interp, typename data_t, typename in_t, typename scalar_t>
  data_t interpolation2d(in_t && in, scalar_t x, scalar_t y)
  {
    if constexpr (interp == interpolation2d_type::bilinear)
    {
      return linear_interpolation2d<data_t>(in, x, y);
    }
    else if constexpr (interp == interpolation2d_type::bicublic)
    {
      return cubic_interpolation2d<data_t>(in, x, y);
    }
  }
}

#endif
