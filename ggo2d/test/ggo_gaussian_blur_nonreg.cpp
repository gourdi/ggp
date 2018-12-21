//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/time/ggo_chronometer.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/blur/ggo_mean_box_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>
#include <string>
#include <optional>

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, y_32f)
{
  constexpr int width = 3;
  constexpr int height = 3;

  float image[width * height] = {
    0.f, 0.f, 1.f,
    0.f, 0.f, 0.f,
    1.f, 0.f, 0.f };

  ggo::gaussian_blur<ggo::y_32f_yu>(image, { width, height }, width * sizeof(float), 0.5f);

  const float expected[width * height] = {
    0.000000000f, 0.0951632485f, 0.798329771f,
    0.0951632485f, 0.0226874743f, 0.0951632485f,
    0.798329771f, 0.0951632485f, 0.000000000f };

  GGO_CHECK(ggo::compare(image, expected, 0.0001f) == true);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, y_8u)
{
  constexpr int width = 7;
  constexpr int height = 4;

  uint8_t image[width * height] = {
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

  ggo::gaussian_blur<ggo::y_8u_yu>(image, { width, height }, width * sizeof(uint8_t), 0.5f);

  const uint8_t expected[width * height] = {
    0xff, 0xe4, 0x1b, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xe4, 0x1b, 0x00, 0x03, 0x15, 0x03,
    0xe4, 0xcc, 0x18, 0x00, 0x15, 0x9e, 0x15,
    0x1b, 0x18, 0x03, 0x00, 0x03, 0x15, 0x03 };
  GGO_CHECK(ggo::compare(image, expected) == true);
}

//#define GGO_BENCH 1

namespace
{
  template <ggo::image_format format>
  void gaussian_blur2d_test(void * buffer, int width, int height, int line_step, const std::string & filename)
  {
    ggo::fill_solid<format>(buffer, width, height, line_step, ggo::blue_8u());

    ggo::paint<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_f({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), ggo::green<ggo::rgb_8u>());

    ggo::paint<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_f({ 0.f, 0.f }, 0.25f * std::min(width, height)), ggo::green<ggo::rgb_8u>());
    
    ggo::gaussian_blur<format>(buffer, { width, height }, line_step, 5.f);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur2d<format>(buffer, line_step, { width, height }, 5.f);
    }
    std::cout << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, buffer, format, width, height, line_step);
#endif
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, gaussian_rgb8u)
{
  const int width = 800;
  const int height = 600;

  // rgb_8u_yu
  const int line_step_rgb_8u_yu = 3 * width;
  std::vector<uint8_t> buffer_rgb_8u_yu(line_step_rgb_8u_yu * height);
  gaussian_blur2d_test<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), width, height, line_step_rgb_8u_yu, "gaussian_blur_rgb_8u_yu.bmp");

  // bgra_8u_yd
  const int line_step_bgr_8u_yd = 3 * width;
  std::vector<uint8_t> buffer_bgr_8u_yd(line_step_bgr_8u_yd * height);
  gaussian_blur2d_test<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), width, height, line_step_bgr_8u_yd, "gaussian_blur_bgr_8u_yd.bmp");

  // Compare buffers.
#ifndef GGO_BENCH
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::read_pixel<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, line_step_rgb_8u_yu);
      auto c_bgr_8u_yd = ggo::read_pixel<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), x, y, height, line_step_bgr_8u_yd);

      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgr_8u_yd);
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, gaussian_y_8u_yu)
{
  const int width = 800;
  const int height = 600;
  const int line_step = width;

  std::vector<uint8_t> buffer(line_step * height);

  ggo::fill_solid<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 0x00);

  ggo::paint<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_f({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), 0xff);

  ggo::paint<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_f({ 0.f, 0.f }, 0.25f * std::min(width, height)), 0xff);

  ggo::gaussian_blur<ggo::y_8u_yu>(buffer.data(), { width, height }, line_step, 5.f);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000; ++i)
  {
    ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer.data(), line_step, { width, height }, 5.f);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#else
  ggo::save_bmp("gaussian_blur_y_8u_yu.bmp", buffer.data(), ggo::y_8u_yu, width, height, line_step);
#endif
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, gaussian_rgb_8u_clipping)
{
  const int width = 400;
  const int height = 300;
  const int line_step = 3 * width;
  const auto clipping = ggo::rect_int::from_left_right_bottom_top(2, 100, 4, 200);

  std::vector<uint8_t> buffer(line_step * height);

  // rgb yu
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, line_step, ggo::blue_8u());

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_f({ 10.f, 0.5f * height }, 0.5f * height - 8.f), ggo::red_8u());

  ggo::gaussian_blur<ggo::rgb_8u_yu>(buffer.data(), { width, height }, line_step, clipping, 5.f);

  ggo::save_bmp("gaussian_blur_rgb_8u_yu_clipping.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, line_step);

  // rgb yd
  ggo::fill_solid<ggo::rgb_8u_yd>(buffer.data(), width, height, line_step, ggo::blue_8u());

  ggo::paint<ggo::rgb_8u_yd, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_f({ 10.f, 0.5f * height }, 0.5f * height - 8.f), ggo::red_8u());

  ggo::gaussian_blur<ggo::rgb_8u_yd>(buffer.data(), { width, height }, line_step, clipping, 5.f);

  ggo::save_bmp("gaussian_blur_rgb_8u_yd_clipping.bmp", buffer.data(), ggo::rgb_8u_yd, width, height, line_step);
}

