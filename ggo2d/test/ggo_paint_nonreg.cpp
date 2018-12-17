#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <kernel/time/ggo_chronometer.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_color.h>
#include <2d/ggo_blit.h>
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>
#include <2d/paint/ggo_pixel_sampling.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_gradient_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/paint/ggo_blur_paint.h>

//#define GGO_BENCH

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_overwrite_sampling1)
{
  std::vector<uint8_t> buffer(10 * 11, 0);

  ggo::paint<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_f({ 2.5f, 3.5f }, 3.f), 0xff);

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
    ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_f(2.5f, 3.5f, 3.f), 0xff);
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

  ggo::paint<ggo::y_8u_yu, ggo::sampling_1>(buffer.data(), 10, 10, 11, ggo::disc_f({ 2.5f, 3.5f }, 3.f), brush, blender);

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

  ggo::paint<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_f({ 2.5f, 3.5f }, 3.f), 0xff);

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

  ggo::paint<ggo::y_8u_yu, ggo::sampling_4x4>(buffer.data(), 10, 10, 11, ggo::disc_f({ 2.5f, 3.5f }, 3.f),
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

  ggo::paint<ggo::y_8u_yu, ggo::sampling_16x16>(buffer.data(), 6, 6, 6, ggo::disc_f({ 3.f, 3.f }, 2.5f), 0xff);

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

  const ggo::disc_f disc({ 35.f, 25.f }, 15.f);

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_rgb_8u_yu.data(), width, height, 3 * width, disc, ggo::green_8u());
  ggo::paint<ggo::bgr_8u_yd, ggo::sampling_4x4>(buffer_bgr_8u_yd.data(), width, height, 4 * width, disc, ggo::green_8u());

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
  using paint_polygon = ggo::static_paint_shape<ggo::polygon2d_f, ggo::rgb_8u>;
  const paint_polygon polygon1({ { 10, 10 },{ 50, 10 },{ 50, 90 },{ 10, 90 } }, ggo::white_8u());
  const paint_polygon polygon2({ { 90, 10 },{ 50, 10 },{ 50, 90 },{ 90, 90 } }, ggo::white_8u());

  const std::vector<const paint_polygon *> polygons{ &polygon1, &polygon2 };

  ggo::array_8u buffer_polygons(3 * width * height);
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer_polygons.data(), width, height, line_step, ggo::blue_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer_polygons.data(), width, height, line_step, polygons);

  ggo::save_bmp("paint_polygons.bmp", buffer_polygons.data(), ggo::rgb_8u_yu, width, height, line_step);

  // Paint rectangles.
  using paint_rect = ggo::static_paint_shape<ggo::rect_f, ggo::rgb_8u>;
  const paint_rect rect1(ggo::rect_f::from_left_right_bottom_top(10, 50, 10, 90), ggo::white_8u());
  const paint_rect rect2(ggo::rect_f::from_left_right_bottom_top(50, 90, 10, 90), ggo::white_8u());

  const std::vector<const paint_rect *> rects{ &rect1, &rect2 };

  ggo::array_8u buffer_rectangles(3 * width * height);
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

  ggo::paint<ggo::y_32f_yu, ggo::sampling_4x4>(buffer.data(), width, height, width * sizeof(float),
    ggo::disc_f({ 0.25f * width, 0.25f * height }, 20.f),
    ggo::solid_color_brush<float>(1.f), ggo::alpha_blender_y32f(0.8f));

  ggo::save_bmp("paint_y_32f_yu.bmp", buffer.data(), ggo::y_32f_yu, width, height, width * sizeof(float));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, extended_segment_8x8_sampling)
{
  const int width = 60;
  const int height = 60;

  std::vector<uint8_t> buffer(3 * width * height, 255);

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer.data(), width, height, 3 * width, 
    ggo::capsule_f({ 10.f, 10.f }, { 50.f, 15.f }, 5.f), ggo::red_8u());

  ggo::save_bmp("paint_extended_segment.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, difference)
{
  const int width = 100;
  const int height = 100;

  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> shape;
  shape.add_shape(std::make_shared<ggo::disc_f>(ggo::pos2_f(50.f, 50.f), 40.f));
  shape.add_shape(std::make_shared<ggo::disc_f>(ggo::pos2_f(70.f, 70.f), 30.f));

  ggo::array_8u buffer(3 * width * height);
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, shape, ggo::yellow_8u());

  ggo::save_bmp("paint_difference.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, blur)
{
  const int width = 120;
  const int height = 100;

  std::vector<uint8_t> buffer(3 * width * height, 0x00);

  auto paint_pixel = [&](int x, int y, int samples_count, int samples_sup)
  {
    ggo::rgb_8u c(uint8_t(ggo::round_div(samples_count * 0xff, samples_sup)), uint8_t(0x00), uint8_t(0x00));
    ggo::write_pixel<ggo::rgb_8u_yu>(buffer.data(), x, y, height, 3 * width, c);
  };

  {
    std::fill(buffer.begin(), buffer.end(), 0x00);

    ggo::paint_blur_shape(
      ggo::disc_f({ 50, 40 }, 20), width, height, 5.f, 1.5f, paint_pixel);

    ggo::save_bmp("paint_blur_1.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  }

  {
    std::fill(buffer.begin(), buffer.end(), 0x00);

    ggo::paint_blur_shape(
      ggo::disc_f({ 50, 40 }, 20), width, height, 5.f, 0.5f, paint_pixel);

    ggo::save_bmp("paint_blur_2.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, gradient)
{
  const int width = 140;
  const int height = 120;
  const int line_step = 3 * width;

  std::vector<uint8_t> buffer(line_step * height, 0);

  ggo::pos2_f center(40.f, 50.f);
  ggo::gradient_brush<ggo::rgb_8u> brush(center, { 2.f, 1.f });
  brush.push_color(-20.f, ggo::yellow_8u());
  brush.push_color(  0.f, ggo::blue_8u());
  brush.push_color( 20.f, ggo::green_8u());

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_f(center, 35.f),
    brush, ggo::overwrite_blender<ggo::rgb_8u, ggo::rgb_8u>());

  ggo::save_bmp("paint_gradient.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, clipping)
{
  const int width = 140;
  const int height = 120;
  const int line_step = 3 * width;

  std::vector<uint8_t> buffer(line_step * height, 0);

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_f({ 70.f, 40.f }, 35.f),
    ggo::make_solid_brush<ggo::rgb_8u>(ggo::red_8u()), ggo::overwrite_blender<ggo::rgb_8u, ggo::rgb_8u>(),
    ggo::rect_int::from_left_right_bottom_top(0, width / 2, 0, height - 1), 8, 2);

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer.data(), width, height, line_step, ggo::disc_f({ 70.f, 60.f }, 35.f),
    ggo::make_solid_brush<ggo::rgb_8u>(ggo::blue_8u()), ggo::overwrite_blender<ggo::rgb_8u, ggo::rgb_8u>(),
    ggo::rect_int::from_left_right_bottom_top(width / 2 + 1, width - 1, 0, height - 1), 8, 2);

  ggo::save_bmp("paint_clipping.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rgba8)
{
  const int width = 140;
  const int height = 120;
  const int line_step_argb = 4 * width;
  const int line_step_rgb = 3 * width;

  std::vector<uint8_t> buffer_argb(line_step_argb * height, 0);

  ggo::paint<ggo::rgba_8u_yd, ggo::sampling_4x4>(
    buffer_argb.data(), width, height, line_step_argb, ggo::disc_f({ 70.f, 40.f }, 35.f),
    { 0xff, 0x00, 0x00, 0xff });

  ggo::paint<ggo::rgba_8u_yd, ggo::sampling_4x4>(
    buffer_argb.data(), width, height, line_step_argb, ggo::disc_f({ 70.f, 60.f }, 35.f),
    { 0x00, 0x00, 0xff, 0xff }, 0.5f);

  std::vector<uint8_t> buffer_rgb(line_step_rgb * height, 0xff);
  ggo::blit<ggo::rgba_8u_yd, ggo::rgb_8u_yd>(buffer_argb.data(), width, height, line_step_argb,
    buffer_rgb.data(), width, height, line_step_rgb, 0, 0);

  ggo::save_bmp("paint_rgba.bmp", buffer_rgb.data(), ggo::rgb_8u_yd, width, height, line_step_rgb);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rgba8_multishape)
{
  const int width = 140;
  const int height = 120;
  const int line_step_argb = 4 * width;
  const int line_step_rgb = 3 * width;

  std::vector<uint8_t> buffer_argb(line_step_argb * height, 0);

  using paint_disc = ggo::static_paint_shape<ggo::disc_f, ggo::rgba_8u>;
  std::vector<paint_disc> paints_discs;

  for (int i = 0; i < 512; ++i)
  {
    paints_discs.emplace_back(ggo::disc_f({ 0.5f * width + 0.01f * i, 0.5f * height }, 5.f), ggo::rgba_8u(255, 0, 0, 255));
  }
  ggo::paint_shapes<ggo::rgba_8u_yd, ggo::sampling_8x8>(buffer_argb.data(), width, height, line_step_argb, paints_discs);

  std::vector<uint8_t> buffer_rgb(line_step_rgb * height, 0xff);
  ggo::blit<ggo::rgba_8u_yd, ggo::rgb_8u_yd>(buffer_argb.data(), width, height, line_step_argb,
    buffer_rgb.data(), width, height, line_step_rgb, 0, 0);

  ggo::save_bmp("paint_rgba_multi.bmp", buffer_rgb.data(), ggo::rgb_8u_yd, width, height, line_step_rgb);
}


