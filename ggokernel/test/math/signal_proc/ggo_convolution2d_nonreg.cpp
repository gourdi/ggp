#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_convolution2d.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, horz)
{
  const float in[14] = {
    1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[14] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  ggo::apply_symetric_kernel_2d_horz<ggo::border_mode::mirror>(in, out, { 7, 2 }, kernel, 2);

  GGO_CHECK_FLOAT_EQ(out[0], 2.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[1], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[2], 0.f);
  GGO_CHECK_FLOAT_EQ(out[3], 0.f);
  GGO_CHECK_FLOAT_EQ(out[4], 0.f);
  GGO_CHECK_FLOAT_EQ(out[5], 0.f);
  GGO_CHECK_FLOAT_EQ(out[6], 0.f);

  GGO_CHECK_FLOAT_EQ(out[7], 0.f);
  GGO_CHECK_FLOAT_EQ(out[8], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[9], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[10], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[11], 0.f);
  GGO_CHECK_FLOAT_EQ(out[12], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[13], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, vert)
{
  const float in[12] = {
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };
  float out[12] = {
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };

  ggo::apply_symetric_kernel_2d_vert<ggo::border_mode::mirror>(in, out, { 4, 3 }, kernel, 2);

  GGO_CHECK_FLOAT_EQ(out[0], 0.f);
  GGO_CHECK_FLOAT_EQ(out[4], 0.f);
  GGO_CHECK_FLOAT_EQ(out[8], 0.f);

  GGO_CHECK_FLOAT_EQ(out[1], 2.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[5], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[9], 0.f / 3.f);

  GGO_CHECK_FLOAT_EQ(out[2], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[6], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[10], 1.f / 3.f);

  GGO_CHECK_FLOAT_EQ(out[3], 0.f);
  GGO_CHECK_FLOAT_EQ(out[7], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(out[11], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, full)
{
  std::vector<int> input = {
    11, 12, 13, 14, 15, 16,
    21, 22, 23, 24, 25, 26,
    31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46,
    51, 52, 53, 54, 55, 56 };

  const int kernel[] = { 1, 1 };

  ggo::apply_symetric_kernel_2d<ggo::border_mode::mirror>(input.data(), input.data(), { 6, 5 }, kernel, 2);

  const std::vector<int> expected = {
    132, 138, 147, 156, 165, 171,
    192, 198, 207, 216, 225, 231,
    282, 288, 297, 306, 315, 321,
    372, 378, 387, 396, 405, 411,
    432, 438, 447, 456, 465, 471 };

  GGO_CHECK_EQ(input, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, full_with_clipping)
{
  std::vector<int> input = {
    11, 12, 13, 14, 15, 16, 
    21, 22, 23, 24, 25, 26, 
    31, 32, 33, 34, 35, 36, 
    41, 42, 43, 44, 45, 46, 
    51, 52, 53, 54, 55, 56 };

  const int kernel[] = { 1, 1 };

  ggo::apply_symetric_kernel_2d<ggo::memory_lines_order::top_down, ggo::border_mode::mirror>(
    input.data(), input.data(), { 6, 5 }, ggo::rect_int::from_left_right_bottom_top(0, 2, 2, 3), kernel, 2);

  const std:: vector<int> expected = {
    11,  12,  13, 14, 15, 16,
    192, 198, 207, 24, 25, 26,
    282, 288, 297, 34, 35, 36,
    41,  42,  43, 44, 45, 46,
    51,  52,  53, 54, 55, 56 };

  GGO_CHECK_EQ(input, expected);
}
