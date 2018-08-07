#include <array>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <2d/io/ggo_bmp.h>

#if 0

namespace
{
  const int SIZE = 800;

  ////////////////////////////////////////////////////////////////////
  uint8_t bilinear_interpolation2d_zero(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_zero(input, x, y, width, height)); };

    return ggo::to<uint8_t>(ggo::bilinear_interpolation2d<float>(in, x, y));
  }

  ////////////////////////////////////////////////////////////////////
  uint8_t bicubic_interpolation2d_zero(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_zero(input, x, y, width, height)); };

    return ggo::to<uint8_t>(ggo::bicubic_interpolation2d<float>(in, x, y));
  }

  ////////////////////////////////////////////////////////////////////
  template <uint8_t(interpolate_func)(const uint8_t *, int, int, float, float)>
  void test_interpolation(const std::string & filename)
  {
    ggo::rgb_image_buffer_uint8 image(SIZE, SIZE);
    
    const std::array<uint8_t, 9> input {{
        50, 25, 185,
        200, 132, 98,
        43, 77, 210}};

    uint8_t * ptr = image.data();
    
    for (int y = 0; y < SIZE; ++y)
    {
      for (int x = 0; x < SIZE; ++x)
      {
        float x_f = 0.025f * x - 4;
        float y_f = 0.025f * y - 4;
        
        uint8_t v = interpolate_func(input.data(), 3, 3, x_f, y_f);
        *ptr++ = v;
        *ptr++ = v;
        *ptr++ = v;
      }
    }
    
    ggo::save_bmp(filename, image.data(), SIZE, SIZE);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation, test)
{
  test_interpolation<bilinear_interpolation2d_zero>("test_bilinear_zero.bmp");
  test_interpolation<ggo::bilinear_interpolation2d_mirror>("test_bilinear_mirror.bmp");
  test_interpolation<bicubic_interpolation2d_zero>("test_bicubic_zero.bmp");
  test_

#endif