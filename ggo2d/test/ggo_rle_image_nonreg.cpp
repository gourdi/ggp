#include "ggo_2d_nonreg.h"
#include <2d/rle_image/ggo_rle_image.h>
#include <2d/fill/ggo_fill.h>
#include <2d/processing/ggo_blit.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, y8u)
{
  constexpr int width = 15;
  constexpr int height = 15;

  ggo::rle_image<uint8_t> rle_image(width, height);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      rle_image.write_pixel(x, y, y * width + x);
    }
  }

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      GGO_CHECK_EQ(rle_image.read_pixel(x, y), y * width + x);
    }
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, y8u_fill)
{
  constexpr int width = 15;
  constexpr int height = 15;

  ggo::rle_image<uint8_t> image(width, height);

  ggo::fill_solid(image, 0x80);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      GGO_CHECK_EQ(image.read_pixel(x, y), 0x80);
    }
  }

  ggo::fill_black(image);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      GGO_CHECK_EQ(image.read_pixel(x, y), 0x00);
    }
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, y8u_blit)
{
  constexpr int width = 15;
  constexpr int height = 15;

  ggo::rle_image<uint8_t> dst(width, height);

  ggo::image_t<ggo::pixel_type::y_8u, ggo::lines_order::up> src({ width, height });
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      src.write_pixel(x, y, y * width + x);
    }
  }

  ggo::blit(src, dst);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      GGO_CHECK_EQ(dst.read_pixel(x, y), y * width + x);
    }
  }
}
