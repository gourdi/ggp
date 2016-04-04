#include <array>
#include <ggo_nonreg.h>
#include <ggo_interpolation2d.h>
#include <ggo_rgb_image_data.h>
#include <ggo_bmp.h>

namespace
{
  const int SIZE = 800;

  ////////////////////////////////////////////////////////////////////
  uint8_t bilinear_interpolation2d_zero(const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_fixed_value(input, x, y, width, height, uint8_t(0), stride)); };

    return ggo::to<uint8_t>(ggo::bilinear_interpolation2d<float>(in, x, y));
  }

  ////////////////////////////////////////////////////////////////////
  uint8_t bicubic_interpolation2d_zero(const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_fixed_value(input, x, y, width, height, uint8_t(0), stride)); };

    return ggo::to<uint8_t>(ggo::bicubic_interpolation2d<float>(in, x, y));
  }

  ////////////////////////////////////////////////////////////////////
  template <uint8_t(interpolate_func)(const uint8_t *, int, int, float, float, int)>
  void test_interpolation(const std::string & filename)
  {
    ggo::rgb_image_data_uint8 image_data(SIZE, SIZE);
    
    const std::array<uint8_t, 9> input {{
        50, 25, 185,
        200, 132, 98,
        43, 77, 210}};

    uint8_t * ptr = image_data.get_buffer();
    
    for (int y = 0; y < SIZE; ++y)
    {
      for (int x = 0; x < SIZE; ++x)
      {
        float x_f = 0.025f * x - 4;
        float y_f = 0.025f * y - 4;
        
        uint8_t v = interpolate_func(input.data(), 3, 3, x_f, y_f, 1);
        *ptr++ = v;
        *ptr++ = v;
        *ptr++ = v;
      }
    }
    
    ggo::save_bmp(filename, image_data.get_buffer(), SIZE, SIZE);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation, test)
{
  test_interpolation<bilinear_interpolation2d_zero>("test_bilinear_zero.bmp");
  test_interpolation<ggo::bilinear_interpolation2d_mirror>("test_bilinear_mirror.bmp");
  test_interpolation<bicubic_interpolation2d_zero>("test_bicubic_zero.bmp");
  test_interpolation<ggo::bicubic_interpolation2d_mirror>("test_bicubic_mirror.bmp");
}