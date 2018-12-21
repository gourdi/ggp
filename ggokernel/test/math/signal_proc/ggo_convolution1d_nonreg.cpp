#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_convolution1d.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, kernel_larger_than_data)
{
  const int in[4] = { 1, 6, 2, 3 };
  const int kernel[3] = { 1, 1, 1 };
  int out[4] = { 0, 0, 0, 0 };

  auto read = [&](int i) { return in[i]; };
  auto write = [&](int i, int v) { out[i] = v; };

  ggo::symmetric_convolution<ggo::border_mode::duplicate_edge>(read, write, 4, 0, 3, kernel, 3);
  GGO_CHECK_EQ(out[0], 11);
  GGO_CHECK_EQ(out[1], 13);
  GGO_CHECK_EQ(out[2], 15);
  GGO_CHECK_EQ(out[3], 17);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, kernel_smaller_than_data)
{
  const int in[5] = { 1, 6, 2, 3, 5 };
  const int kernel[3] = { 1, 1, 1 };
  int out[5] = { 0, 0, 0, 0, 0 };

  auto read = [&](int i) { return in[i]; };
  auto write = [&](int i, int v) { out[i] = v; };

  ggo::symmetric_convolution<ggo::border_mode::duplicate_edge>(read, write, 5, 0, 4, kernel, 3);
  GGO_CHECK_EQ(out[0], 11);
  GGO_CHECK_EQ(out[1], 13);
  GGO_CHECK_EQ(out[2], 17);
  GGO_CHECK_EQ(out[3], 21);
  GGO_CHECK_EQ(out[4], 20);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, clipping_close_to_borders)
{
  const int in[5] = { 1, 6, 2, 3, 5 };
  const int kernel[3] = { 1, 1, 1 };
  int out[5] = { 0, 0, 0, 0, 0 };

  auto read = [&](int i) { return in[i]; };
  auto write = [&](int i, int v) { out[i] = v; };

  ggo::symmetric_convolution<ggo::border_mode::mirror>(read, write, 5, 1, 3, kernel, 3);
  GGO_CHECK_EQ(out[0], 0);
  GGO_CHECK_EQ(out[1], 13);
  GGO_CHECK_EQ(out[2], 17);
  GGO_CHECK_EQ(out[3], 21);
  GGO_CHECK_EQ(out[4], 18);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, clipping_inside_data)
{
  const int in[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  const int kernel[3] = { 1, 1, 1 };
  int out[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  auto read = [&](int i) { return in[i]; };
  auto write = [&](int i, int v) { out[i] = v; };

  ggo::symmetric_convolution<ggo::border_mode::mirror>(read, write, 8, 3, 4, kernel, 3);
  GGO_CHECK_EQ(out[0], 0);
  GGO_CHECK_EQ(out[1], 0);
  GGO_CHECK_EQ(out[2], 0);
  GGO_CHECK_EQ(out[3], 5);
  GGO_CHECK_EQ(out[4], 5);
  GGO_CHECK_EQ(out[5], 0);
  GGO_CHECK_EQ(out[6], 0);
  GGO_CHECK_EQ(out[7], 0);

}

