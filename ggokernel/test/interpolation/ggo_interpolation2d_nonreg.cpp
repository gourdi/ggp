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
  int stride = 2;

  // Interpolate directly on input points.
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 0, 0, stride) == 0);
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 1.f, 0.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 2.f, 0.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 0.f, 1.f, stride) == 1);
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 1.f, 1.f, stride) == 2);
  GGO_CHECK(ggo::bilinear_interpolation2d(data.data(), 3, 2, 2.f, 1.f, stride) == 3);
  
  // Check mirroring.
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, -1, -1, stride) == 0));
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 3, 2, stride) == 3));
  
  // Check interpolation between same values.
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, -0.5, -0.5, stride) == 0));
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, -3.5, -2.5, stride) == 3));
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 2.5, 1.5, stride) == 3));
  GGO_CHECK((ggo::bilinear_interpolation2d<uint8_t, uint8_t, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 5.5, 3.5, stride) == 0));
  
  // Check linear interpolation (ie. on integer x or y coordinate).
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 0.f, 0.25f, stride)), 0.25f);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 2.f, 0.75f, stride)), 2.75f);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, -1.f, -3.5f, stride)), 0.5f);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, -1.5f, -3.5f, stride)), 1.25f);
  GGO_CHECK_FABS((ggo::bilinear_interpolation2d<uint8_t, float, ggo::fetch_data_duplicated_edge_mirror2d_const<uint8_t>>(data.data(), 3, 2, 0.5f, 0.5f, stride)), 1.25f);
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
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 0, 0, stride) == 0));
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 1, 0, stride) == 2));
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 2, 0, stride) == 2));
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 0, 1, stride) == 1));
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 1, 1, stride) == 2));
  GGO_CHECK((ggo::bicubic_interpolation2d(data.data(), 3, 2, 2, 1, stride) == 3));
  
  // Check mirroring.
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, -1, -1, stride)), 0.f);
  GGO_CHECK_FABS((ggo::bicubic_interpolation2d<uint8_t, float>(data.data(), 3, 2, 3, 2, stride)), 3.f);
}
