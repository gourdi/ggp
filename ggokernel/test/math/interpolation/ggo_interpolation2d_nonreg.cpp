#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_borders.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <kernel/memory/ggo_memory_layouts.h>
#include <array>

namespace
{
  /////////////////////////////////////////////////////////////////////
  float linear_interpolation2d(const float* input, int width, int height, float x, float y)
  {
    ggo::bottom_up_memory_layout<sizeof(float)> memory_layout({ width, height }, width * sizeof(float));

    auto in = [&](int x, int y)
    {
      x = ggo::mirror_index(x, width);
      y = ggo::mirror_index(y, height);

      const void* ptr = memory_layout.at(static_cast<const void*>(input), x, y);
      return *static_cast<const float*>(ptr);
    };

    return ggo::linear_interpolation2d(in, x, y);
  }


  /////////////////////////////////////////////////////////////////////
  float cubic_interpolation2d(const float * input, int width, int height, float x, float y)
  {
    ggo::bottom_up_memory_layout<sizeof(float)> memory_layout({ width, height }, width * sizeof(float));

    auto in = [&](int x, int y)
    {
      x = ggo::mirror_index(x, width);
      y = ggo::mirror_index(y, height);

      const void* ptr = memory_layout.at(static_cast<const void*>(input), x, y);
      return *static_cast<const float*>(ptr);
    };

    return ggo::cubic_interpolation2d(in, x, y);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation2d, bilinear)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  const std::array<float, 6> data {{0.f, 2.f, 2.f, 1.f, 2.f, 3.f}};
  
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.0f,  0.0f), 0.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  1.0f,  0.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  2.0f,  0.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.0f,  1.0f), 1.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  1.0f,  1.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  2.0f,  1.0f), 3.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2, -1.0f, -1.0f), 0.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  3.0f, -1.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  3.0f,  2.0f), 3.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2, -3.5f, -2.5f), 3.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  2.5f,  1.5f), 3.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  5.5f,  3.5f), 0.f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.0f,  0.5f), 0.5f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.5f,  0.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.5f, -1.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  0.5f, -0.5f), 1.0f);
  GGO_CHECK_FLOAT_EQ(linear_interpolation2d(data.data(), 3, 2,  1.5f,  0.5f), 2.25f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(interpolation2d, bicubic)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  const std::array<float, 6> data {{0.f, 2.f, 2.f, 1.f, 2.f, 3.f}};

  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  0.0f,  0.0f), 0.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  1.0f,  0.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  2.0f,  0.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  0.0f,  1.0f), 1.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  1.0f,  1.0f), 2.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  2.0f,  1.0f), 3.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2, -1.0f, -1.0f), 0.f);
  GGO_CHECK_FLOAT_EQ(cubic_interpolation2d(data.data(), 3, 2,  3.0f,  2.0f), 3.f);
}
