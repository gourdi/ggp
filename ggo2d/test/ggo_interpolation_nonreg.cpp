#include <array>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <2d/io/ggo_bmp.h>

namespace
{
  ////////////////////////////////////////////////////////////////////
  uint8_t linear_interpolation2d(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_zero<ggo::lines_order::up>(input, x, y, width, height, width)); };

    return ggo::round_to<uint8_t>(ggo::linear_interpolation2d<float>(in, x, y));
  }

  ////////////////////////////////////////////////////////////////////
  uint8_t cubic_interpolation2d(const uint8_t * input, int width, int height, float x, float y)
  {
    auto in = [&](int x, int y) { return static_cast<float>(ggo::get2d_zero<ggo::lines_order::up>(input, x, y, width, height, width)); };

    return ggo::round_to<uint8_t>(ggo::clamp(ggo::cubic_interpolation2d<float>(in, x, y), 0.f, 255.f));
  }

  ////////////////////////////////////////////////////////////////////
  template <uint8_t(interpolate_func)(const uint8_t *, int, int, float, float)>
  void test_interpolation(const std::string & filename)
  {
    constexpr int size = 100;

    ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::down> image({ size, size });
    
    const std::array<uint8_t, 9> input{ {
        50, 25, 185,
        200, 132, 98,
        43, 77, 210} };

    uint8_t v = interpolate_func(input.data(), 3, 3, 0.25f, 0.25f);
    
    for (int y = 0; y < size; ++y)
    {
      for (int x = 0; x < size; ++x)
      {
        float x_f = 0.05f * x - 1;
        float y_f = 0.05f * y - 1;
        
        uint8_t v = interpolate_func(input.data(), 3, 3, x_f, y_f);

        image.write_pixel(x, y, v);
      }
    }
    
    ggo::save_bmp(filename, image);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation2d, test)
{
  test_interpolation<linear_interpolation2d>("interpolate_bilinear.bmp");
  test_interpolation<cubic_interpolation2d>("interpolate_bicubic.bmp");
}
