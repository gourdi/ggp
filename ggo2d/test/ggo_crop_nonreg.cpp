#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_buffer.h>
#include <2d/processing/ggo_crop.h>


template <ggo::image_format format>
auto make_image(int width, int height, std::initializer_list<typename ggo::image_format_traits<format>::color_t> pixels)
{
  if (width * height != pixels.size())
  {
    throw std::runtime_error("invalid pixels count");
  }

  ggo::image image(width, height, format);

  auto it = pixels.begin();
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      ggo::write_pixel<format>(image.data(), x, y, height, image.line_byte_step(), *it++);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, y_8u_yu)
{
  auto image = make_image<ggo::y_8u_yu>(6, 5, {
    00, 01, 02, 03, 04, 05,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23, 24, 25,
    30, 31, 32, 33, 34, 35,
    40, 41, 42, 43, 44, 45 });

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 3);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 12);
    GGO_CHECK_EQ(ptr[1], 13);
    GGO_CHECK_EQ(ptr[2], 14);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 5);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 10);
    GGO_CHECK_EQ(ptr[1], 11);
    GGO_CHECK_EQ(ptr[2], 12);
    GGO_CHECK_EQ(ptr[3], 13);
    GGO_CHECK_EQ(ptr[4], 14);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 0, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 1);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 10);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(4, 10, 1, 2));
    GGO_CHECK_EQ(cropped.width(), 2);
    GGO_CHECK_EQ(cropped.height(), 2);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 14);
    GGO_CHECK_EQ(ptr[1], 15);
    GGO_CHECK_EQ(ptr[2], 24);
    GGO_CHECK_EQ(ptr[3], 25);
  }
}

