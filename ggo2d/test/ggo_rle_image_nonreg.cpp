#include "ggo_2d_nonreg.h"
#include <2d/rle_image/ggo_rle_image.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint_layer.h>
#include <2d/processing/ggo_blit.h>
#include <2d/processing/ggo_mask.h>

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
GGO_TEST(rle_image, fill)
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
GGO_TEST(rle_image, blit)
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

/////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, mask)
{
  ggo::rle_image<uint8_t> image(3, 2, 0xff, 1);
  auto mask = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 0x00, 0x10, 0x20 },
    { 0x40, 0xff, 0x80 } });

  ggo::apply_mask(image, mask, ggo::make_solid_brush(0x00_u8));

  GGO_CHECK_EQ(image.read_pixel(0, 0), 0xbf);
  GGO_CHECK_EQ(image.read_pixel(1, 0), 0x00);
  GGO_CHECK_EQ(image.read_pixel(2, 0), 0x7f);
  GGO_CHECK_EQ(image.read_pixel(0, 1), 0xff);
  GGO_CHECK_EQ(image.read_pixel(1, 1), 0xef);
  GGO_CHECK_EQ(image.read_pixel(2, 1), 0xdf);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, paint)
{
  ggo::rle_image<uint8_t> image(3, 2, 0x00, 1);

  ggo::paint<ggo::sampling_16x16>(image, ggo::disc_f({ 0.f, 1.f }, 1.5), 0xff_u8);

  GGO_CHECK_EQ(image.read_pixel(0, 0), 0xff);
  GGO_CHECK_EQ(image.read_pixel(1, 0), 0x61);
  GGO_CHECK_EQ(image.read_pixel(2, 0), 0x00);
  GGO_CHECK_EQ(image.read_pixel(0, 1), 0xff);
  GGO_CHECK_EQ(image.read_pixel(1, 1), 0x61);
  GGO_CHECK_EQ(image.read_pixel(2, 1), 0x00);
}

