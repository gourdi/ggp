#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/rle_image/ggo_rle_image.h>
#include <2d/io/ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(rle_image, y8u)
{
  const int width = 15;
  const int height = 15;

  ggo::rle_image<uint8_t> rle_image(width, height);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      rle_image.set(x, y, y * width + x);
    }
  }

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      GGO_CHECK_EQ(rle_image.get(x, y), y * width + x);
    }
  }
}

