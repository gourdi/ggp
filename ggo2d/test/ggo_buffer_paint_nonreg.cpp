#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <ggo_chronometer.h>
#include <ggo_shapes2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_bmp.h>

//#define GGO_BENCH

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, hard_rect_y_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };

  ggo::paint_rect_safe<ggo::y_8u_yu>(buffer.data(), 8, 6, 10, 5, 12, 2, 8, { 99 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 99, 99, 99, 38, 39,
    40, 41, 42, 43, 44, 99, 99, 99, 48, 49,
    50, 51, 52, 53, 54, 99, 99, 99, 58, 59,
    60, 61, 62, 63, 64, 99, 99, 99, 68, 69 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, hard_rect_bgra_8u_yd)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };

  ggo::paint_rect_safe<ggo::bgra_8u_yd>(buffer.data(), 2, 6, 10, 0, -1, 2, -1, { 97, 98, 99 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    99, 98, 97, 43, 44, 45, 46, 47, 48, 49,
    99, 98, 97, 53, 54, 55, 56, 57, 58, 59,
    99, 98, 97, 63, 64, 65, 66, 67, 68, 69 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, hard_rect_rgb_32f_yu)
{
  const int line_step = 10 * sizeof(float) + 1; // 1 extra byte per line.
  std::vector<uint8_t> buffer(4 * line_step, 0);

  auto fill_line = [](float * line, float start_value)
  {
    line[0] = start_value + 0;
    line[1] = start_value + 1;
    line[2] = start_value + 2;
    line[3] = start_value + 3;
    line[4] = start_value + 4;
    line[5] = start_value + 5;
    line[6] = start_value + 6;
    line[7] = start_value + 7;
    line[8] = start_value + 8;
    line[9] = start_value + 9;
  };

  fill_line(reinterpret_cast<float *>(buffer.data() + 0 * line_step), 10);
  fill_line(reinterpret_cast<float *>(buffer.data() + 1 * line_step), 20);
  fill_line(reinterpret_cast<float *>(buffer.data() + 2 * line_step), 30);
  fill_line(reinterpret_cast<float *>(buffer.data() + 3 * line_step), 40);

  ggo::paint_rect_safe<ggo::rgb_32f_yu>(buffer.data(), 3, 4, line_step, 1, -1, 5, 2, { 97.f, 98.f, 99.f });

  auto compare_line = [&](float * line, const std::vector<float> & expected)
  {
    GGO_CHECK(line[0] == expected[0]);
    GGO_CHECK(line[1] == expected[1]);
    GGO_CHECK(line[2] == expected[2]);
    GGO_CHECK(line[3] == expected[3]);
    GGO_CHECK(line[4] == expected[4]);
    GGO_CHECK(line[5] == expected[5]);
    GGO_CHECK(line[6] == expected[6]);
    GGO_CHECK(line[7] == expected[7]);
    GGO_CHECK(line[8] == expected[8]);
    GGO_CHECK(line[9] == expected[9]);
  };

  compare_line(reinterpret_cast<float *>(buffer.data() + 0 * line_step), { 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 1 * line_step), { 20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 2 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 36.f, 37.f, 38.f, 39.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 3 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 46.f, 47.f, 48.f, 49.f });

  // Check stride remained untouched.
  GGO_CHECK_EQ(buffer[1 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[2 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[3 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[4 * line_step - 1], 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, shape_y_8u_yu_sampling1)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  auto brush = [](int x, int y) { return 0xff; };
  auto blend = [](uint8_t bkgd_color, uint8_t brush_color) { return brush_color; };
  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_float(2.f, 3.f, 3.f), brush, blend);

  const std::vector<uint8_t> expected{
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,
    0xff,	0xff, 0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0x00,	0x00,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0x00,	0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  GGO_CHECK(buffer == expected);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000000; ++i)
  {
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_float(2.f, 3.f, 3.f), brush, blend);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, shape_y_8u_yu_sampling4x4)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  auto brush = [](int x, int y) { return 0xff; };
  auto blend = [](uint8_t bkgd_color, uint8_t brush_color) { return brush_color; };
  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_float(2.f, 3.f, 3.f), brush, blend);

  const std::vector<uint8_t> expected{
    0x00, 0x50, 0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9f, 0xff, 0xff, 0xff, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9f, 0xff, 0xff, 0xff, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x50, 0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,	0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  GGO_CHECK(buffer == expected);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000000; ++i)
  {
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_float(2.f, 3.f, 3.f), brush, blend);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_paint, compare_rgb_8u_yu_and_bgra_8u_yd)
{
  const int width = 80;
  const int height = 60;
  std::vector<uint8_t> buffer_rgb_8u_yu(width * height * 3, 0);
  std::vector<uint8_t> buffer_bgra_8u_yd(width * height * 4, 0);

  auto brush = [](int x, int y) { return ggo::color_8u(0xff, 0x00, 0x00); };
  auto blend = [](ggo::color_8u bkgd_color, ggo::color_8u brush_color) { return brush_color; };

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer_rgb_8u_yu.data(), width, height, 3 * width,
    ggo::disc_float(35.f, 25.f, 15.f), brush, blend);

  ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(
    buffer_bgra_8u_yd.data(), width, height, 4 * width,
    ggo::disc_float(35.f, 25.f, 15.f), brush, blend);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::get_pixel<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, 3 * width);
      auto c_bgra_8u_yd = ggo::get_pixel<ggo::bgra_8u_yd>(buffer_bgra_8u_yd.data(), x, y, height, 4 * width);
      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgra_8u_yd);
    }
  }
}


