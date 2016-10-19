//#define GGO_GAUSSIAN_DEBUG 1

#include <ggo_nonreg.h>
#include <ggo_chronometer.h>
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_bmp.h>

//#define GGO_BENCH 1

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian, rgb)
{
  const int width = 800;
  const int height = 600;
  const int line_step = 3 * width;

  std::vector<uint8_t> buffer(line_step * height);

  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, line_step, ggo::color_8u::BLUE);

  auto brush = [](int x, int y) { return ggo::color_8u(0xff, 0x00, 0xff); };
  auto blend = [](ggo::color_8u bkgd_color, ggo::color_8u brush_color) { return brush_color; };

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.5f * width, 0.5f * height, 0.25f * std::min(width, height)), brush, blend);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step,
    ggo::disc_float(0.f, 0.f, 0.25f * std::min(width, height)), brush, blend);

  ggo::gaussian_blur2d<ggo::rgb_8u_yu>(buffer.data(), width, height, line_step, 5.f);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000; ++i)
  {
    ggo::gaussian_blur2d<ggo::rgb_8u_yu>(buffer.data(), width, height, line_step, 5.f);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#endif

  ggo::save_bmp("gaussian_rgb.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, line_step);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian, y)
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
#endif

  ggo::save_bmp("gaussian_y.bmp", buffer.data(), ggo::y_8u_yu, width, height, line_step);
}

