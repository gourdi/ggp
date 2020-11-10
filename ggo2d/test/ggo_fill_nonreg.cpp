#include "ggo_2d_nonreg.h"
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_black_y_8u_rows_up)
{
  auto image = make_image_t<ggo::pixel_type::y_8u>({ 5, 3 }, {
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34 });

  ggo::fill_black(image);

  GGO_CHECK_PIXELS(image, {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_black_y_8u_rows_up_clipping)
{
  auto image = make_image_t<ggo::pixel_type::y_8u>({ 5, 3 }, {
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34 });

  ggo::fill_black(image, ggo::rect_int::from_left_right_bottom_top(2, 3, 0, 1));

  GGO_CHECK_PIXELS(image, {
    10, 11,  0,  0, 14,
    20, 21,  0,  0, 24,
    30, 31, 32, 33, 34 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_black_rgba_8u_rows_up)
{
  auto image = make_image_t<ggo::pixel_type::rgba_8u>({2, 3 }, {
    { 10, 11, 12, 13 }, { 14, 15, 16, 17 },
    { 20, 21, 22, 23 }, { 24, 25, 26, 27 },
    { 30, 31, 32, 33 }, { 34, 35, 36, 37 } });

  ggo::fill_black(image);

  GGO_CHECK_PIXELS(image, {
    { 0x00, 0x00, 0x00, 0xff }, { 0x00, 0x00, 0x00, 0xff },
    { 0x00, 0x00, 0x00, 0xff }, { 0x00, 0x00, 0x00, 0xff },
    { 0x00, 0x00, 0x00, 0xff }, { 0x00, 0x00, 0x00, 0xff } });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_y_8u_rows_up)
{
  auto image = make_image_t<ggo::pixel_type::y_8u>({ 5, 3 }, {
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34 });

  ggo::fill_solid(image, 42);

  GGO_CHECK_PIXELS(image, {
    42, 42, 42, 42, 42,
    42, 42, 42, 42, 42,
    42, 42, 42, 42, 42 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_y_8u_rows_up_clipping)
{
  auto image = make_image_t<ggo::pixel_type::y_8u>({ 5, 3 }, {
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34 });

  ggo::fill_solid(image, 42, ggo::rect_int::from_left_right_bottom_top(2, 3, 0, 1));

  GGO_CHECK_PIXELS(image, {
    10, 11, 42, 42, 14,
    20, 21, 42, 42, 24,
    30, 31, 32, 33, 34 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_bgra_8u_rows_down)
{
  using memory_layout_t = ggo::rows_memory_layout<4, ggo::vertical_direction::down>;

  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  ggo::image_t<ggo::pixel_type::bgra_8u, memory_layout_t> image(buffer.data(), memory_layout_t({ 2, 6 }, 10));

  ggo::fill_solid(image, { uint8_t(96), uint8_t(97), uint8_t(98), uint8_t(99) });

  const std::vector<uint8_t> expected{
    98, 97, 96, 99, 98, 97, 96, 99, 18, 19,
    98, 97, 96, 99, 98, 97, 96, 99, 28, 29,
    98, 97, 96, 99, 98, 97, 96, 99, 38, 39,
    98, 97, 96, 99, 98, 97, 96, 99, 48, 49,
    98, 97, 96, 99, 98, 97, 96, 99, 58, 59,
    98, 97, 96, 99, 98, 97, 96, 99, 68, 69 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_rgb_32f_yu)
{
  constexpr int line_step = 10 * sizeof(float) + 1; // 1 extra byte per line.
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

  using memory_layout_t = ggo::rows_memory_layout<12, ggo::vertical_direction::up>;

  ggo::image_t<ggo::pixel_type::rgb_32f, memory_layout_t> image(buffer.data(), memory_layout_t({ 3, 4 }, line_step));

  ggo::fill_solid(image, { 97.f, 98.f, 99.f });

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

  compare_line(reinterpret_cast<float *>(buffer.data() + 0 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 19.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 1 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 29.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 2 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 39.f });
  compare_line(reinterpret_cast<float *>(buffer.data() + 3 * line_step), { 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 97.f, 98.f, 99.f, 49.f });

  // Check stride remained untouched.
  GGO_CHECK_EQ(buffer[1 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[2 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[3 * line_step - 1], 0);
  GGO_CHECK_EQ(buffer[4 * line_step - 1], 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_checker)
{
  ggo::image_y_8u image({ 7, 4 });

  ggo::fill_checker(image, 0x00, 0xff, 3);

  GGO_CHECK_PIXELS(image, {
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, 4colors)
{
  ggo::image_y_8u image({ 6, 5 });

  ggo::fill_4_colors(image, 0x00, 0xff, 0x80, 0x40);

  GGO_CHECK_PIXELS(image, {
    0x40, 0x66, 0x8c, 0xb3, 0xd9, 0xff, 
    0x50, 0x66, 0x7d, 0x93, 0xa9, 0xbf, 
    0x60, 0x66, 0x6d, 0x73, 0x79, 0x80, 
    0x70, 0x66, 0x5d, 0x53, 0x49, 0x40, 
    0x80, 0x66, 0x4d, 0x33, 0x1a, 0x00 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, curve)
{
  ggo::image_y_8u image({ 6, 5 });

  const std::map<float, float> curve{ { 0.0f, 0.0f }, { 0.5f, 0.2f }, { 1.0f, 1.f } };

  ggo::fill_color_curve(image, curve);

  GGO_CHECK_PIXELS(image, {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x99, 0x99, 0x99, 0x99, 0x99, 0x99,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, gaussian)
{
  ggo::image_y_8u image({ 6, 5 });

  ggo::fill_gaussian(image, 2.f, 0xff, 0x00);

  GGO_CHECK_PIXELS(image, {
    0x14, 0x35, 0x58, 0x58, 0x35, 0x14,
    0x2a, 0x71, 0xbb, 0xbb, 0x71, 0x2a,
    0x35, 0x91, 0xf0, 0xf0, 0x91, 0x35,
    0x2a, 0x71, 0xbb, 0xbb, 0x71, 0x2a,
    0x14, 0x35, 0x58, 0x58, 0x35, 0x14 });
}

#if 0       
/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, perlin)
{
  ggo::image_t<ggo::pixel_type::rgb_8u> image({ 160, 140 });

  ggo::fill_perlin(image, 5.f, ggo::red_8u(), ggo::yellow_8u());

  ggo::save_bmp("fill_perlin.bmp", image);
}
#endif

