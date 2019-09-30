#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/signal_processing/ggo_dwt.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(dwt, haar)
{
  float data[] = {1, 2, 3, 8, 9, 5};
  float low[3];
  float high[3];

  ggo::fdwt(data, low, high, 6, ggo::DWT_HAAR);
  std::fill(std::begin(data), std::end(data), 0.f);
  ggo::idwt(low, high, data, 6, ggo::DWT_HAAR);

  GGO_CHECK_FLOAT_EQ(data[0], 1);
  GGO_CHECK_FLOAT_EQ(data[1], 2);
  GGO_CHECK_FLOAT_EQ(data[2], 3);
  GGO_CHECK_FLOAT_EQ(data[3], 8);
  GGO_CHECK_FLOAT_EQ(data[4], 9);
  GGO_CHECK_FLOAT_EQ(data[5], 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(dwt, daubechies4)
{
  double data[] = {1, 2, 3, 8, 9, 5, 5, 7};
  double low[4];
  double high[4];

  ggo::fdwt(data, low, high, 8, ggo::DWT_D4);
  std::fill(std::begin(data), std::end(data), 0.f);
  ggo::idwt(low, high, data, 8, ggo::DWT_D4);

  GGO_CHECK_FLOAT_EQ(data[0], 1);
  GGO_CHECK_FLOAT_EQ(data[1], 2);
  GGO_CHECK_FLOAT_EQ(data[2], 3);
  GGO_CHECK_FLOAT_EQ(data[3], 8);
  GGO_CHECK_FLOAT_EQ(data[4], 9);
  GGO_CHECK_FLOAT_EQ(data[5], 5);
  GGO_CHECK_FLOAT_EQ(data[6], 5);
  GGO_CHECK_FLOAT_EQ(data[7], 7);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(dwt, daubechies6)
{
  double data[] = {1, 2, 3, 3, 9, 5, 5, 7};
  double low[4];
  double high[4];

  ggo::fdwt(data, low, high, 8, ggo::DWT_D6);
  std::fill(std::begin(data), std::end(data), 0.f);
  ggo::idwt(low, high, data, 8, ggo::DWT_D6);

  GGO_CHECK_FLOAT_EQ(data[0], 1);
  GGO_CHECK_FLOAT_EQ(data[1], 2);
  GGO_CHECK_FLOAT_EQ(data[2], 3);
  GGO_CHECK_FLOAT_EQ(data[3], 3);
  GGO_CHECK_FLOAT_EQ(data[4], 9);
  GGO_CHECK_FLOAT_EQ(data[5], 5);
  GGO_CHECK_FLOAT_EQ(data[6], 5);
  GGO_CHECK_FLOAT_EQ(data[7], 7);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(dwt, daubechies8)
{
  float data[] = {1, 2, 3, 3, 9, 5};
  float low[3];
  float high[3];

  ggo::fdwt(data, low, high, 6, ggo::DWT_D8);
  std::fill(std::begin(data), std::end(data), 0.f);
  ggo::idwt(low, high, data, 6, ggo::DWT_D8);

  GGO_CHECK_FLOAT_EQ(data[0], 1);
  GGO_CHECK_FLOAT_EQ(data[1], 2);
  GGO_CHECK_FLOAT_EQ(data[2], 3);
  GGO_CHECK_FLOAT_EQ(data[3], 3);
  GGO_CHECK_FLOAT_EQ(data[4], 9);
  GGO_CHECK_FLOAT_EQ(data[5], 5);
}
