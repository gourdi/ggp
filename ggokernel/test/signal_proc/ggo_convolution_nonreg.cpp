#include <ggo_nonreg.h>
#include <ggo_convolution.h>
#include <ggo_buffer_access.h>
#include <ggo_buffer_iterator.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, float_mirror)
{
  const float in[7] = { 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  ggo::const_buffer_iterator<sizeof(float), ggo::data_accessor<float>> input_it(in);
  ggo::buffer_iterator<sizeof(float), ggo::data_accessor<float>> output_it(out);

  auto left  = [&](int x) { return ggo::get1d_mirror(in, x, 7); };
  auto right = [&](int x) { return ggo::get1d_mirror(in, x, 7); };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[3], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[6], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, uint8_zero_borders_fixed_point)
{
  const uint8_t in[7] = { 0, 0, 255, 0, 0, 0, 255 };
  const uint16_t kernel[2] = { 128, 64 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  using accessor = ggo::fixed_point_data_accessor<uint8_t, uint16_t, 8>;

  ggo::const_buffer_iterator<1, accessor> input_it(in);
  ggo::buffer_iterator<1, accessor> output_it(out);

  auto left   = [&](int x) { return static_cast<uint16_t>(0); };
  auto right  = [&](int x) { return static_cast<uint16_t>(0); };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel, 2);

  GGO_CHECK(out[0] == 0);
  GGO_CHECK(out[1] == 64);
  GGO_CHECK(out[2] == 128);
  GGO_CHECK(out[3] == 64);
  GGO_CHECK(out[4] == 0);
  GGO_CHECK(out[5] == 64);
  GGO_CHECK(out[6] == 128);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, uint8_fixed_value_borders)
{
  const uint8_t in[7] = { 0, 0, 255, 0, 0, 0, 255 };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  using accessor = ggo::cast_data_accessor<uint8_t, float>;

  ggo::const_buffer_iterator<1, accessor> input_it(in);
  ggo::buffer_iterator<1, accessor> output_it(out);

  auto left   = [&](int x) { return 255.f; };
  auto right  = [&](int x) { return 255.f; };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel, 2);

  GGO_CHECK(out[0] == 85);
  GGO_CHECK(out[1] == 85);
  GGO_CHECK(out[2] == 85);
  GGO_CHECK(out[3] == 85);
  GGO_CHECK(out[4] == 0);
  GGO_CHECK(out[5] == 85);
  GGO_CHECK(out[6] == 170);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, float_horz)
{
  const float in[14] = {
    1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[14] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto left  = [&](int x, int y) { GGO_CHECK(x < 0);  GGO_CHECK(y >= 0 && y < 2); return 0.f; };
  auto right = [&](int x, int y) { GGO_CHECK(x >= 7); GGO_CHECK(y >= 0 && y < 2); return 0.f; };

  auto input_line_iterator  = [&](int y) { return ggo::const_buffer_iterator<sizeof(float), ggo::data_accessor<float>>(in + 7 * y); };
  auto output_line_iterator = [&](int y) { return ggo::buffer_iterator<sizeof(float), ggo::data_accessor<float>>(out + 7 * y); };

  ggo::apply_symetric_kernel_2d_horz(input_line_iterator, output_line_iterator, left, right, 7, 2,  kernel, 2);

  GGO_CHECK_FABS(out[0], 1.f / 3.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 0.f);
  GGO_CHECK_FABS(out[3], 0.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 0.f);
  GGO_CHECK_FABS(out[6], 0.f);

  GGO_CHECK_FABS(out[7], 0.f);
  GGO_CHECK_FABS(out[8], 1.f / 3.f);
  GGO_CHECK_FABS(out[9], 1.f / 3.f);
  GGO_CHECK_FABS(out[10], 1.f / 3.f);
  GGO_CHECK_FABS(out[11], 0.f);
  GGO_CHECK_FABS(out[12], 1.f / 3.f);
  GGO_CHECK_FABS(out[13], 1.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, float_vert)
{
  const float in[9] = {
    0.f, 0.f, 1.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 0.f };
  float out[9] = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };

  auto bottom = [&](int x, int y) { return ggo::get2d_mirror<float, ggo::y_down>(in, x, y, 3, 3, 3 * sizeof(float)); };
  auto top    = [&](int x, int y) { return ggo::get2d_mirror<float, ggo::y_down>(in, x, y, 3, 3, 3 * sizeof(float)); };

  const int offset = -3 * static_cast<int>(sizeof(float));

  auto input_column_iterator  = [&](int x) { return ggo::const_buffer_iterator<0, ggo::data_accessor<float>>(in + 6 + x, offset); };
  auto output_column_iterator = [&](int x) { return ggo::buffer_iterator<0, ggo::data_accessor<float>>(out + 6 + x, offset); };

  ggo::apply_symetric_kernel_2d_vert(input_column_iterator, output_column_iterator, bottom, top, 3, 3, kernel, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[3], 0.f);
  GGO_CHECK_FABS(out[6], 0.f);

  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 1.f / 3.f);
  GGO_CHECK_FABS(out[7], 1.f / 3.f);

  GGO_CHECK_FABS(out[2], 2.f / 3.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[8], 0.f);
}

