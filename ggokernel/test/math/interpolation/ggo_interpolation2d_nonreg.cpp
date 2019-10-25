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
  
  const std::array<float, 12> data {{0.f, 2.f, 2.f, 1.f, 2.f, 3.f}};
  
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  0.5f,  0.50f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  1.5f,  0.50f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  0.50f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  0.5f,  1.50f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  1.5f,  1.50f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  1.50f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -0.5f, -0.50f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  3.0f,  2.00f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -0.5f, -0.50f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -3.5f, -2.50f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  1.50f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  5.5f,  3.50f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  0.5f,  0.75f), 0.25f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  1.25f), 2.75f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -0.5f, -3.00f), 0.50f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -1.0f, -3.00f), 1.25f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  1.0f,  1.00f), 1.25f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(interpolation2d, bicubic)
{
  // ^
  // |
  // 1 2 3
  // 0 2 2 --->
  
  const std::array<float, 12> data {{0.f, 2.f, 2.f, 1.f, 2.f, 3.f}};

  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  0.5f,  0.5f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  1.5f,  0.5f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  0.5f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  0.5f,  1.5f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  1.5f,  1.5f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  2.5f,  1.5f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2, -0.5f, -0.5f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation2d_mirror<ggo::lines_order::up>(data.data(), 3, 2,  3.5f,  2.5f), 3.f);
}
