#include <ggo_nonreg.h>
#include <array>
#include <ggo_interpolation1d.h>
#include <ggo_interpolation2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation2d, bilinear)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  std::array<uint8_t, 12> data {{0, 0, 2, 0, 2, 0, 1, 0, 2, 0, 3, 0}};
  int stride = 2;

  // Interpolate directly on input points.
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 0.f, 0.f, stride) == 0);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 1.f, 0.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 2.f, 0.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 0.f, 1.f, stride) == 1);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 1.f, 1.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 2.f, 1.f, stride) == 3);
  
  // Check mirroring.
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, -1, -1, stride) == 0);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 3, 2, stride) == 3);
  
  // Check interpolation between same values.
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, -0.5f, -0.5f, stride) == 0);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, -3.5f, -2.5f, stride) == 3);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 2.5f, 1.5f, stride) == 3);
  GGO_CHECK(ggo::bilinear_interpolation2d_mirror(data.data(), 3, 2, 5.5f, 3.5f, stride) == 0);
  
  // Check linear interpolation (ie. on integer x or y coordinate).
  auto float_interpolation = [](const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::to<float>(ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride)); };

    return ggo::bilinear_interpolation2d<float>(in, x, y);
  };
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, 0.f, 0.25f, stride), 0.25f);
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, 2.f, 0.75f, stride), 2.75f);
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, -1.f, -3.5f, stride), 0.5f);
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, -1.5f, -3.5f, stride), 1.25f);
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, 0.5f, 0.5f, stride), 1.25f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(interpolation2d, bicubic)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  std::array<uint8_t, 12> data {{0, 0, 2, 0, 2, 0, 1, 0, 2, 0, 3, 0}};
  const int stride = 2;

  // Interpolate directly on input points.
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 0, 0, stride) == 0);
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 1, 0, stride) == 2);
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 2, 0, stride) == 2);
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 0, 1, stride) == 1);
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 1, 1, stride) == 2);
  GGO_CHECK(ggo::bicubic_interpolation2d_mirror(data.data(), 3, 2, 2, 1, stride) == 3);
  
  // Check mirroring.
  auto float_interpolation = [](const uint8_t * input, int width, int height, float x, float y, int stride)
  {
    auto in = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(input, x, y, width, height, stride); };

    return ggo::bicubic_interpolation2d<float>(in, x, y);
  };

  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, -1, -1, stride), 0.f);
  GGO_CHECK_FABS(float_interpolation(data.data(), 3, 2, 3, 2, stride), 3.f);
}
