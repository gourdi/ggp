#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_y_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };

  ggo::fill_solid<ggo::y_8u_yu>(buffer.data(), 8, 6, 10, 99, ggo::rect_int::from_left_right_bottom_top(5, 12, 2, 8));

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
GGO_TEST(fill, fill_solid_bgra_8u_yd)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };

  ggo::fill_solid<ggo::bgra_8u_yd>(buffer.data(), 2, 6, 10, { uint8_t(96), uint8_t(97), uint8_t(98), uint8_t(99) }, ggo::rect_int::from_left_right_bottom_top(-1, 0, -1, 2));

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    98, 97, 96, 99, 44, 45, 46, 47, 48, 49,
    98, 97, 96, 99, 54, 55, 56, 57, 58, 59,
    98, 97, 96, 99, 64, 65, 66, 67, 68, 69 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, fill_solid_rgb_32f_yu)
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

  ggo::fill_solid<ggo::rgb_32f_yu>(buffer.data(), 3, 4, line_step, { 97.f, 98.f, 99.f }, ggo::rect_int::from_left_right_bottom_top(-1, 1, 2, 5));

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
GGO_TEST(fill, perlin)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);

  ggo::fill_perlin<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, 5.f, ggo::red_8u(), ggo::yellow_8u());

  ggo::save_bmp("fill_perlin.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, 4colors)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);

  ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, 
    ggo::red<ggo::color_8u>(),
    ggo::yellow<ggo::color_8u>(),
    ggo::blue<ggo::color_8u>(),
    ggo::white<ggo::color_8u>(),
    ggo::rect_int::from_width_height(width, height));

  ggo::save_bmp("fill_4colors.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, curve)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);
  ggo::linear_curve<float, ggo::color_32f> curve;

  curve.push_point(0.0f, ggo::red<ggo::color_32f>());
  curve.push_point(0.5f, ggo::green<ggo::color_32f>());
  curve.push_point(1.0f, ggo::yellow<ggo::color_32f>());

  ggo::fill_color_curve<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, curve);

  ggo::save_bmp("fill_curve.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, gaussian)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);

  ggo::fill_gaussian<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, 50.f, ggo::yellow<ggo::color_8u>(), ggo::blue<ggo::color_8u>());

  ggo::save_bmp("fill_gaussian.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, checker)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);

  ggo::fill_checker<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::yellow<ggo::color_8u>(), ggo::blue<ggo::color_8u>(), 10);

  ggo::save_bmp("fill_checker.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}



