#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <kernel/time/ggo_chronometer.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_color.h>
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>
#include <2d/brush/ggo_gradient_brush.h>
#include <2d/blend/ggo_additive_blend.h>
#include <2d/paint/ggo_paint_layer.h>
#include <2d/paint/ggo_blur_paint.h>

//#define GGO_BENCH
#if 0

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rect_y_8u_yu_overwrite_sampling16x16)
{
  constexpr int width = 6;
  constexpr int height = 4;

  std::vector<uint8_t> buffer(height * width, 0);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height }, width);

  ggo::paint<ggo::sampling_16x16>(image, ggo::rect_f(2.f, 1.f, 3.f, 1.f), 0xff);

  const std::vector<uint8_t> expected{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,	0x00, 0xff,	0xff,	0xff,	0x00,
    0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
    0x00,	0x00,	0x00,	0x00,	0x00,	0x00 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(paint, shape_y_8u_yu_overwrite_sampling1)
{
  constexpr int width = 10;
  constexpr int height = 10;
  constexpr int line_byte_step = 11;

  std::vector<uint8_t> buffer(height * line_byte_step, 0);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height }, line_byte_step);

  ggo::paint<ggo::sampling_1>(image, ggo::disc_f({ 2.5f, 3.5f }, 3.f), 0xff);

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
  constexpr int width = 10;
  constexpr int height = 10;
  constexpr int line_byte_step = 11;

  std::vector<uint8_t> buffer(height * line_byte_step, 0x80);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height }, line_byte_step);

  ggo::solid_color_brush<uint8_t> brush(0xff);
  ggo::additive_blender<uint8_t> blender;

  ggo::paint<ggo::sampling_1>(image, ggo::disc_f({ 2.5f, 3.5f }, 3.f), brush, blender);

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
  constexpr int width = 10;
  constexpr int height = 10;
  constexpr int line_byte_step = 11;

  std::vector<uint8_t> buffer(height * line_byte_step, 0);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height }, line_byte_step);

  ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f({ 2.5f, 3.5f }, 3.f), 0xff);

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
  constexpr int width = 10;
  constexpr int height = 10;
  constexpr int line_byte_step = 11;

  std::vector<uint8_t> buffer(height * line_byte_step, 0);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height }, line_byte_step);

  ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f({ 2.5f, 3.5f }, 3.f), 0xff_u8, 0.5f);

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
  constexpr int width = 6;
  constexpr int height = 6;

  std::vector<uint8_t> buffer(height * height, 0);
  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> image(buffer.data(), { width, height });

  ggo::paint<ggo::sampling_16x16>(image, ggo::disc_f({ 3.f, 3.f }, 2.5f), 0xff);

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
  constexpr int width = 80;
  constexpr int height = 60;
  std::vector<uint8_t> buffer_rgb_8u_yu(width * height * 3, 0);
  std::vector<uint8_t> buffer_bgr_8u_yd(width * height * 4, 0);
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>   image_rgb_8u_yu(buffer_rgb_8u_yu.data(), { width, height });
  ggo::image_t<ggo::pixel_type::bgr_8u, ggo::lines_order::down> image_bgr_8u_yd(buffer_bgr_8u_yd.data(), { width, height });

  const ggo::disc_f disc({ 35.f, 25.f }, 15.f);

  ggo::paint<ggo::sampling_4x4>(image_rgb_8u_yu, disc, ggo::green_8u());
  ggo::paint<ggo::sampling_4x4>(image_bgr_8u_yd, disc, ggo::green_8u());

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = image_rgb_8u_yu.read_pixel(x, y);
      auto c_bgr_8u_yd = image_bgr_8u_yd.read_pixel(x, y);
      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgr_8u_yd);
    }
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, polygons_rectangles)
{
  constexpr int width = 100;
  constexpr int height = 100;
  constexpr int line_byte_step = 3 * width;

  // Paint polygons.
  ggo::canvas<ggo::rgb_8u> canvas_polygons;
  canvas_polygons.make_layer(ggo::polygon2d_f({ { 10, 10 },{ 50, 10 },{ 50, 90 },{ 10, 90 } }), ggo::white_8u());
  canvas_polygons.make_layer(ggo::polygon2d_f({ { 90, 10 },{ 50, 10 },{ 50, 90 },{ 90, 90 } }), ggo::white_8u());

  std::vector<uint8_t> buffer_polygons(width * line_byte_step);
  ggo::image_t<ggo::pixel_type::rgb_8u> image_polygons(buffer_polygons.data(), { width, height });

  ggo::fill_solid(image_polygons, ggo::blue_8u());
  ggo::paint<ggo::sampling_4x4>(image_polygons, canvas_polygons);

  ggo::save_bmp("paint_polygons.bmp", image_polygons);

  // Paint rectangles.
  ggo::canvas<ggo::rgb_8u> canvas_rectangles;
  canvas_rectangles.make_layer(ggo::rect_f::from_left_right_bottom_top(10, 50, 10, 90), ggo::white_8u());
  canvas_rectangles.make_layer(ggo::rect_f::from_left_right_bottom_top(50, 90, 10, 90), ggo::white_8u());

  std::vector<uint8_t> buffer_rectangles(width * line_byte_step);
  ggo::image_t<ggo::pixel_type::rgb_8u> image_rectangles(buffer_rectangles.data(), { width, height });

  ggo::fill_solid(image_rectangles, ggo::blue_8u());
  ggo::paint<ggo::sampling_4x4>(image_rectangles, canvas_rectangles);

  ggo::save_bmp("paint_rectangles.bmp", image_rectangles);

  // Compare images.
  GGO_CHECK(buffer_polygons == buffer_rectangles);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, y_32f_yu)
{
  constexpr int width = 60;
  constexpr int height = 60;

  ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> image({ width, height });

  ggo::fill_solid(image, 0.1f);

  ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f({ 0.25f * width, 0.25f * height }, 20.f), 1.f, 0.8f);

  ggo::save_bmp("paint_y_32f_yu.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, extended_segment_8x8_sampling)
{
  constexpr int width = 60;
  constexpr int height = 60;

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image({ width, height });

  ggo::fill_solid(image, ggo::white_8u());

  ggo::paint<ggo::sampling_8x8>(image, ggo::capsule_f({ 10.f, 10.f }, { 50.f, 15.f }, 5.f), ggo::red_8u());

  ggo::save_bmp("paint_extended_segment.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, difference)
{
  constexpr int width = 100;
  constexpr int height = 100;

  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> shape;
  shape.add_shape(std::make_shared<ggo::disc_f>(ggo::pos2_f(50.f, 50.f), 40.f));
  shape.add_shape(std::make_shared<ggo::disc_f>(ggo::pos2_f(70.f, 70.f), 30.f));

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image({ width, height });
  ggo::fill_solid(image, ggo::gray_8u());
  ggo::paint<ggo::sampling_4x4>(image, shape, ggo::yellow_8u());

  ggo::save_bmp("paint_difference.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, blur)
{
  constexpr int width = 120;
  constexpr int height = 100;

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image({ width, height });

  auto paint_pixel = [&](int x, int y, int samples_count, int samples_sup)
  {
    ggo::rgb_8u c(uint8_t(ggo::round_div(samples_count * 0xff, samples_sup)), uint8_t(0x00), uint8_t(0x00));
    image.write_pixel(x, y, c);
  };

  {
    ggo::fill_black(image);

    ggo::paint_blur_shape(
      ggo::disc_f({ 50, 40 }, 20), width, height, 5.f, 1.5f, paint_pixel);

    ggo::save_bmp("paint_blur_1.bmp", image);
  }

  {
    ggo::fill_black(image);

    ggo::paint_blur_shape(
      ggo::disc_f({ 50, 40 }, 20), width, height, 5.f, 0.5f, paint_pixel);

    ggo::save_bmp("paint_blur_2.bmp", image);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, gradient)
{
  //constexpr int width = 140;
  //constexpr int height = 120;

  //ggo::image_t<ggo::pixel_type::rgb_8u> image({ width, height });
  //ggo::fill_black(image);

  //ggo::pos2_f center(40.f, 50.f);
  //ggo::gradient_brush<ggo::rgb_8u> brush(center, { 2.f, 1.f });
  //brush.push_color(-20.f, ggo::yellow_8u());
  //brush.push_color(  0.f, ggo::blue_8u());
  //brush.push_color( 20.f, ggo::green_8u());

  //ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f(center, 35.f), brush, ggo::overwrite_blender<ggo::rgb_8u>());

  //ggo::save_bmp("paint_gradient.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, clipping)
{
  const int width = 140;
  const int height = 120;

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image({ width, height });
  ggo::fill_black(image);

  ggo::paint<ggo::sampling_4x4>(
    image, ggo::disc_f({ 70.f, 40.f }, 35.f), ggo::red_8u(), 
    ggo::rect_int::from_left_right_bottom_top(0, width / 2, 0, height - 1));

  ggo::paint<ggo::sampling_4x4>(
    image, ggo::disc_f({ 70.f, 60.f }, 35.f), ggo::blue_8u(),
    ggo::rect_int::from_left_right_bottom_top(width / 2 + 1, width - 1, 0, height - 1));

  ggo::save_bmp("paint_clipping.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rgba8)
{
  //constexpr int width = 140;
  //constexpr int height = 120;

  //ggo::image_t<ggo::pixel_type::rgba_8u, ggo::lines_order::down> image({ width, height });
  //ggo::fill_black(image);
  //ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f({ 70.f, 40.f }, 35.f), { 0xff, 0x00, 0x00, 0xff });
  //ggo::paint<ggo::sampling_4x4>(image, ggo::disc_f({ 70.f, 60.f }, 35.f), { 0x00, 0x00, 0xff, 0xff }, 0.5f);

  //ggo::save_bmp("paint_rgba.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rgba8_multishape_sameshape)
{
  //constexpr int width = 140;
  //constexpr int height = 120;

  //ggo::image_t<ggo::pixel_type::rgba_8u, ggo::lines_order::down> image({ width, height });
  //ggo::fill_black(image);

  //ggo::canvas<ggo::rgba_8u> canvas;

  //for (int i = 0; i < 8; ++i)
  //{
  //  canvas.make_layer(ggo::disc_f({ 0.5f * width, 0.5f * height }, 50.f), { 255, 0, 0, 255 });
  //}
  //ggo::paint<ggo::sampling_8x8>(image, canvas);

  //ggo::save_bmp("paint_rgba_multi_shame_shape.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, rgba8_multishape_discs)
{
  //constexpr int width = 140;
  //constexpr int height = 120;

  //ggo::image_t<ggo::pixel_type::rgba_8u, ggo::lines_order::down> image({ width, height });
  //ggo::fill_checker(image, { 0xff, 0xff, 0xff, 0xff }, { 0x80, 0x80, 0x80, 0x80 }, 20);

  //ggo::canvas<ggo::rgba_8u> canvas;

  //canvas.make_layer(ggo::disc_f({ 0.4f * width, 0.5f * height }, 50.f), { 0xff, 0, 0, 0x80 });
  //canvas.make_layer(ggo::disc_f({ 0.5f * width, 0.5f * height }, 50.f), { 0xff, 0, 0, 0x80 });
  //canvas.make_layer(ggo::disc_f({ 0.6f * width, 0.5f * height }, 50.f), { 0xff, 0, 0, 0x80 });

  //ggo::paint<ggo::sampling_8x8>(image, canvas);

  //ggo::save_bmp("paint_rgba_multi_discs.bmp", image);
}

#endif

