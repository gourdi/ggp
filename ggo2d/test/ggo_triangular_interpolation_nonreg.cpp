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
  constexpr int image_size = 200;

  ggo::static_image<ggo::rgb_8u_yd> image(image_size, image_size);

  image.fill(ggo::black_8u());

  image.for_each_pixel([&](int x, int y)
  {
    const ggo::pos2_f p = ggo::from_pixel_to_math<float>(ggo::pos2_i(x, y));
    const ggo::pos2_f p0(50.f, 50.f);
    const ggo::pos2_f p1(150.f, 50.f);
    const ggo::pos2_f p2(50.f, 150.f);
    
    auto c = ggo::triangular_interpolation(p0, ggo::red_32f(), p1, ggo::green_32f(), p2, ggo::blue_32f(), p);
    if (c.has_value() == true)
    {
      image.write_pixel(x, y, ggo::convert_color_to<ggo::rgb_8u>(*c));
    }
  });
  
  ggo::save_bmp("test_triangular_interpolation_function.bmp", image);
}


////////////////////////////////////////////////////////////////////
GGO_TEST(paint, color_triangle)
{
  const int width = 120;
  const int height = 100;

  using color_triangle_t = ggo::solid_color_triangle<float, ggo::rgb_8u>;

  const color_triangle_t triangle1({ { 10.f, 10.f },{ 110.f, 10.f },{ 110.f, 90.f } }, ggo::green_32f(), ggo::red_32f(), ggo::yellow_32f());
  const color_triangle_t triangle2({ triangle1._triangle.v1(),{ 50.f, 90.f }, triangle1._triangle.v3() }, ggo::green_32f(), ggo::blue_32f(), ggo::yellow_32f());

  const std::vector<const color_triangle_t *> triangles{ &triangle1, &triangle2 };

  ggo::array_8u buffer(3 * width * height);
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, triangles);

  ggo::save_bmp("paint_color_triangles.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, alpha_color_triangle)
{
  const int width = 120;
  const int height = 100;

  using color_triangle_t = ggo::alpha_color_triangle<float, ggo::rgb_8u>;
  using brush_color_t = color_triangle_t::brush_color_t;

  const color_triangle_t triangle1({ { 10.f, 10.f },{ 110.f, 10.f },{ 110.f, 90.f } },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.75f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f));

  const color_triangle_t triangle2({ triangle1._triangle.v1(),{ 50.f, 90.f }, triangle1._triangle.v3() },
    brush_color_t(0.f, 1.f, 0.f, 1.f),
    brush_color_t(0.f, 1.f, 0.f, 0.25f),
    brush_color_t(0.f, 1.f, 0.f, 0.5f));

  const std::vector<const color_triangle_t *> triangles{ &triangle1, &triangle2 };

  ggo::array_8u buffer(3 * width * height);
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, ggo::gray_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, triangles);

  ggo::save_bmp("paint_alpha_color_triangles.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
