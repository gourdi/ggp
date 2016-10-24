//#define GGO_GAUSSIAN_DEBUG 1

#include <ggo_nonreg.h>
#include <ggo_chronometer.h>
#include <ggo_gaussian_blur2d.h>
#include <ggo_mean_box_blur2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_bmp.h>
#include <string>

//#define GGO_BENCH 1

namespace
{
  template <ggo::pixel_buffer_format pbf>
  void gaussian_blur2d_test(void * buffer, int width, int height, int line_step, const std::string & filename)
  {
    ggo::fill_solid<pbf>(buffer, width, height, line_step, ggo::color_8u::BLUE);

    auto brush = [](int x, int y) { return ggo::color_8u::GREEN; };
    auto blend = [](ggo::color_8u bkgd_color, ggo::color_8u brush_color) { return brush_color; };

    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float(0.5f * width, 0.5f * height, 0.25f * std::min(width, height)), brush, blend);

    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float(0.f, 0.f, 0.25f * std::min(width, height)), brush, blend);

    ggo::gaussian_blur2d<pbf>(buffer, width, height, line_step, 5.f);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur2d<pbf>(buffer, width, height, line_step, 5.f);
    }
    std::cout << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, buffer, pbf, width, height, line_step);
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
  const int line_step_bgra_8u_yd = 4 * width;
  std::vector<uint8_t> buffer_bgra_8u_yd(line_step_bgra_8u_yd * height);
  gaussian_blur2d_test<ggo::bgra_8u_yd>(buffer_bgra_8u_yd.data(), width, height, line_step_bgra_8u_yd, "gaussian_blur_bgra_8u_yd.bmp");

  // Compare buffers.
#ifndef GGO_BENCH
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::get_pixel<ggo::rgb_8u_yu>(ggo::get_pixel_ptr<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, line_step_rgb_8u_yu));
      auto c_bgra_8u_yd = ggo::get_pixel<ggo::bgra_8u_yd>(ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer_bgra_8u_yd.data(), x, y, height, line_step_bgra_8u_yd));

      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgra_8u_yd);
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

  ggo::fill_solid<ggo::y_8u_yu, uint8_t>(buffer.data(), width, height, line_step, 0x00);

  auto brush = [](int x, int y) { return 0xff; };
  auto blend = [](uint8_t bkgd_color, uint8_t brush_color) { return brush_color; };

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.5f * width, 0.5f * height, 0.25f * std::min(width, height)), brush, blend);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.f, 0.f, 0.25f * std::min(width, height)), brush, blend);

  ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 5.f);

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
  template <ggo::pixel_buffer_format pbf>
  void mean_box_blur2d_test(void * buffer, int width, int height, int line_step, const std::string & filename)
  {
    ggo::fill_solid<pbf>(buffer, width, height, line_step, ggo::color_8u::BLUE);

    auto brush = [](int x, int y) { return ggo::color_8u::GREEN; };
    auto blend = [](ggo::color_8u bkgd_color, ggo::color_8u brush_color) { return brush_color; };

    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float(0.5f * width, 0.5f * height, 0.25f * std::min(width, height)), brush, blend);

    ggo::paint_shape<pbf, ggo::sampling_4x4>(
      buffer, width, height, line_step,
      ggo::disc_float(0.f, 0.f, 0.25f * std::min(width, height)), brush, blend);

    ggo::mean_box_blur2d<pbf>(buffer, width, height, line_step, 9);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur2d<pbf>(buffer, width, height, line_step, 5.f);
    }
    std::cout << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, buffer, pbf, width, height, line_step);
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
  const int line_step_bgra_8u_yd = 4 * width;
  std::vector<uint8_t> buffer_bgra_8u_yd(line_step_bgra_8u_yd * height);
  mean_box_blur2d_test<ggo::bgra_8u_yd>(buffer_bgra_8u_yd.data(), width, height, line_step_bgra_8u_yd, "mean_box_blur_bgra_8u_yd.bmp");

  // Compare buffers.
#ifndef GGO_BENCH
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::get_pixel<ggo::rgb_8u_yu>(ggo::get_pixel_ptr<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, line_step_rgb_8u_yu));
      auto c_bgra_8u_yd = ggo::get_pixel<ggo::bgra_8u_yd>(ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer_bgra_8u_yd.data(), x, y, height, line_step_bgra_8u_yd));

      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgra_8u_yd);
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

  ggo::fill_solid<ggo::y_8u_yu, uint8_t>(buffer.data(), width, height, line_step, 0x00);

  auto brush = [](int x, int y) { return 0xff; };
  auto blend = [](uint8_t bkgd_color, uint8_t brush_color) { return brush_color; };

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.5f * width, 0.5f * height, 0.25f * std::min(width, height)), brush, blend);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.f, 0.f, 0.25f * std::min(width, height)), brush, blend);

  ggo::mean_box_blur2d<ggo::y_8u_yu>(buffer.data(), width, height, line_step, 9);

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


