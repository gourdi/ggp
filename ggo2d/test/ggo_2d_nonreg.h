#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/ggo_image.h>

////////////////////////////////////////////////////////////////////
template <ggo::image_format format>
auto make_image(int width, int height, std::initializer_list<typename ggo::image_format_traits<format>::color_t> pixels)
{
  if (width * height != pixels.size())
  {
    throw std::runtime_error("invalid pixels count");
  }

  ggo::image image({ width, height }, format);

  auto it = pixels.begin();
  for (int y = 0; y < height; ++y)
  {
    void * ptr = ggo::move_ptr(image.data(), y * image.line_byte_step());

    for (int x = 0; x < width; ++x)
    {
      ggo::write_pixel<format>(ptr, *it++);
      ptr = ggo::move_ptr<ggo::image_format_traits<format>::pixel_byte_size>(ptr);
    }
  }

  return image;
}

