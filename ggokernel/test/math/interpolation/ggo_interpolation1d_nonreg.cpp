#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, linear)
{
  const float data[] = { 1, 2, 7, 5 };

  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 0.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 1.0f), 2.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 2.0f), 7.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 3.0f), 5.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 0.5f), 1.5f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 2.5f), 6.0f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, cubic)
{
  const float data[] = { 1, 2, 7, 5 };

  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 0.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 1.0f), 2.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 2.0f), 7.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 3.0f), 5.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 0.5f), 1.1875f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 2.5f), 6.3125f);
}
