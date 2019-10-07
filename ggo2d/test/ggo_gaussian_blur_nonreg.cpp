//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/time/ggo_chronometer.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/io/ggo_bmp.h>

////////////////////////////////////////////////////////////////////
template <ggo::lines_order memory_lines_order>
bool test_y_32f()
{
  constexpr int width = 7;
  constexpr int height = 4;

  float pixels[width * height] = {
    1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, };

  ggo::gaussian_blur(ggo::image_view_t<ggo::pixel_type::y_32f, memory_lines_order>(pixels, { width, height }), 0.5f);

  const float expected[width * height] = {
    1.00000000f,  0.893493056f,  0.106506981f,  0.000000000f, 0.000000000f,  0.0000000000f, 0.000000000f,
    1.00000000f,  0.893493056f,  0.106506981f,  0.000000000f, 0.0113437371f, 0.0838195086f, 0.0113437371f,
    0.893493056f, 0.798329771f,  0.0951632485f, 0.000000000f, 0.0838195086f, 0.619347036f,  0.0838195086f,
    0.106506981f, 0.0951632485f, 0.0113437371f, 0.000000000f, 0.0113437371f, 0.0838195086f, 0.0113437371f };
  return ggo::compare(pixels, expected, 0.0001f);
}

GGO_TEST(gaussian_blur, y_32f)
{
  GGO_CHECK(test_y_32f<ggo::lines_order::up>());
  GGO_CHECK(test_y_32f<ggo::lines_order::down>());
}

////////////////////////////////////////////////////////////////////
template <ggo::lines_order memory_lines_order>
bool test_y_8u()
{
  constexpr int width = 7;
  constexpr int height = 4;

  uint8_t pixels[width * height] = {
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

  ggo::gaussian_blur(ggo::image_view_t<ggo::pixel_type::y_8u, memory_lines_order>(pixels, { width, height }), 0.5f);

  const uint8_t expected[width * height] = {
    0xff, 0xe4, 0x1b, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xe4, 0x1b, 0x00, 0x03, 0x15, 0x03,
    0xe4, 0xcc, 0x18, 0x00, 0x15, 0x9e, 0x15,
    0x1b, 0x18, 0x03, 0x00, 0x03, 0x15, 0x03, };
  return ggo::compare(pixels, expected);
}

GGO_TEST(gaussian_blur, y_8u)
{
  GGO_CHECK(test_y_8u<ggo::lines_order::up>());
  GGO_CHECK(test_y_8u<ggo::lines_order::down>());
}

//#define GGO_BENCH 1

namespace
{
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
  void gaussian_blur2d_test(ggo::image_t<pixel_type, memory_lines_order> & image, const std::string & filename)
  {
    ggo::fill_solid(image, ggo::blue_8u());

    ggo::paint<ggo::sampling_4x4>(image,
      ggo::disc_f({ 0.5f * image.width(), 0.5f * image.height() }, 0.25f * std::min(image.width(), image.height())), ggo::green_8u());

    ggo::paint<ggo::sampling_4x4>(image,
      ggo::disc_f({ 0.f, 0.f }, 0.25f * std::min(image.width(), image.height())), ggo::green_8u());
    
    ggo::gaussian_blur(image, 5.f);

#ifdef GGO_BENCH
    ggo::chronometer chronometer;
    for (int i = 0; i < 1000; ++i)
    {
      ggo::gaussian_blur(image, 5.f);
    }
    std::cout << filename << ": " << chronometer.get_display_time(true) << std::endl;
#else
    ggo::save_bmp(filename, image);
#endif
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, gaussian_rgb8u)
{
  constexpr int width = 800;
  constexpr int height = 600;

  // rgb_8u_yu
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> image_rgb_8u_yu({ width, height });
  gaussian_blur2d_test(image_rgb_8u_yu, "gaussian_blur_rgb_8u_yu.bmp");

  // bgr_8u_yd
  ggo::image_t<ggo::pixel_type::bgr_8u, ggo::lines_order::down> image_bgr_8u_yd({ width, height });
  gaussian_blur2d_test(image_bgr_8u_yd, "gaussian_blur_bgr_8u_yd.bmp");

  // Compare buffers.
#ifndef GGO_BENCH
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      auto c_rgb_8u_yu = image_rgb_8u_yu.read_pixel(x, y);
      auto c_bgr_8u_yd = image_bgr_8u_yd.read_pixel(x, y);

      GGO_CHECK_EQ(c_rgb_8u_yu, c_bgr_8u_yd);
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, gaussian_y_8u_yu)
{
  constexpr int width = 800;
  constexpr int height = 600;

  ggo::image_t<ggo::pixel_type::y_8u> image({ width, height });

  ggo::fill_black(image);

  ggo::paint<ggo::sampling_4x4>(image,
    ggo::disc_f({ 0.5f * width, 0.5f * height }, 0.25f * std::min(width, height)), 0xff);

  ggo::paint<ggo::sampling_4x4>(image,
    ggo::disc_f({ 0.f, 0.f }, 0.25f * std::min(width, height)), 0xff);

  ggo::gaussian_blur(image, 5.f);

#ifdef GGO_BENCH
  ggo::chronometer chronometer;
  for (int i = 0; i < 1000; ++i)
  {
    ggo::gaussian_blur(image, 5.f);
  }
  std::cout << chronometer.get_display_time(true) << std::endl;
#else
  ggo::save_bmp("gaussian_blur_y_8u_yu.bmp", image);
#endif
}

