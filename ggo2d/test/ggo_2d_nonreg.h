#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/ggo_image.h>

////////////////////////////////////////////////////////////////////
template <typename memory_layout_t, ggo::pixel_type pixel_type, size_t l>
auto make_image_aux_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[l])
{
  if (s.width() * s.height() != l)
  {
    throw std::runtime_error("invalid number of pixels");
  }

  ggo::image_t<pixel_type, memory_layout_t> image(s);

  int i = 0;
  for (int y = 0; y < s.height(); ++y)
  {
    for (int x = 0; x < s.width(); ++x)
    {
      void * ptr = ggo::move_ptr(image.data(), y * image.memory_layout()._line_byte_step + x * ggo::pixel_type_traits<pixel_type>::pixel_byte_size);

      ggo::pixel_type_traits<pixel_type>::write(ptr, coefs[i++]);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t l>
auto make_image_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[l])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::up>, pixel_type>(s, coefs);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t l>
auto make_image_rows_down_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[l])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::down>, pixel_type>(s, coefs);
}

////////////////////////////////////////////////////////////////////
template <typename memory_layout_t, ggo::pixel_type pixel_type>
auto make_image_aux_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  ggo::image_t<pixel_type, memory_layout_t> image(s);

  for (int y = 0; y < s.height(); ++y)
  {
    for (int x = 0; x < s.width(); ++x)
    {
      image.write_pixel(x, y, fill_value);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::up>, pixel_type>(s, fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_rows_down_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::down>, pixel_type>(s, fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_aux(std::unique_ptr<ggo::memory_layout> mem_layout, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  ggo::image image(pixel_type, std::move(mem_layout));

  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      ggo::write_pixel<pixel_type>(image, x, y, fill_value);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux<pixel_type>(std::make_unique<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::up>>(s), fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_rows_down(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux<pixel_type>(std::make_unique<ggo::rows_memory_layout<pixel_byte_size, ggo::vertical_direction::down>>(s), fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, 
  typename img1_memory_layout_t, typename img1_void_ptr_t,
  typename img2_memory_layout_t, typename img2_void_ptr_t>
bool compare_images(const ggo::image_base_t<pixel_type, img1_memory_layout_t, img1_void_ptr_t> & image1,
                    const ggo::image_base_t<pixel_type, img2_memory_layout_t, img2_void_ptr_t> & image2)
{
  if (image1.width() != image2.width() || image1.height() != image2.height())
  {
    return false;
  }

  for (int y = 0; y < image1.height(); ++y)
  {
    for (int x = 0; x < image1.width(); ++x)
    {
      if (image1.read_pixel(x, y) != image2.read_pixel(x, y))
      {
        return false;
      }
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
template <typename void_ptr_t, typename data_t>
struct compare_images_functor
{
  template <ggo::pixel_type pixel_type>
  static bool call(const ggo::image_base<void_ptr_t> & image, const ggo::array2<data_t> & pixels)
  {
    if (image.size().width() != pixels.dim(1) || image.size().height() != pixels.dim(0))
    {
      return false;
    }

    const data_t * ptr = pixels.data();
    for (int y = 0; y < image.height(); ++y)
    {
      for (int x = 0; x < image.width(); ++x)
      {
        auto c1 = ggo::read_pixel<pixel_type>(image, x, y);
        auto c2 = ptr[x];
        if (c1 != c2)
        {
          return false;
        }
      }

      ptr += pixels.width();
    }

    return true;
  }
};

////////////////////////////////////////////////////////////////////
template <typename void_ptr_t, typename data_t>
bool compare_images(const ggo::image_base<void_ptr_t> & image, const ggo::array2<data_t> & pixels)
{
  return ggo::dispatch_pixel_type<compare_images_functor<void_ptr_t, data_t>>(image.pixel_type(), image, pixels);
}

////////////////////////////////////////////////////////////////////
#define GGO_CHECK_IMG(img1, img2) GGO_CHECK(compare_images(img1, img2));

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, typename void_ptr_t, typename memory_layout_t, size_t l>
bool check_pixels(const ggo::image_base_t<pixel_type, memory_layout_t, void_ptr_t>& image,
  typename ggo::pixel_type_traits<pixel_type>::color_t const (&pixels)[l])
{
  if (image.size().width() * image.size().height() != l)
  {
    return false;
  }

  int i = 0;
  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      const void * ptr = ggo::move_ptr(image.data(), y * image.memory_layout()._line_byte_step + x * ggo::pixel_type_traits<pixel_type>::pixel_byte_size);

      auto pixel = ggo::pixel_type_traits<pixel_type>::read(ptr);

      if (pixel != pixels[i++])
      {
        return false;
      }
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
// Using __VA_ARGS__ because of https://stackoverflow.com/questions/20913103/is-it-possible-to-pass-a-brace-enclosed-initializer-as-a-macro-parameter
#define GGO_CHECK_PIXELS(img, ...) GGO_CHECK(check_pixels(img, __VA_ARGS__));

