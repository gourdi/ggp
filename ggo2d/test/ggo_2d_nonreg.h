#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
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

  void * ptr = image.data();
  auto it = pixels.begin();
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      ggo::pixel_type_traits<pixel_type>::write(ggo::move_ptr(ptr, x * image.pixel_byte_size()), *it++);
    }
    ptr = ggo::move_ptr(ptr, image.line_byte_step());
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
auto make_image_t(int width, int height, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  ggo::image_t<pixel_type, memory_lines_order> image({ width, height });

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      image.write_pixel(x, y, fill_value);
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

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
auto make_image(int width, int height, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  ggo::image image({ width, height }, pixel_type, memory_lines_order);

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      void * ptr = image.pixel_ptr(x, y);

      ggo::pixel_type_traits<pixel_type>::write(ptr, fill_value);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, typename void_ptr_t, bool owns_buffer, typename data_t>
bool compare_images(const ggo::image_base_t<pixel_type, memory_lines_order, void_ptr_t, owns_buffer> & image, const ggo::array2d<data_t> & pixels)
{
  if (image.size() != pixels.size())
  {
    return false;
  }

  const void * ptr1 = image.data();
  const data_t * ptr2 = pixels.data();
  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      auto c1 = ggo::pixel_type_traits<pixel_type>::read(ggo::move_ptr(ptr1, x * image.pixel_byte_size()));
      auto c2 = ptr2[x];
      if (c1 != c2)
      {
        return false;
      }
    }

    ptr1 = ggo::move_ptr(ptr1, image.line_byte_step());
    ptr2 += pixels.width();
  }

  return true;
}

////////////////////////////////////////////////////////////////////
struct compare_images_functor
{
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, typename void_ptr_t, bool owns_buffer, typename data_t>
  static bool call(const ggo::image_base<void_ptr_t, owns_buffer> & image, const ggo::array2d<data_t> & pixels)
  {
    ggo::const_image_view_t<pixel_type, memory_lines_order> view(image.data(), image.size());

    return compare_images(view, pixels);
  }
};

////////////////////////////////////////////////////////////////////
template <typename void_ptr_t, bool owns_buffer, typename data_t>
bool compare_images(const ggo::image_base<void_ptr_t, owns_buffer> & image, const ggo::array2d<data_t> & pixels)
{
  return ggo::dispatch_image_format<compare_images_functor>(image.pixel_type(), image.memory_lines_order(), image, pixels);
}

////////////////////////////////////////////////////////////////////
#define GGO_CHECK_IMG(img1, img2) GGO_CHECK(compare_images(img1, img2));

