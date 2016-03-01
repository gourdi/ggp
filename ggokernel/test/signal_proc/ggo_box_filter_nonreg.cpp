#include <ggo_nonreg.h>
#include <ggo_box_filter.h>
#include <array>

#include <stdio.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(box_filter, box_filter3_uint8)
{
  std::array<uint8_t, 7> data {1, 1, 2, 2, 255, 255, 255};
  
  ggo::box_filter3_1d(data.data(), 7);

  GGO_CHECK(data[0] == 1);
  GGO_CHECK(data[1] == 1);
  GGO_CHECK(data[2] == 2);
  GGO_CHECK(data[3] == 86);
  GGO_CHECK(data[4] == 171);
  GGO_CHECK(data[5] == 255);
  GGO_CHECK(data[6] == 255);
}
  
/////////////////////////////////////////////////////////////////////
GGO_TEST(box_filter, box_filter3)
{
  std::array<float, 9> data {1, 0, 5, 0, 2, 0, 7, 0, 4};
  
  ggo::box_filter3_1d(data.data(), 5, 2);
  
  GGO_CHECK_FABS(data[0], 11.f / 3);
  GGO_CHECK_FABS(data[1], 0);
  GGO_CHECK_FABS(data[2], 8.f / 3);
  GGO_CHECK_FABS(data[3], 0);
  GGO_CHECK_FABS(data[4], 14.f / 3);
  GGO_CHECK_FABS(data[5], 0);
  GGO_CHECK_FABS(data[6], 13.f / 3);
  GGO_CHECK_FABS(data[7], 0);
  GGO_CHECK_FABS(data[8], 18.f / 3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(box_filter, box_filter5)
{
  std::array<float, 11> data {1, 0, 5, 0, 2, 0, 7, 0, 4, 0, 5};
  
  ggo::box_filter5_1d(data.data(), 6, 2);
  
  GGO_CHECK_FABS(data[0], (2 + 5 + 1 + 5 + 2) / 5.f);
  GGO_CHECK_FABS(data[1], 0);
  GGO_CHECK_FABS(data[2], (5 + 1 + 5 + 2 + 7) / 5.f);
  GGO_CHECK_FABS(data[3], 0);
  GGO_CHECK_FABS(data[4], (1 + 5 + 2 + 7 + 4) / 5.f);
  GGO_CHECK_FABS(data[5], 0);
  GGO_CHECK_FABS(data[6], (5 + 2 + 7 + 4 + 5) / 5.f);
  GGO_CHECK_FABS(data[7], 0);
  GGO_CHECK_FABS(data[8], (2 + 7 + 4 + 5 + 4) / 5.f);
  GGO_CHECK_FABS(data[9], 0);
  GGO_CHECK_FABS(data[10], (7 + 4 + 5 + 4 + 7) / 5.f);
}
  