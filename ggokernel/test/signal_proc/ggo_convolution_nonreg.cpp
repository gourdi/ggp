#include <ggo_nonreg.h>
#include <ggo_convolution.h>
#include <ggo_buffer_access.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, float_mirror)
{
  const float in[7] = { 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto left = [&](int x) { return ggo::get1d_mirror(in, x, 7); };
  auto center = [&](int x) { return in[x]; };
  auto right = [&](int x) { return ggo::get1d_mirror(in, x, 7); };
  auto output = [&](int x, float v) { ggo::set1d(out, x, 7, v); };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[3], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[6], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, float_zero_buffer_api)
{
  const float in[7] = { 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto left  = [&](int x) { return 0.f; };
  auto right = [&](int x) { return 0.f; };
  auto read  = [&](const void * ptr) { return *static_cast<const float*>(ptr); };
  auto write = [&](void * ptr, float v) { float * ptr_32f = static_cast<float*>(ptr); *ptr_32f = v; };

  ggo::apply_symetric_kernel_1d(in, sizeof(float), read, out, sizeof(float), write, left, right, 7, kernel, 2);

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 1.f / 3.f);
  GGO_CHECK_FABS(out[2], 1.f / 3.f);
  GGO_CHECK_FABS(out[3], 1.f / 3.f);
  GGO_CHECK_FABS(out[4], 0.f);
  GGO_CHECK_FABS(out[5], 1.f / 3.f);
  GGO_CHECK_FABS(out[6], 1.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution1d, uint8_zero_borders_fixed_point)
{
  const uint8_t in[7] = { 0, 0, 255, 0, 0, 0, 255 };
  const uint16_t kernel[2] = { 128, 64 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  auto left   = [&](int x) { return static_cast<uint16_t>(0); };
  auto center = [&](int x) { return static_cast<uint16_t>(in[x]); };
  auto right  = [&](int x) { return static_cast<uint16_t>(0); };
  auto output = [&](int x, uint16_t v) { out[x] = (v + 128) >> 8; };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel, 2);

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

  auto left   = [&](int x) { return 255.f; };
  auto center = [&](int x) { return ggo::to<float>(in[x]); };
  auto right  = [&](int x) { return 255.f; };
  auto output = [&](int x, float v) { ggo::set1d(out, x, 7, ggo::to<uint8_t>(v)); };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel, 2);

  GGO_CHECK(out[0] == 85);
  GGO_CHECK(out[1] == 85);
  GGO_CHECK(out[2] == 85);
  GGO_CHECK(out[3] == 85);
  GGO_CHECK(out[4] == 0);
  GGO_CHECK(out[5] == 85);
  GGO_CHECK(out[6] == 170);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(convolution2d, float_zero_horz)
{
  const float in[14] = {
    1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };
  float out[14] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto left =  [&](int x, int y) { GGO_CHECK(y >= 0 && y < 2); return 0.f; };
  auto right = [&](int x, int y)  { GGO_CHECK(y >= 0 && y < 2); return 0.f; };
  auto read  = [&](const void * ptr) { return *static_cast<const float*>(ptr); };
  auto write = [&](void * ptr, float v) { float * ptr_32f = static_cast<float*>(ptr); *ptr_32f = v; };

  ggo::apply_symetric_kernel_2d_horz<ggo::y_down>(in, sizeof(float), 7 * sizeof(float), read,
    out, sizeof(float), 7 * sizeof(float), write,
    left, right, 7, 2,  kernel, 2);

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
  const float kernel[2] = { 1.f / 3.f, 1.f / 3.f };

  auto bottom = [&](int x, int y) { return ggo::get2d_mirror<float, ggo::y_down>(in, x, y, 3, 3); };
  auto top    = [&](int x, int y) { return ggo::get2d_mirror<float, ggo::y_down>(in, x, y, 3, 3); };
  auto read   = [&](const void * ptr) { return *static_cast<const float*>(ptr); };
  auto write  = [&](void * ptr, float v) { float * ptr_32f = static_cast<float*>(ptr); *ptr_32f = v; };

  ggo::apply_symetric_kernel_2d_vert<ggo::y_down>(in, sizeof(float), 3 * sizeof(float), read,
    out, sizeof(float), 3 * sizeof(float), write,
    bottom, top, 3, 3, kernel, 2);

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
