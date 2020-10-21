#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/ggo_image.h>

////////////////////////////////////////////////////////////////////
template <typename memory_layout_t, ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image_aux_t(typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{
  ggo::image_t<pixel_type, memory_layout_t> image({ w, h });

  for (int y = 0; y < h; ++y)
  {
    for (int x = 0; x < w; ++x)
    {
      image.write_pixel(x, y, coefs[y][x]);
    }
  }

  return image;
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image_t(typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::bottom_up_memory_layout<pixel_byte_size>, pixel_type>(coefs);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image_rows_down_t(typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::top_down_memory_layout<pixel_byte_size>, pixel_type>(coefs);
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

  return make_image_aux_t<ggo::bottom_up_memory_layout<pixel_byte_size>, pixel_type>(s, fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_rows_down_t(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image_aux_t<ggo::top_down_memory_layout<pixel_byte_size>, pixel_type>(s, fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image(std::unique_ptr<ggo::memory_layout> mem_layout, typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{  
  ggo::image image(pixel_type, std::move(mem_layout));

  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      ggo::write_pixel<pixel_type>(image, x, y, coefs[y][x]);
    }
  }

  return image;

}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image(typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image<pixel_type>(std::make_unique<ggo::bottom_up_memory_layout<pixel_byte_size>>(ggo::size(w, h)), coefs);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, size_t h, size_t w>
auto make_image_rows_down(typename ggo::pixel_type_traits<pixel_type>::color_t const (&coefs)[h][w])
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image<pixel_type>(std::make_unique<ggo::top_down_memory_layout<pixel_byte_size>>(ggo::size(w, h)), coefs);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image(std::unique_ptr<ggo::memory_layout> mem_layout, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
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

  return make_image<pixel_type>(std::make_unique<ggo::bottom_up_memory_layout<pixel_byte_size>>(s), fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type>
auto make_image_rows_down(ggo::size s, typename ggo::pixel_type_traits<pixel_type>::color_t fill_value)
{
  constexpr int pixel_byte_size = ggo::pixel_type_traits<pixel_type>::pixel_byte_size;

  return make_image<pixel_type>(std::make_unique<ggo::top_down_memory_layout<pixel_byte_size>>(s), fill_value);
}

////////////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, typename void_ptr_t, typename memory_layout_t, typename data_t>
bool compare_images(const ggo::image_base_t<pixel_type, memory_layout_t, void_ptr_t> & image, const ggo::array2<data_t> & pixels)
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
      auto c1 = image.read_pixel(x, y);
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

