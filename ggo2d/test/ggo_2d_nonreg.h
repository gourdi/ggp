#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/ggo_image.h>

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
auto make_image_t(int width, int height, std::initializer_list<typename ggo::pixel_type_traits<pixel_type>::color_t> pixels)
{
  if (width * height != pixels.size())
  {
    throw std::runtime_error("invalid pixels count");
  }

  ggo::image_t<pixel_type, memory_lines_order> image({ width, height });

  auto it = pixels.begin();
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      image.write_pixel(x, y, *it++);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
auto make_image(int width, int height, std::initializer_list<typename ggo::pixel_type_traits<pixel_type>::color_t> pixels)
{
  if (width * height != pixels.size())
  {
    throw std::runtime_error("invalid pixels count");
  }

  ggo::image image({ width, height }, pixel_type, memory_lines_order);

  auto it = pixels.begin();
  for (int y = 0; y < height; ++y)
  {
    void * ptr = ggo::move_ptr(image.data(), y * image.line_byte_step());

    for (int x = 0; x < width; ++x)
    {
      ggo::pixel_type_traits<pixel_type>::write(ptr, *it++);
      ptr = ggo::move_ptr<ggo::pixel_type_traits<pixel_type>::pixel_byte_size>(ptr);
    }
  }

  return image;
}

