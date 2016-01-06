#include <ggo_nonreg.h>
#include <array>
#include <ggo_interpolation2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation2d, bilinear)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  std::array<uint8_t, 12> data {{0, 0, 2, 0, 2, 0, 1, 0, 2, 0, 3, 0}};
  const int stride = 2;

  // Interpolate directly on input points.
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0, 0, stride)), 0);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 1, 0, stride)), 2);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2, 0, stride)), 2);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0, 1, stride)), 1);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 1, 1, stride)), 2);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2, 1, stride)), 3);
  
  // Check mirroring.
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, -1, -1, stride)), 0);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 3, 2, stride)), 3);
  
  // Check interpolation between same values.
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, -0.5, -0.5, stride)), 0);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, -3.5, -2.5, stride)), 3);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2.5, 1.5, stride)), 3);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 5.5, 3.5, stride)), 0);
  
  // Check linear interpolation (ie. on integer x or y coordinate).
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0.f, 0.25f, stride)),0.25);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2.f, 0.75f, stride)), 2.75);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, -1.f, -3.5f, stride)), 0.5);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, -1.5f, -3.5f, stride)), 1.25);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0.5f, 0.5f, stride)), 1.25);
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
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0, 0, stride)), 0);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 1, 0, stride)), 2);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2, 0, stride)), 2);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 0, 1, stride)), 1);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 1, 1, stride)), 2);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 2, 1, stride)), 3);
  
  // Check mirroring.
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, -1, -1, stride)), 0);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 3, 2, stride)), 3);
}
