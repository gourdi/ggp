#ifndef __GGO_INTERPOLATION_3D__
#define __GGO_INTERPOLATION_3D__

#include <kernel/ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// Definition.
namespace ggo
{
  template <typename input_t, typename interpolation_t>
  interpolation_t linear_interpolation3d(const input_t * input,
                                         int size_x, int size_y, int size_z,
                                         interpolation_t x, interpolation_t y, interpolation_t z);

}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename input_t, typename interpolation_t>
  interpolation_t linear_interpolation3d(const input_t * input,
                                         int size_x, int size_y, int size_z,
                                         interpolation_t x, interpolation_t y, interpolation_t z)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    int z_i = z >= 0 ? static_cast<int>(z) : static_cast<int>(z - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);
    GGO_ASSERT_LE(z_i, z);
    GGO_ASSERT_LE(z, z_i + 1);

    // Interpolate.
    interpolation_t dx = x - x_i;
    interpolation_t dy = y - y_i;
    interpolation_t dz = z - z_i;
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    GGO_ASSERT_GE(dz, 0);
    GGO_ASSERT_LE(dz, 1);
    
    // Loop indexes.
    int x_i0 = pos_mod(x_i, size_x);
    int y_i0 = pos_mod(y_i, size_y);
    int z_i0 = pos_mod(z_i, size_z);
    int x_i1 = pos_mod(x_i + 1, size_x);
    int y_i1 = pos_mod(y_i + 1, size_x);
    int z_i1 = pos_mod(z_i + 1, size_x);
    
    //              vZYX
    interpolation_t v000 = static_cast<interpolation_t>(input[z_i0 * size_x * size_y + y_i0 * size_x + x_i0]);
    interpolation_t v001 = static_cast<interpolation_t>(input[z_i0 * size_x * size_y + y_i0 * size_x + x_i1]);
    interpolation_t v010 = static_cast<interpolation_t>(input[z_i0 * size_x * size_y + y_i1 * size_x + x_i0]);
    interpolation_t v011 = static_cast<interpolation_t>(input[z_i0 * size_x * size_y + y_i1 * size_x + x_i1]);
    interpolation_t v100 = static_cast<interpolation_t>(input[z_i1 * size_x * size_y + y_i0 * size_x + x_i0]);
    interpolation_t v101 = static_cast<interpolation_t>(input[z_i1 * size_x * size_y + y_i0 * size_x + x_i1]);
    interpolation_t v110 = static_cast<interpolation_t>(input[z_i1 * size_x * size_y + y_i1 * size_x + x_i0]);
    interpolation_t v111 = static_cast<interpolation_t>(input[z_i1 * size_x * size_y + y_i1 * size_x + x_i1]);
    
    //              vYX
    interpolation_t v00 = (1 - dz) * v000 + dz * v100;
    interpolation_t v01 = (1 - dz) * v001 + dz * v101;
    interpolation_t v10 = (1 - dz) * v010 + dz * v110;
    interpolation_t v11 = (1 - dz) * v011 + dz * v111;
    
    //              vX
    interpolation_t v0 = (1 - dy) * v00 + dy * v10;
    interpolation_t v1 = (1 - dy) * v01 + dy * v11;

    return (1 - dx) * v0 + dx * v1;
  }
}

#endif
