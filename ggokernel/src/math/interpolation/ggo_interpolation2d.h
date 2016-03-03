#ifndef __GGO_INTERPOLATION_2D__
#define __GGO_INTERPOLATION_2D__

#include <ggo_data_fetcher.h>
#include <ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  template <typename input_type,
            typename interpol_horz_type = float,
            typename interpol_vert_type = input_type,
            typename output_type = input_type,
            input_type(fetch_func)(const input_type *, int, int, int, int, int) = ggo::fetch_data_standard2d_const<input_type>>
  output_type bilinear_interpolation2d(const input_type * input,
                                       int width,
                                       int height,
                                       interpol_horz_type x,
                                       interpol_horz_type y,
                                       int stride = 1);

  template <typename input_type,
            typename interpol_horz_type = float,
            typename interpol_vert_type = input_type,
            typename output_type = input_type,
            input_type(fetch_func)(const input_type *, int, int, int, int, int) = ggo::fetch_data_duplicated_edge_mirror2d_const<input_type>>
  output_type bicubic_interpolation2d(const input_type * input,
                                      int width,
                                      int height,
                                      interpol_horz_type x,
                                      interpol_horz_type y,
                                      int stride);
}

/////////////////////////////////////////////////////////////////////
// Some usefull shortcuts.
namespace ggo
{
  uint8_t bilinear_interpolation2d_uint8(const uint8_t * input, int width, int height, float x, float y, int stride = 1)
  {
    return bilinear_interpolation2d<uint8_t, float, float>(input, width, height, x, y, stride);
  }

  uint8_t bilinear_interpolation2d_uint8_mirror(const uint8_t * input, int width, int height, float x, float y, int stride = 1)
  {
    return bilinear_interpolation2d<uint8_t, float, float, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(input, width, height, x, y, stride);
  }

  uint8_t bicubic_interpolation2d_uint8(const uint8_t * input, int width, int height, float x, float y, int stride = 1)
  {
    return bicubic_interpolation2d<uint8_t, float, float>(input, width, height, x, y, stride);
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename input_type,
            typename interpol_horz_type,
            typename interpol_vert_type,
            typename output_type,
            input_type(fetch_func)(const input_type *, int, int, int, int, int)>
  output_type bilinear_interpolation2d(const input_type * input,
                                       int width,
                                       int height,
                                       interpol_horz_type x,
                                       interpol_horz_type y,
                                       int stride)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    interpol_horz_type dx = x - x_i;
    interpol_horz_type dy = y - y_i;
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    interpol_vert_type v00 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i, y_i));
    interpol_vert_type v01 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i, y_i + 1));
    interpol_vert_type v10 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i));
    interpol_vert_type v11 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i + 1));
    
    interpol_vert_type v1 = (1 - dy) * v00 + dy * v01;
    interpol_vert_type v2 = (1 - dy) * v10 + dy * v11;

    return ggo::to<output_type>((1 - dx) * v1 + dx * v2);
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename input_type,
            typename interpol_horz_type,
            typename interpol_vert_type,
            typename output_type,
            input_type(fetch_func)(const input_type *, int, int, int, int, int)>
  output_type bicubic_interpolation2d(const input_type * input,
                                      int width,
                                      int height,
                                      interpol_horz_type x,
                                      interpol_horz_type y,
                                      int stride)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    interpol_vert_type v00 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i - 1, y_i - 1));
    interpol_vert_type v10 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 0, y_i - 1));
    interpol_vert_type v20 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i - 1));
    interpol_vert_type v30 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 2, y_i - 1));
    interpol_vert_type v0 = cubic_interpolation(static_cast<interpol_horz_type>(x_i - 1), v00,
                                                static_cast<interpol_horz_type>(x_i + 0), v10,
                                                static_cast<interpol_horz_type>(x_i + 1), v20,
                                                static_cast<interpol_horz_type>(x_i + 2), v30, x);
    
    interpol_vert_type v01 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i - 1, y_i));
    interpol_vert_type v11 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 0, y_i));
    interpol_vert_type v21 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i));
    interpol_vert_type v31 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 2, y_i));
    interpol_vert_type v1 = cubic_interpolation(static_cast<interpol_horz_type>(x_i - 1), v01,
                                                static_cast<interpol_horz_type>(x_i + 0), v11,
                                                static_cast<interpol_horz_type>(x_i + 1), v21,
                                                static_cast<interpol_horz_type>(x_i + 2), v31, x);
    
    interpol_vert_type v02 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i - 1, y_i + 1));
    interpol_vert_type v12 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 0, y_i + 1));
    interpol_vert_type v22 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i + 1));
    interpol_vert_type v32 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 2, y_i + 1));
    interpol_vert_type v2 = cubic_interpolation(static_cast<interpol_horz_type>(x_i - 1), v02,
                                                static_cast<interpol_horz_type>(x_i + 0), v12,
                                                static_cast<interpol_horz_type>(x_i + 1), v22,
                                                static_cast<interpol_horz_type>(x_i + 2), v32, x);
    
    interpol_vert_type v03 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i - 1, y_i + 2));
    interpol_vert_type v13 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 0, y_i + 2));
    interpol_vert_type v23 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 1, y_i + 2));
    interpol_vert_type v33 = ggo::to<interpol_vert_type>(fetch_func(input, width, height, stride, x_i + 2, y_i + 2));
    interpol_vert_type v3 = cubic_interpolation(static_cast<interpol_horz_type>(x_i - 1), v03,
                                                static_cast<interpol_horz_type>(x_i + 0), v13,
                                                static_cast<interpol_horz_type>(x_i + 1), v23,
                                                static_cast<interpol_horz_type>(x_i + 2), v33, x);
    
    return ggo::to<output_type>(cubic_interpolation(static_cast<interpol_horz_type>(y_i - 1), v0,
                                                    static_cast<interpol_horz_type>(y_i + 0), v1,
                                                    static_cast<interpol_horz_type>(y_i + 1), v2,
                                                    static_cast<interpol_horz_type>(y_i + 2), v3, y));
  }
}


#endif
