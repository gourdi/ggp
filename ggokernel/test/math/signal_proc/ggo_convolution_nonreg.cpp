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

