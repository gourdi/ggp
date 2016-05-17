#include <memory>
#include <ggo_nonreg.h>
#include <ggo_set2.h>
#include <ggo_shapes2d.h>
#include <ggo_triangle_interpolation.h>
#include <ggo_triangle_interpolation_brush.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(triangular_interpolation, function)
{
  const int IMAGE_SIZE = 200;
  ggo::rgb_image_buffer_uint8 image(IMAGE_SIZE, IMAGE_SIZE, ggo::color::BLACK);

  image.for_each_pixel([&](int x, int y)
  {
    const ggo::point2d_float p(static_cast<float>(x), static_cast<float>(y));
    const ggo::point2d_float p0(50, 50);
    const ggo::point2d_float p1(150, 50);
    const ggo::point2d_float p2(50, 150);
    
    ggo::color pixel_color(0);
    ggo::triangular_interpolation(p0, ggo::color::RED, p1, ggo::color::GREEN, p2, ggo::color::BLUE, p, pixel_color);
    image.set(x, y, pixel_color);
  });
  
  ggo::save_bmp("test_triangular_interpolation_function.bmp", image.data(), IMAGE_SIZE, IMAGE_SIZE);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(triangular_interpolation, brush)
{
  const int IMAGE_SIZE = 200;
  ggo::rgb_image_buffer_uint8 image(IMAGE_SIZE, IMAGE_SIZE, ggo::color::BLACK);
  
  const ggo::point2d_float p0(50, 50);
  const ggo::point2d_float p1(150, 50);
  const ggo::point2d_float p2(50, 150);
  
  auto triangle = std::make_shared<ggo::polygon2d_float>();
  triangle->add_points(p0, p1, p2);
  
  auto brush = std::make_shared<ggo::rgb_triangle_interpolation_brush>(p0, ggo::color::RED, p1, ggo::color::GREEN, p2, ggo::color::BLUE);
  
  ggo::paint(image, triangle, brush);
  
  ggo::save_bmp("test_triangular_interpolation_brush.bmp", image.data(), IMAGE_SIZE, IMAGE_SIZE);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(triangular_interpolation, opacity)
{
  const int IMAGE_SIZE = 200;
  ggo::rgb_image_buffer_uint8 image(IMAGE_SIZE, IMAGE_SIZE);
  
  fill_checker(image, ggo::color::WHITE, ggo::color::BLACK, 20);
  
  const ggo::point2d_float p0(50, 50);
  const ggo::point2d_float p1(150, 50);
  const ggo::point2d_float p2(50, 150);
  
  auto triangle = std::make_shared<ggo::polygon2d_float>();
  triangle->add_points(p0, p1, p2);
  
  auto color_brush = std::make_shared<ggo::rgb_solid_brush>(ggo::color::RED);
  auto opacity_brush = std::make_shared<ggo::opacity_triangle_interpolation_brush>(p0, 0.25f, p1, 0.5f, p2, 1.f);
  
  paint(image, triangle, color_brush, opacity_brush);
  
  ggo::save_bmp("test_triangular_interpolation_opacity.bmp", image.data(), IMAGE_SIZE, IMAGE_SIZE);
}

