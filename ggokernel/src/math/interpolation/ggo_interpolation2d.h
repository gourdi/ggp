#ifndef __GGO_INTERPOLATION_2D__
#define __GGO_INTERPOLATION_2D__

#include <ggo_edges_management.h>
#include <ggo_interpolation1d.h>
#include <ggo_memory_layouts.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  template <typename data_t, typename getter, typename interpolation_t>
  data_t bilinear_interpolation2d(const getter & in, interpolation_t x, interpolation_t y);

  template <typename data_t, typename getter, typename interpolation_t>
  data_t bicubic_interpolation2d(const getter & in, interpolation_t x, interpolation_t y);
}

/////////////////////////////////////////////////////////////////////
// Some usefull specializations.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename interpolation_t, ggo::direction lines_memory_order>
  inline data_t bilinear_interpolation2d_mirror(const data_t * input, int width, int height, int line_byte_step, interpolation_t x, interpolation_t y)
  {
    auto in = [&](int x, int y) { return ggo::get2d_mirror<lines_memory_order>(input, x, y, width, height, line_byte_step); };

    return bilinear_interpolation2d<data_t>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::direction lines_memory_order>
  inline uint8_t bilinear_interpolation2d_mirror(const uint8_t * input, int width, int height, int line_byte_step, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_mirror<lines_memory_order>(input, x, y, width, height, line_byte_step)); };

    return ggo::round_to<uint8_t>(bilinear_interpolation2d<float>(in, x, y));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename interpolation_t, ggo::direction lines_memory_order>
  inline data_t bicubic_interpolation2d_mirror(const data_t * input, int width, int height, int line_byte_step, interpolation_t x, interpolation_t y)
  {
    auto in = [&](int x, int y) { return ggo::get2d_mirror<ggo::lines_typed_memory_access<lines_memory_order>>(input, x, y, width, height, line_byte_step); };

    return bicubic_interpolation2d<data_t>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <ggo::direction lines_memory_order>
  inline uint8_t bicubic_interpolation2d_mirror(const uint8_t * input, int width, int height, int line_byte_step, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_mirror<lines_memory_order>(input, x, y, width, height, line_byte_step)); };

    return ggo::round_to<uint8_t>(bicubic_interpolation2d<float>(in, x, y));
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename getter, typename interpolation_t>
  data_t bilinear_interpolation2d(const getter & in, interpolation_t x, interpolation_t y)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    interpolation_t dx = x - x_i;
    interpolation_t dy = y - y_i;
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    data_t v00 = in(x_i, y_i);
    data_t v01 = in(x_i, y_i + 1);
    data_t v10 = in(x_i + 1, y_i);
    data_t v11 = in(x_i + 1, y_i + 1);
    
    data_t v1 = (1 - dy) * v00 + dy * v01;
    data_t v2 = (1 - dy) * v10 + dy * v11;

    return (1 - dx) * v1 + dx * v2;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename getter, typename interpolation_t>
  data_t bicubic_interpolation2d(const getter & in, interpolation_t x, interpolation_t y)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    data_t v00 = in(x_i - 1, y_i - 1);
    data_t v10 = in(x_i + 0, y_i - 1);
    data_t v20 = in(x_i + 1, y_i - 1);
    data_t v30 = in(x_i + 2, y_i - 1);
    data_t v0 = cubic_interpolation(static_cast<interpolation_t>(x_i - 1), v00,
                                    static_cast<interpolation_t>(x_i + 0), v10,
                                    static_cast<interpolation_t>(x_i + 1), v20,
                                    static_cast<interpolation_t>(x_i + 2), v30, x);
    
    data_t v01 = in(x_i - 1, y_i);
    data_t v11 = in(x_i + 0, y_i);
    data_t v21 = in(x_i + 1, y_i);
    data_t v31 = in(x_i + 2, y_i);
    data_t v1 = cubic_interpolation(static_cast<interpolation_t>(x_i - 1), v01,
                                    static_cast<interpolation_t>(x_i + 0), v11,
                                    static_cast<interpolation_t>(x_i + 1), v21,
                                    static_cast<interpolation_t>(x_i + 2), v31, x);
    
    data_t v02 = in(x_i - 1, y_i + 1);
    data_t v12 = in(x_i + 0, y_i + 1);
    data_t v22 = in(x_i + 1, y_i + 1);
    data_t v32 = in(x_i + 2, y_i + 1);
    data_t v2 = cubic_interpolation(static_cast<interpolation_t>(x_i - 1), v02,
                                    static_cast<interpolation_t>(x_i + 0), v12,
                                    static_cast<interpolation_t>(x_i + 1), v22,
                                    static_cast<interpolation_t>(x_i + 2), v32, x);
    
    data_t v03 = in(x_i - 1, y_i + 2);
    data_t v13 = in(x_i + 0, y_i + 2);
    data_t v23 = in(x_i + 1, y_i + 2);
    data_t v33 = in(x_i + 2, y_i + 2);
    data_t v3 = cubic_interpolation(static_cast<interpolation_t>(x_i - 1), v03,
                                    static_cast<interpolation_t>(x_i + 0), v13,
                                    static_cast<interpolation_t>(x_i + 1), v23,
                                    static_cast<interpolation_t>(x_i + 2), v33, x);
    
    return cubic_interpolation(static_cast<interpolation_t>(y_i - 1), v0,
                               static_cast<interpolation_t>(y_i + 0), v1,
                               static_cast<interpolation_t>(y_i + 1), v2,
                               static_cast<interpolation_t>(y_i + 2), v3, y);
  }
}

#endif
