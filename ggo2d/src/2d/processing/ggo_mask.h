#ifndef __GGO_MASK__
#define __GGO_MASK__

#include <kernel/ggo_size.h>
#include <2d/ggo_image.h>

namespace ggo
{
  // Apply a mask on a brush.
  template <
    pixel_type image_pixel_type, lines_order image_memory_lines_order, bool image_owns_buffer,
    pixel_type mask_pixel_type, lines_order mask_memory_lines_order, typename mask_void_ptr_t, bool mask_owns_buffer,
    typename brush_t
  >
    void apply_mask(
      image_base_t<image_pixel_type, image_memory_lines_order, void *, image_owns_buffer> & image,
      const image_base_t<mask_pixel_type, mask_memory_lines_order, mask_void_ptr_t, mask_owns_buffer> & mask,
      ggo::size size,
      brush_t && brush)
  {
    using mask_color_t = typename pixel_type_traits<mask_pixel_type>::color_t;
    static_assert(color_traits<mask_color_t>::samples_count == 1);

    if (image.size() != mask.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < size.height(); ++y)
    {
      for (int x = 0; x < size.width(); ++x)
      {
        auto alpha = mask.read_pixel(x, y);
        auto c1 = brush(x, y);
        auto c2 = image.read_pixel(x, y);
        auto c3 = ggo::linerp(c1, c2, alpha);
        image.write_pixel(x, y, c3);
      }
    }
  }

  // Apply a mask on an image.
  template <
    pixel_type image_pixel_type, lines_order image_memory_lines_order, bool image_owns_buffer,
    pixel_type mask_pixel_type, lines_order mask_memory_lines_order, typename mask_void_ptr_t, bool mask_owns_buffer,
    pixel_type layer_pixel_type, lines_order layer_memory_lines_order, typename layer_void_ptr_t, bool layer_owns_buffer
  >
    void apply_mask(
      image_base_t<image_pixel_type, image_memory_lines_order, void *, image_owns_buffer> & image,
      const image_base_t<mask_pixel_type, mask_memory_lines_order, mask_void_ptr_t, mask_owns_buffer> & mask,
      const image_base_t<layer_pixel_type, layer_memory_lines_order, layer_void_ptr_t, layer_owns_buffer> & layer)
  {
    using mask_color_t = image_format_traits<mask_format>::color_t;
    static_assert(color_traits<mask_color_t>::samples_count == 1);

    if (image.size() != mask.size() || image.size() != layer.size())
    {
      throw std::runtime_error("size mismatch");
    }

    for (int y = 0; y < size.height(); ++y)
    {
      for (int x = 0; x < size.width(); ++x)
      {
        auto alpha = mask.read_pixel(x, y);
        auto c1 = layer.read_pixel(x, y);
        auto c2 = image.read_pixel(x, y);
        auto c3 = ggo::linerp(c1, c2, alpha);
        image.write_pixel(x, y, c3);
      }
    }
  }
}

#endif
