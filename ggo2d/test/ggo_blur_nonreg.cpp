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

//#define GGO_BENCH 1

namespace
{
  template <ggo::image_format format>
  void gaussian_blur2d_test(void * buffer, int width, int height, int line_step, const std::string & filename)
  {
    ggo::fill_solid<format>(buffer, width, height, line_step, ggo::blue_8u());

    ggo::paint_shape<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), ggo::green<ggo::color_8u>());

    ggo::paint_shape<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float({ 0.f, 0.f }, 0.25f * std::min(width, height)), ggo::green<ggo::color_8u>());
    
    ggo::gaussian_blur2d<format>(buffer, line_step, { width, height }, 5.f);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur2d<pbf>(buffer, width, height, line_step, 5.f);
    }
    std::cout << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, buffer, format, width, height, line_step);
#endif
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(blur, gaussian_rgb8u)
{
  const int width = 800;
  const int height = 600;

  // rgb_8u_yu
  const int line_step_rgb_8u_yu = 3 * width;
  std::vector<uint8_t> buffer_rgb_8u_yu(line_step_rgb_8u_yu * height);
  gaussian_blur2d_test<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), width, height, line_step_rgb_8u_yu, "gaussian_blur_rgb_8u_yu.bmp");

  // bgra_8u_yd
  const int line_step_bgra_8u_yd = 3 * width;
  std::vector<uint8_t> buffer_bgr_8u_yd(line_step_bgra_8u_yd * height);
  gaussian_blur2d_test<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), width, height, line_step_bgra_8u_yd, "gaussian_blur_bgr_8u_yd.bmp");

  // Compare buffers.
#ifndef GGO_BENCH
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::read_pixel<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, line_step_rgb_8u_yu);
      auto c_bgr_8u_yd = ggo::read_pixel<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), x, y, height, line_step_bgra_8u_yd);

      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgr_8u_yd);
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////
GGO_TEST(blur, gaussian_y_8u_yu)
{
  const int width = 800;
  const int height = 600;
  const int line_step = width;

  std::vector<uint8_t> buffer(line_step * height);

  ggo::fill_solid<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 0x00);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), 0xff);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float({ 0.f, 0.f }, 0.25f * std::min(width, height)), 0xff);

  ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer.data(), line_step, { width, height }, 5.f);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000; ++i)
  {
    ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 5.f);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#else
  ggo::save_bmp("gaussian_blur_y_8u_yu.bmp", buffer.data(), ggo::y_8u_yu, width, height, line_step);
#endif
}

namespace
{
  template <ggo::image_format format>
  void mean_box_blur2d_test(void * buffer, int width, int height, int line_step, const std::string & filename)
  {
    ggo::fill_solid<format>(buffer, width, height, line_step, ggo::blue_8u());

    ggo::paint_shape<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), ggo::green<ggo::color_8u>());

    ggo::paint_shape<format, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float({ 0.f, 0.f }, 0.25f * std::min(width, height)), ggo::green<ggo::color_8u>());
    
    ggo::mean_box_blur2d<format>(buffer, line_step, { width, height }, 9);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur2d<pbf>(buffer, width, height, line_step, 5.f);
    }
    std::cout << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, buffer, format, width, height, line_step);
#endif
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(blur, mean_box_rgb8u)
{
  const int width = 800;
  const int height = 600;

  // rgb_8u_yu
  const int line_step_rgb_8u_yu = 3 * width;
  std::vector<uint8_t> buffer_rgb_8u_yu(line_step_rgb_8u_yu * height);
  mean_box_blur2d_test<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), width, height, line_step_rgb_8u_yu, "mean_box_blur_rgb_8u_yu.bmp");

  // bgra_8u_yd
  const int line_step_bgr_8u_yd = 3 * width;
  std::vector<uint8_t> buffer_bgr_8u_yd(line_step_bgr_8u_yd * height);
  mean_box_blur2d_test<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), width, height, line_step_bgr_8u_yd, "mean_box_blur_bgr_8u_yd.bmp");

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
GGO_TEST(blur, mean_box_y_8u_yu)
{
  const int width = 800;
  const int height = 600;
  const int line_step = width;

  std::vector<uint8_t> buffer(line_step * height);

  ggo::fill_solid<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 0x00);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), 0xff);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float({ 0.f, 0.f }, 0.25f * std::min(width, height)), 0xff);

  ggo::mean_box_blur2d<ggo::y_8u_yu>(buffer.data(), line_step, { width, height }, 9);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000; ++i)
  {
    ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 5.f);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#else
  ggo::save_bmp("mean_box_blur_y_8u_yu.bmp", buffer.data(), ggo::y_8u_yu, width, height, line_step);
#endif
}
