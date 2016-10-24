#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <ggo_shapes2d.h>
#include <ggo_chronometer.h>
#include <ggo_shapes2d.h>
#include <ggo_color.h>
#include <ggo_sampling_paint.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_buffer_paint.h>
#include <ggo_blit.h>
#include <ggo_color_triangle.h>
#include <ggo_blender_abc.h>
#include <ggo_bmp.h>
#include <array>

//#define GGO_BENCH

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, hard_rect_y_8u_yu)
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
GGO_TEST(paint, hard_rect_bgra_8u_yd)
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
GGO_TEST(paint, hard_rect_rgb_32f_yu)
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
GGO_TEST(paint, shape_y_8u_yu_sampling1)
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
GGO_TEST(paint, shape_y_8u_yu_sampling4x4)
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
GGO_TEST(paint, compare_rgb_8u_yu_and_bgra_8u_yd)
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

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, polygons_rectangles)
{
  const int width = 100;
  const int height = 100;
  const int line_step = 3 * width;

  // Paint polygons.
  std::array<ggo::solid_color_shape<ggo::polygon2d_float, ggo::color_8u>, 2> polygons;

  polygons[0]._color = ggo::color_8u::WHITE;
  polygons[0]._shape.add_point(10, 10);
  polygons[0]._shape.add_point(50, 10);
  polygons[0]._shape.add_point(50, 90);
  polygons[0]._shape.add_point(10, 90);

  polygons[1]._color = ggo::color_8u::WHITE;
  polygons[1]._shape.add_point(90, 10);
  polygons[1]._shape.add_point(50, 10);
  polygons[1]._shape.add_point(50, 90);
  polygons[1]._shape.add_point(90, 90);

  std::array<uint8_t, 3 * width * height> buffer_polygons;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer_polygons.data(), width, height, line_step, ggo::color_8u::BLUE);
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_polygons.data(), width, height, line_step, polygons.cbegin(), polygons.cend());

  ggo::save_bmp("paint_polygons.bmp", buffer_polygons.data(), ggo::rgb_8u_yu, width, height, line_step);

  // Paint rectangles.
  std::array<ggo::solid_color_shape<ggo::rect_float, ggo::color_8u>, 2> rectangles;
  rectangles[0] = { ggo::rect_float::from_left_right_bottom_top(10, 50, 10, 90), ggo::color_8u::WHITE };
  rectangles[1] = { ggo::rect_float::from_left_right_bottom_top(50, 90, 10, 90), ggo::color_8u::WHITE };

  std::array<uint8_t, 3 * width * height> buffer_rectangles;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer_rectangles.data(), width, height, line_step, ggo::color_8u::BLUE);
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_rectangles.data(), width, height, line_step, rectangles.cbegin(), rectangles.cend());

  ggo::save_bmp("paint_rectangles.bmp", buffer_rectangles.data(), ggo::rgb_8u_yu, width, height, line_step);

  // Compare images.
  GGO_CHECK(buffer_polygons == buffer_rectangles);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, y_32f_yu)
{
  const int width = 60;
  const int height = 60;

  std::vector<float> buffer_y_32f_yu(width * height, 0.0f);

  auto brush = [](int x, int y) { return 1.f; };
  auto blend = [](float bkgd_color, float brush_color) { return 0.2f * bkgd_color + 0.8f * brush_color; };

  ggo::paint_shape<ggo::y_32f_yu, ggo::sampling_4x4>(buffer_y_32f_yu.data(), width, height, width * sizeof(float),
    ggo::disc_float(0.25f * width, 0.25f * height, 20.f),
    brush, blend);

  // Convert to rgb_8u_yu
  std::vector<uint8_t> buffer_rgb_8u_yu(3 * width * height);

  ggo::blit<ggo::y_32f_yu, ggo::rgb_8u_yu>(buffer_y_32f_yu.data(), width, height, width * sizeof(float),
    buffer_rgb_8u_yu.data(), width, height, 3 * width, 0, 0);

  ggo::save_bmp("paint_y_32f_yu.bmp", buffer_rgb_8u_yu.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, extended_segment_8x8_sampling)
{
  const int width = 60;
  const int height = 60;

  ggo::extended_segment_float segment({ 10.f, 10.f }, { 50.f, 15.f }, 5.f);

  auto brush = [](int x, int y) { return ggo::color_8u::RED; };
  auto blend = [](const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color) { return brush_color; };

  std::vector<uint8_t> buffer(3 * width * height, 255);
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer.data(), width, height, 3 * width, segment, brush, blend);

  ggo::save_bmp("paint_extended_segment.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, difference)
{
  const int width = 100;
  const int height = 100;

  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> shape;
  shape.add_shape(std::make_shared<ggo::disc_float>(50.f, 50.f, 40.f));
  shape.add_shape(std::make_shared<ggo::disc_float>(70.f, 70.f, 30.f));

  std::array<uint8_t, 3 * width * height> buffer;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::color_8u::GRAY);
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, shape, ggo::color_8u::YELLOW);

  ggo::save_bmp("paint_difference.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, color_triangle)
{
  const int width = 120;
  const int height = 100;

  using opaque_blender_rgb8u = ggo::opaque_blender<ggo::color_8u>;
  using color_triangle_rgb8u = ggo::color_triangle<opaque_blender_rgb8u, ggo::color_8u, ggo::color_32f, float>;

  std::vector<color_triangle_rgb8u> triangles;

  ggo::triangle2d_float triangle1({ 10.f, 10.f }, { 110.f, 10.f }, { 110.f, 90.f });
  triangles.emplace_back(triangle1, ggo::color_32f::GREEN, ggo::color_32f::RED, ggo::color_32f::YELLOW, opaque_blender_rgb8u());

  ggo::triangle2d_float triangle2(triangles[0]._triangle.v1(), { 50.f, 90.f }, triangles[0]._triangle.v3());
  triangles.emplace_back(triangle2, ggo::color_32f::GREEN, ggo::color_32f::BLUE, ggo::color_32f::YELLOW, opaque_blender_rgb8u());

  std::array<uint8_t, 3 * width * height> buffer;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::color_8u::GRAY);
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, triangles.begin(), triangles.end());

  ggo::save_bmp("paint_color_triangles.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

