#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>
#include <kernel/math/interpolation/ggo_interpolation2d.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation2d, bilinear)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  const std::array<uint8_t, 12> data {{0, 2, 2, 1, 2, 3}};
  
  // Interpolate directly on input points.
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 0.f, 0.f)), 0);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 1.f, 0.f)), 2);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 2.f, 0.f)), 2);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 0.f, 1.f)), 1);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 1.f, 1.f)), 2);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 2.f, 1.f)), 3);
  
  // Check mirroring.
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, -1, -1)), 0);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 3, 2)), 3);

  // Check interpolation between same values.
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, -0.5f, -0.5f)), 0);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, -3.5f, -2.5f)), 3);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 2.5f, 1.5f)), 3);
  GGO_CHECK_EQ((ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 5.5f, 3.5f)), 0);
  
  // Check linear interpolation (ie. on integer x or y coordinate).
  auto float_interpolation = [](const uint8_t * input, int width, int height, float x, float y)
  {
    auto get2d = [&](int x, int y) { return static_cast<float>(input[y * width + x]); };
    auto in = [&](int x, int y) { return ggo::get2d_mirror(get2d, x, y, width, height); };

    return ggo::linear_interpolation2d<float>(in, x, y);
  };
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, 0.f, 0.25f), 0.25f);
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, 2.f, 0.75f), 2.75f);
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, -1.f, -3.5f), 0.5f);
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, -1.5f, -3.5f), 1.25f);
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, 0.5f, 0.5f), 1.25f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(interpolation2d, bicubic)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  std::array<uint8_t, 12> data {{0, 2, 2, 1, 2, 3}};

  // Interpolate directly on input points.
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 0, 0)) == 0);
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 1, 0)) == 2);
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 2, 0)) == 2);
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 0, 1)) == 1);
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 1, 1)) == 2);
  GGO_CHECK((ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, 3, 2, 1)) == 3);
  
  // Check mirroring.
  auto float_interpolation = [](const uint8_t * input, int width, int height, float x, float y)
  {
    auto get2d = [&](int x, int y) { return static_cast<float>(input[y * width + x]); };
    auto in = [&](int x, int y) { return ggo::get2d_mirror(get2d, x, y, width, height); };

    return ggo::cubic_interpolation2d<float>(in, x, y);
  };

  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, -1, -1), 0.f);
  GGO_CHECK_FLOAT_EQ(float_interpolation(data.data(), 3, 2, 3, 2), 3.f);
}
