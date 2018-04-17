#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <ggo_shapes2d.h>
#include <ggo_chronometer.h>
#include <ggo_shapes2d.h>
#include <ggo_color.h>
#include <ggo_pixel_sampling.h>
#include <ggo_paint.h>
#include <ggo_blit.h>
#include <ggo_color_triangle.h>
#include <ggo_fill.h>
#include <ggo_bmp.h>
#include <ggo_brush.h>
#include <ggo_gradient_brush.h>
#include <ggo_blend.h>
#include <ggo_blur_paint.h>
#include <array>

//#define GGO_BENCH

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_overwrite_sampling1)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_float(2.5f, 3.5f, 3.f), 0xff);

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
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_float(2.5f, 3.5f, 3.f), 0xff);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_add_sampling1)
{
  std::vector<uint8_t> buffer(10 * 11, 0x80);

  ggo::solid_color_brush<uint8_t> brush(0xff);
  ggo::add_blender<uint8_t, uint8_t> blender;

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_float(2.5f, 3.5f, 3.f), brush, blender);

  const std::vector<uint8_t> expected{
    0x80, 0x80, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,	0x80,
    0xff,	0xff, 0xff,	0xff,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0xff,	0xff,	0xff,	0xff,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0x80,	0x80,	0xff,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,	0x80,
    0x80,	0x80, 0x80,	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_overwrite_sampling4x4)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_float(2.5f, 3.5f, 3.f), 0xff);

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
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_alpha_sampling4x4)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_float(2.5f, 3.5f, 3.f),
    ggo::solid_color_brush<uint8_t>(0xff), ggo::alpha_blender_y8u(0.5f));

  const std::vector<uint8_t> expected{
    0x00, 0x28, 0x40, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x50, 0x80, 0x80, 0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x50, 0x80, 0x80, 0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x28, 0x40, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,	0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_sampling16x16)
{
  std::vector<uint8_t> buffer(6 * 6, 0);

  ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_16x16>(buffer.data(), 6, 6, 6, ggo::disc_float(3.f, 3.f, 2.5f), 0xff);

  const std::vector<uint8_t> expected{
    0x00, 0x13, 0x6f, 0x6f, 0x13, 0x00,
    0x13, 0xe3, 0xff, 0xff, 0xe3, 0x13,
    0x6f, 0xff, 0xff, 0xff, 0xff, 0x6f,
    0x6f, 0xff, 0xff, 0xff, 0xff, 0x6f,
    0x13, 0xe3, 0xff, 0xff, 0xe3, 0x13,
    0x00, 0x13, 0x6f, 0x6f, 0x13, 0x00 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, compare_rgb_8u_yu_and_bgra_8u_yd)
{
  const int width = 80;
  const int height = 60;
  std::vector<uint8_t> buffer_rgb_8u_yu(width * height * 3, 0);
  std::vector<uint8_t> buffer_bgr_8u_yd(width * height * 4, 0);

  const ggo::disc_float disc(35.f, 25.f, 15.f);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_rgb_8u_yu.data(), width, height, 3 * width, disc, ggo::green_8u());
  ggo::paint_shape<ggo::bgr_8u_yd, ggo::sampling_4x4>(buffer_bgr_8u_yd.data(), width, height, 4 * width, disc, ggo::green_8u());

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = ggo::read_pixel<ggo::rgb_8u_yu>(buffer_rgb_8u_yu.data(), x, y, height, 3 * width);
      auto c_bgr_8u_yd = ggo::read_pixel<ggo::bgr_8u_yd>(buffer_bgr_8u_yd.data(), x, y, height, 4 * width);
      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgr_8u_yd);
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
  using paint_polygon = ggo::solid_color_shape<ggo::polygon2d_float, ggo::color_8u>;
  const paint_polygon polygon1({ { 10, 10 },{ 50, 10 },{ 50, 90 },{ 10, 90 } }, ggo::white_8u());
  const paint_polygon polygon2({ { 90, 10 },{ 50, 10 },{ 50, 90 },{ 90, 90 } }, ggo::white_8u());

  const std::vector<const paint_polygon *> polygons{ &polygon1, &polygon2 };

  std::array<uint8_t, 3 * width * height> buffer_polygons;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer_polygons.data(), width, height, line_step, ggo::blue_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_polygons.data(), width, height, line_step, polygons);

  ggo::save_bmp("paint_polygons.bmp", buffer_polygons.data(), ggo::rgb_8u_yu, width, height, line_step);

  // Paint rectangles.
  using paint_rect = ggo::solid_color_shape<ggo::rect_float, ggo::color_8u>;
  const paint_rect rect1(ggo::rect_float::from_left_right_bottom_top(10, 50, 10, 90), ggo::white_8u());
  const paint_rect rect2(ggo::rect_float::from_left_right_bottom_top(50, 90, 10, 90), ggo::white_8u());

  const std::vector<const paint_rect *> rects{ &rect1, &rect2 };

  std::array<uint8_t, 3 * width * height> buffer_rectangles;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer_rectangles.data(), width, height, line_step, ggo::blue_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_rectangles.data(), width, height, line_step, rects);

  ggo::save_bmp("paint_rectangles.bmp", buffer_rectangles.data(), ggo::rgb_8u_yu, width, height, line_step);

  // Compare images.
  GGO_CHECK(buffer_polygons == buffer_rectangles);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, y_32f_yu)
{
  const int width = 60;
  const int height = 60;

  std::vector<float> buffer(width * height, 0.1f);

  ggo::paint_shape<ggo::y_32f_yu, ggo::sampling_4x4>(buffer.data(), width, height, width * sizeof(float),
    ggo::disc_float(0.25f * width, 0.25f * height, 20.f),
    ggo::solid_color_brush<float>(1.f), ggo::alpha_blender_y32f(0.8f));

  ggo::save_bmp("paint_y_32f_yu.bmp", buffer.data(), ggo::y_32f_yu, width, height, width * sizeof(float));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, extended_segment_8x8_sampling)
{
  const int width = 60;
  const int height = 60;

  std::vector<uint8_t> buffer(3 * width * height, 255);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer.data(), width, height, 3 * width, 
    ggo::extended_segment_float({ 10.f, 10.f }, { 50.f, 15.f }, 5.f), ggo::red_8u());

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
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, shape, ggo::yellow_8u());

  ggo::save_bmp("paint_difference.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, color_triangle)
{
  const int width = 120;
  const int height = 100;

  using color_triangle_t = ggo::solid_color_triangle<float, ggo::color_8u>;

  const color_triangle_t triangle1({ { 10.f, 10.f }, { 110.f, 10.f }, { 110.f, 90.f } }, ggo::green_32f(), ggo::red_32f(), ggo::yellow_32f());
  const color_triangle_t triangle2({ triangle1._triangle.v1(), { 50.f, 90.f }, triangle1._triangle.v3() }, ggo::green_32f(), ggo::blue_32f(), ggo::yellow_32f());

  const std::vector<const color_triangle_t *> triangles{ &triangle1, &triangle2 };

  std::array<uint8_t, 3 * width * height> buffer;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, triangles);

  ggo::save_bmp("paint_color_triangles.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, alpha_color_triangle)
{
  const int width = 120;
  const int height = 100;

  using color_triangle_t = ggo::alpha_color_triangle<float, ggo::color_8u>;
  using brush_color_t = color_triangle_t::brush_color_t;

  const color_triangle_t triangle1({ { 10.f, 10.f }, { 110.f, 10.f }, { 110.f, 90.f } },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.75f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f));

  const color_triangle_t triangle2({ triangle1._triangle.v1(), { 50.f, 90.f }, triangle1._triangle.v3() },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.25f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f));

  const std::vector<const color_triangle_t *> triangles{ &triangle1, &triangle2 };

  std::array<uint8_t, 3 * width * height> buffer;
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, triangles);

  ggo::save_bmp("paint_alpha_color_triangles.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, blur)
{
  const int width = 120;
  const int height = 100;

  std::vector<uint8_t> buffer(3 * width * height, 0x00);

  auto paint_pixel = [&](int x, int y, int samples_count, int samples_sup)
  {
    ggo::color_8u c(uint8_t(ggo::round_div(samples_count * 0xff, samples_sup)), uint8_t(0x00), uint8_t(0x00));
    ggo::write_pixel<ggo::rgb_8u_yu>(buffer.data(), x, y, height, 3 * width, c);
  };

  {
    std::fill(buffer.begin(), buffer.end(), 0x00);

    ggo::paint_blur_shape<ggo::blur_samples_type::disc_12_samples>(
      ggo::disc_float(50, 40, 20), width, height, 5.f, paint_pixel);

    ggo::save_bmp("paint_blur_12.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  }

  {
    std::fill(buffer.begin(), buffer.end(), 0x00);

    ggo::paint_blur_shape<ggo::blur_samples_type::disc_52_samples>(
      ggo::disc_float(50, 40, 20), width, height, 5.f, paint_pixel);

    ggo::save_bmp("paint_blur_52.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, gradient)
{
  const int width = 140;
  const int height = 120;
  const int line_step = 3 * width;

  std::vector<uint8_t> buffer(line_step * height, 0);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_float(40.f, 50.f, 35.f),
    ggo::gradient_brush<ggo::color_8u>(ggo::red<ggo::color_8u>(), { 20.f, 30.f }, ggo::green<ggo::color_8u>(), { 60.f, 70.f }),
    ggo::overwrite_blender<ggo::color_8u, ggo::color_8u>());

  ggo::save_bmp("paint_gradient.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, clipping)
{
  const int width = 140;
  const int height = 120;
  const int line_step = 3 * width;

  std::vector<uint8_t> buffer(line_step * height, 0);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_float(70.f, 40.f, 35.f),
    ggo::make_solid_brush<ggo::color_8u>(ggo::red_8u()), ggo::overwrite_blender<ggo::color_8u, ggo::color_8u>(),
    ggo::rect_int::from_left_right_bottom_top(0, width / 2, 0, height - 1), 8, 2);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_float(70.f, 60.f, 35.f),
    ggo::make_solid_brush<ggo::color_8u>(ggo::blue_8u()), ggo::overwrite_blender<ggo::color_8u, ggo::color_8u>(),
    ggo::rect_int::from_left_right_bottom_top(width / 2 + 1, width - 1, 0, height - 1), 8, 2);

  ggo::save_bmp("paint_clipping.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}



