#include <ggo_nonreg.h>
#include <ggo_buffer_fill.h>
#include <ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(fill, perlin)
{
  const int width = 160;
  const int height = 140;

  std::vector<uint8_t> buffer(3 * width * height);

  ggo::fill_perlin<ggo::rgb_8u_yu>(buffer.data(), width, height, 3 * width, 5.f, ggo::red<ggo::color_8u>(), ggo::yellow<ggo::color_8u>());

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



