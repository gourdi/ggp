#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <kernel/math/interpolation/ggo_triangle_interpolation.h>
#include <2d/io/ggo_bmp.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_paint.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(triangular_interpolation, function)
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down> image({ 200, 200 });

  ggo::fill_black(image);

  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      const ggo::pos2_f p = ggo::from_discrete_to_continuous<float>(ggo::pos2_i(x, y));
      const ggo::pos2_f p0(50.f, 50.f);
      const ggo::pos2_f p1(150.f, 50.f);
      const ggo::pos2_f p2(50.f, 150.f);

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
  using color_triangle_t = ggo::solid_color_triangle<ggo::rgb_8u>;

  ggo::scene2d<ggo::rgb_8u> scene;
  const auto & triangle1 = scene.add_shape(color_triangle_t({ { 10.f, 10.f },{ 110.f, 10.f },{ 110.f, 90.f } }, ggo::green_32f(), ggo::red_32f(), ggo::yellow_32f()));
  scene.add_shape(color_triangle_t({ triangle1._triangle.v1(),{ 50.f, 90.f }, triangle1._triangle.v3() }, ggo::green_32f(), ggo::blue_32f(), ggo::yellow_32f()));

  ggo::image_t<ggo::pixel_type::rgb_8u> image({ 120, 100 });
  ggo::fill_solid(image, ggo::gray_8u());
  ggo::paint<ggo::sampling_4x4>(image, scene);

  ggo::save_bmp("paint_color_triangles.bmp", image);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, alpha_color_triangle)
{
  const int width = 120;
  const int height = 100;

  using color_triangle_t = ggo::alpha_color_triangle<ggo::rgb_8u>;
  using brush_color_t = color_triangle_t::brush_color_t;

  ggo::scene2d<ggo::rgb_8u> scene;

  const auto & triangle1 = scene.add_shape(color_triangle_t({ { 10.f, 10.f },{ 110.f, 10.f },{ 110.f, 90.f } },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.75f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f)));

  scene.add_shape(color_triangle_t({ triangle1._triangle.v1(),{ 50.f, 90.f }, triangle1._triangle.v3() },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.25f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f)));

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image({ 120, 100 });
  ggo::fill_solid(image, ggo::gray_8u());
  ggo::paint<ggo::sampling_4x4>(image, scene);

  ggo::save_bmp("paint_alpha_color_triangles.bmp", image);
}
