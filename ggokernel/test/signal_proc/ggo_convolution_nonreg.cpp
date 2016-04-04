#include <ggo_nonreg.h>
#include <ggo_convolution.h>
#include <ggo_data_fetcher.h>
#include <array>

#include <stdio.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, uint8_fixed_value)
{
  const uint8_t in[7] = { 0, 0, 255, 0, 0, 0, 255 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };
  const float filter[2] = { 1.f / 3.f, 1.f / 3.f };

  auto input = [&](int x, int width) { return ggo::to<float>(ggo::get1d_fixed_value<uint8_t>(in, x, width, 255)); };
  auto output = [&](int x, int width, float v) { ggo::set1d_standard(out, x, width, ggo::to<uint8_t>(v)); };

  ggo::apply_symetric_filter_1d(input, output, 7, filter, 2);

  GGO_CHECK(out[0] == 85);
  GGO_CHECK(out[1] == 85);
  GGO_CHECK(out[2] == 85);
  GGO_CHECK(out[3] == 85);
  GGO_CHECK(out[4] == 0);
  GGO_CHECK(out[5] == 85);
  GGO_CHECK(out[6] == 170);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, float_mirror)
{
  const float in[7] = { 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
  const float filter[2] = { 1.f / 3.f, 1.f / 3.f };

  auto input = [&](int x, int width) { return ggo::get1d_duplicated_edge_mirror(in, x, width); };
  auto output = [&](int x, int width, float v) { ggo::set1d_standard(out, x, width, v); };

  ggo::apply_symetric_filter_1d(input, output, 7, filter, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[3], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[6], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, float_mirror_horz)
{
  const float in[14] = { 
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
  float out[14] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
  const float filter[2] = { 1.f / 3.f, 1.f / 3.f };

  auto input = [&](int x, int y, int width, int height) { return ggo::get2d_duplicated_edge_mirror(in, x, y, width, height); };
  auto output = [&](int x, int y, int width, int height, float v) { ggo::set2d_standard(out, x, y, width, height, v); };

  ggo::apply_symetric_filter_2d_horz(input, output, 7, 2, filter, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[3], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[6], 2.f / 3.f);

  GGO_CHECK_FABS(out[7], 0.f);
  GGO_CHECK_FABS(out[8], 0.f);
  GGO_CHECK_FABS(out[9], 0.f);
  GGO_CHECK_FABS(out[10], 0.f);
  GGO_CHECK_FABS(out[11], 0.f);
  GGO_CHECK_FABS(out[12], 0.f);
  GGO_CHECK_FABS(out[13], 0.f);
}


/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, float_mirror_vert)
{
  const float in[9] = {
    0.f, 0.f, 1.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 0.f };
  float out[9] = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f };
  const float filter[2] = { 1.f / 3.f, 1.f / 3.f };

  auto input = [&](int x, int y, int width, int height) { return ggo::get2d_fixed_value(in, x, y, width, height, 0.f); };
  auto output = [&](int x, int y, int width, int height, float v) { ggo::set2d_standard(out, x, y, width, height, v); };

  ggo::apply_symetric_filter_2d_vert(input, output, 3, 3, filter, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[3], 0.f);
  GGO_CHECK_FABS(out[6], 0.f);

  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 1.f / 3.f);
  GGO_CHECK_FABS(out[7], 1.f / 3.f);

  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[8], 0.f);
}

