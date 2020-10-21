#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/interpolation/ggo_triangle_interpolation.h>
#include <2d/io/ggo_bmp.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_paint_layer.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(triangular_interpolation, function)
{
  ggo::image_rgb_8u image({ 200, 200 });

  ggo::fill_black(image);

  const ggo::pos2_f p0(50.f, 50.f);
  const ggo::pos2_f p1(150.f, 50.f);
  const ggo::pos2_f p2(50.f, 150.f);

  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      const ggo::pos2_f p = ggo::get_pixel_center<float>(ggo::pos2_i(x, y));

      auto c = ggo::triangular_interpolation(p0, ggo::red_32f(), p1, ggo::green_32f(), p2, ggo::blue_32f(), p);
      if (c.has_value() == true)
      {
        image.write_pixel(x, y, ggo::convert_color_to<ggo::rgb_8u>(*c));
      }
    }
  }
  
  ggo::save_bmp("test_triangular_interpolation_function.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, color_triangle)
{
  using color_triangle_t = ggo::color_triangle<ggo::rgb_8u, ggo::rgb_8u, ggo::rgb_32f>;

  ggo::canvas<ggo::rgb_8u> canvas;

  std::shared_ptr<const color_triangle_t> triangle1(new color_triangle_t({ { 10.f, 10.f }, { 110.f, 10.f }, { 110.f, 90.f } }, ggo::green_32f(), ggo::red_32f(), ggo::yellow_32f()));
  canvas.add_layer(triangle1);

  std::shared_ptr<const color_triangle_t> triangle2(new color_triangle_t({ triangle1->_triangle.v1(),{ 50.f, 90.f }, triangle1->_triangle.v3() }, ggo::green_32f(), ggo::blue_32f(), ggo::yellow_32f()));
  canvas.add_layer(triangle2);

  ggo::image_rgb_8u image({ 120, 100 });
  ggo::fill_solid(image, ggo::gray_8u());
  ggo::paint<ggo::sampling_4x4>(image, canvas);

  ggo::save_bmp("paint_color_triangles.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, alpha_color_triangle)
{
  using color_triangle_t = ggo::color_triangle<ggo::rgb_8u, ggo::rgba_8u, ggo::rgba_32f>;

  ggo::canvas<ggo::rgb_8u> canvas;

  std::shared_ptr<const color_triangle_t> triangle1(new color_triangle_t({ { 10.f, 10.f },{ 110.f, 10.f },{ 110.f, 90.f } },
    { 0.f, 1.f, 0.f, 1.f },
    { 0.f, 1.f, 0.f, 0.75f },
    { 0.f, 1.f, 0.f, 0.5f }));

  canvas.add_layer(triangle1);

  ggo::image_rgb_8u image({ 120, 100 });
  ggo::fill_solid(image, ggo::gray_8u());
  ggo::paint<ggo::sampling_4x4>(image, canvas);

  ggo::save_bmp("paint_alpha_color_triangles.bmp", image);
}
