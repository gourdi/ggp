#pragma once

#include <kernel/ggo_kernel.h>
#include <kernel/math/ggo_discretization.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename in_t, typename scalar_t>
  auto linear_interpolation3d(in_t && in, scalar_t x, scalar_t y, scalar_t z)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    int x_i = lower_sample_index(x);
    int y_i = lower_sample_index(y);
    int z_i = lower_sample_index(z);

    // Z interpolation.
    scalar_t dz = z - sample_coordinate<scalar_t>(z_i);
    GGO_ASSERT_GE(dz, 0);
    GGO_ASSERT_LE(dz, 1);

    auto v000 = in(x_i, y_i, z_i);
    auto v001 = in(x_i, y_i, z_i + 1);
    auto v00 = (1 - dz) * v000 + dz * v001;

    auto v100 = in(x_i + 1, y_i, z_i);
    auto v101 = in(x_i + 1, y_i, z_i + 1);
    auto v10 = (1 - dz) * v100 + dz * v101;

    auto v010 = in(x_i, y_i + 1, z_i);
    auto v011 = in(x_i, y_i + 1, z_i + 1);
    auto v01 = (1 - dz) * v010 + dz * v011;

    auto v110 = in(x_i + 1, y_i + 1, z_i);
    auto v111 = in(x_i + 1, y_i + 1, z_i + 1);
    auto v11 = (1 - dz) * v110 + dz * v111;

    // Y interpolation.
    scalar_t dy = y - sample_coordinate<scalar_t>(y_i);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    auto v0 = (1 - dy) * v00 + dy * v01;
    auto v1 = (1 - dy) * v10 + dy * v11;

    // X interpolation.
    scalar_t dx = x - sample_coordinate<scalar_t>(x_i);
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);

    return (1 - dx) * v0 + dx * v1;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  auto linear_interpolation3d(const data_t * input, int size_x, int size_y, int size_z, scalar_t x, scalar_t y, scalar_t z)
  {
    auto in = [&](int x, int y, int z) { return input[(z * size_y + y) * size_x + x]; };

    return linear_interpolation3d(in, x, y, z);
  }
}

