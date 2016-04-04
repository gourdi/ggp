#ifndef __GGO_INTERPOLATION_2D__
#define __GGO_INTERPOLATION_2D__

#include <ggo_data_fetcher.h>
#include <ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  template <typename data_type, typename getter, typename interpolation_type>
  data_type bilinear_interpolation2d(const getter & in, interpolation_type x, interpolation_type y);

  template <typename data_type, typename getter, typename interpolation_type>
  data_type bicubic_interpolation2d(const getter & in, interpolation_type x, interpolation_type y);
}

/////////////////////////////////////////////////////////////////////
// Some usefull specializations.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename interpolation_type>
  inline data_type bilinear_interpolation2d_mirror(const data_type * input, int width, int height, interpolation_type x, interpolation_type y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride); };

    return bilinear_interpolation2d<data_type>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <>
  inline uint8_t bilinear_interpolation2d_mirror(const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride)); };

    return ggo::to<uint8_t>(bilinear_interpolation2d<float>(in, x, y));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename interpolation_type>
  inline data_type bicubic_interpolation2d_mirror(const data_type * input, int width, int height, interpolation_type x, interpolation_type y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride); };

    return bicubic_interpolation2d<data_type>(in, x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <>
  inline uint8_t bicubic_interpolation2d_mirror(const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride)); };

    return ggo::to<uint8_t>(bicubic_interpolation2d<float>(in, x, y));
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename getter, typename interpolation_type>
  data_type bilinear_interpolation2d(const getter & in, interpolation_type x, interpolation_type y)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    interpolation_type dx = x - x_i;
    interpolation_type dy = y - y_i;
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    data_type v00 = in(x_i, y_i);
    data_type v01 = in(x_i, y_i + 1);
    data_type v10 = in(x_i + 1, y_i);
    data_type v11 = in(x_i + 1, y_i + 1);
    
    data_type v1 = (1 - dy) * v00 + dy * v01;
    data_type v2 = (1 - dy) * v10 + dy * v11;

    return (1 - dx) * v1 + dx * v2;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename getter, typename interpolation_type>
  data_type bicubic_interpolation2d(const getter & in, interpolation_type x, interpolation_type y)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    data_type v00 = in(x_i - 1, y_i - 1);
    data_type v10 = in(x_i + 0, y_i - 1);
    data_type v20 = in(x_i + 1, y_i - 1);
    data_type v30 = in(x_i + 2, y_i - 1);
    data_type v0 = cubic_interpolation(static_cast<interpolation_type>(x_i - 1), v00,
                                       static_cast<interpolation_type>(x_i + 0), v10,
                                       static_cast<interpolation_type>(x_i + 1), v20,
                                       static_cast<interpolation_type>(x_i + 2), v30, x);
    
    data_type v01 = in(x_i - 1, y_i);
    data_type v11 = in(x_i + 0, y_i);
    data_type v21 = in(x_i + 1, y_i);
    data_type v31 = in(x_i + 2, y_i);
    data_type v1 = cubic_interpolation(static_cast<interpolation_type>(x_i - 1), v01,
                                       static_cast<interpolation_type>(x_i + 0), v11,
                                       static_cast<interpolation_type>(x_i + 1), v21,
                                       static_cast<interpolation_type>(x_i + 2), v31, x);
    
    data_type v02 = in(x_i - 1, y_i + 1);
    data_type v12 = in(x_i + 0, y_i + 1);
    data_type v22 = in(x_i + 1, y_i + 1);
    data_type v32 = in(x_i + 2, y_i + 1);
    data_type v2 = cubic_interpolation(static_cast<interpolation_type>(x_i - 1), v02,
                                       static_cast<interpolation_type>(x_i + 0), v12,
                                       static_cast<interpolation_type>(x_i + 1), v22,
                                       static_cast<interpolation_type>(x_i + 2), v32, x);
    
    data_type v03 = in(x_i - 1, y_i + 2);
    data_type v13 = in(x_i + 0, y_i + 2);
    data_type v23 = in(x_i + 1, y_i + 2);
    data_type v33 = in(x_i + 2, y_i + 2);
    data_type v3 = cubic_interpolation(static_cast<interpolation_type>(x_i - 1), v03,
                                       static_cast<interpolation_type>(x_i + 0), v13,
                                       static_cast<interpolation_type>(x_i + 1), v23,
                                       static_cast<interpolation_type>(x_i + 2), v33, x);
    
    return cubic_interpolation(static_cast<interpolation_type>(y_i - 1), v0,
                               static_cast<interpolation_type>(y_i + 0), v1,
                               static_cast<interpolation_type>(y_i + 1), v2,
                               static_cast<interpolation_type>(y_i + 2), v3, y);
  }
}

#endif
