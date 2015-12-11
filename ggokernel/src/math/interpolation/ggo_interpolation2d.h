#ifndef __GGO_INTERPOLATION_2D__
#define __GGO_INTERPOLATION_2D__

#include <ggo_data_fetcher.h>
#include <ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
// Definition.
namespace ggo
{
  template <typename INPUT_T, typename OUTPUT_T>
  OUTPUT_T bilinear_interpolation2d(const INPUT_T * input,
                                    int width,
                                    int height,
                                    float x,
                                    float y,  
                                    int stride = 1,
                                    const ggo::data_fetcher2d_abc<INPUT_T> & data_fetcher = duplicated_edge_mirror_data_fetcher2d<INPUT_T>());
  
  template <typename INPUT_T, typename OUTPUT_T>
  OUTPUT_T bicubic_interpolation2d(const INPUT_T * input,
                                   int width,
                                   int height,
                                   float x,
                                   float y,
                                   int stride = 1,
                                   const ggo::data_fetcher2d_abc<INPUT_T> & data_fetcher = duplicated_edge_mirror_data_fetcher2d<INPUT_T>());
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename INPUT_T, typename OUTPUT_T>
  OUTPUT_T bilinear_interpolation2d(const INPUT_T * input,
                                    int width,
                                    int height,
                                    float x,
                                    float y,
                                    int stride,
                                    const ggo::data_fetcher2d_abc<INPUT_T> & data_fetcher)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    float dx = x - x_i;
    float dy = y - y_i;
    GGO_ASSERT_GE(dx, 0);
    GGO_ASSERT_LE(dx, 1);
    GGO_ASSERT_GE(dy, 0);
    GGO_ASSERT_LE(dy, 1);
    
    OUTPUT_T v00 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i, y_i));
    OUTPUT_T v01 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i, y_i + 1));
    OUTPUT_T v10 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i));
    OUTPUT_T v11 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i + 1));
    
    OUTPUT_T v1 = (1.f - dy) * v00 + dy * v01;
    OUTPUT_T v2 = (1.f - dy) * v10 + dy * v11;

    return (1.f - dx) * v1 + dx * v2;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename INPUT_T, typename OUTPUT_T>
  OUTPUT_T bicubic_interpolation2d(const INPUT_T * input,
                                   int width,
                                   int height,
                                   float x,
                                   float y,
                                   int stride,
                                   const ggo::data_fetcher2d_abc<INPUT_T> & data_fetcher)
  {
    // The integer coordinate of the lower left value.
    int x_i = x >= 0 ? static_cast<int>(x) : static_cast<int>(x - 1);
    int y_i = y >= 0 ? static_cast<int>(y) : static_cast<int>(y - 1);
    GGO_ASSERT_LE(x_i, x);
    GGO_ASSERT_LE(x, x_i + 1);
    GGO_ASSERT_LE(y_i, y);
    GGO_ASSERT_LE(y, y_i + 1);

    // Interpolate.
    OUTPUT_T v00 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i - 1, y_i - 1));
    OUTPUT_T v10 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 0, y_i - 1));
    OUTPUT_T v20 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i - 1));
    OUTPUT_T v30 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 2, y_i - 1));
    OUTPUT_T v0 = cubic_interpolation(static_cast<float>(x_i - 1), v00,
                                      static_cast<float>(x_i + 0), v10,
                                      static_cast<float>(x_i + 1), v20,
                                      static_cast<float>(x_i + 2), v30, x);
    
    OUTPUT_T v01 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i - 1, y_i));
    OUTPUT_T v11 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 0, y_i));
    OUTPUT_T v21 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i));
    OUTPUT_T v31 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 2, y_i));
    OUTPUT_T v1 = cubic_interpolation(static_cast<float>(x_i - 1), v01,
                                      static_cast<float>(x_i + 0), v11,
                                      static_cast<float>(x_i + 1), v21,
                                      static_cast<float>(x_i + 2), v31, x);
    
    OUTPUT_T v02 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i - 1, y_i + 1));
    OUTPUT_T v12 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 0, y_i + 1));
    OUTPUT_T v22 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i + 1));
    OUTPUT_T v32 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 2, y_i + 1));
    OUTPUT_T v2 = cubic_interpolation(static_cast<float>(x_i - 1), v02,
                                      static_cast<float>(x_i + 0), v12,
                                      static_cast<float>(x_i + 1), v22,
                                      static_cast<float>(x_i + 2), v32, x);
    
    OUTPUT_T v03 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i - 1, y_i + 2));
    OUTPUT_T v13 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 0, y_i + 2));
    OUTPUT_T v23 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 1, y_i + 2));
    OUTPUT_T v33 = static_cast<OUTPUT_T>(data_fetcher.fetch(input, width, height, stride, x_i + 2, y_i + 2));
    OUTPUT_T v3 = cubic_interpolation(static_cast<float>(x_i - 1), v03,
                                      static_cast<float>(x_i + 0), v13,
                                      static_cast<float>(x_i + 1), v23,
                                      static_cast<float>(x_i + 2), v33, x);
    
    return cubic_interpolation(static_cast<float>(y_i - 1), v0,
                               static_cast<float>(y_i + 0), v1,
                               static_cast<float>(y_i + 1), v2,
                               static_cast<float>(y_i + 2), v3, y);
  }
}

#endif
