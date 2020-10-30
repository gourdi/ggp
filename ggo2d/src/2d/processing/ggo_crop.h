#pragma once

#include <2d/ggo_image.h>
#include <kernel/ggo_rect_int.h>

// Static generic images.
namespace ggo
{
  template <typename input_image_t, typename output_image_t, typename scan_t>
  void crop_t(const input_image_t & input_image, ggo::pos2_i offset, output_image_t & output_image, scan_t scan)
  {
    ggo::rect_int r = ggo::rect_int::from_left_width_bottom_height(offset.x(), output_image.width(), offset.y(), output_image.height());

    if (r.clip(input_image.size()) == false || r.size() != output_image.size())
    {
      throw std::runtime_error("invalid crop parameters");
    }

    for (auto [x, y] : scan(r))
    {
      output_image.write_pixel(x, y, input_image.read(x + bottom_left.x(), y + bottom_left().y()));
    }
  }
}

// Static rows memory images.
namespace ggo
{
  template <pixel_type img_pixel_type, vertical_direction rows_vdir, typename void_ptr_t>
  auto crop_t(const ggo::image_base_t<img_pixel_type, rows_memory_layout<pixel_type_traits<img_pixel_type>::pixel_byte_size, rows_vdir>, void_ptr_t> & src, ggo::rect_int rect)
  {
    if (rect.clip(src.size()) == false)
    {
      throw std::runtime_error("invalid crop parameter");
    }

    ggo::image_base_t<img_pixel_type, rows_memory_layout<pixel_type_traits<img_pixel_type>::pixel_byte_size, rows_vdir>, void *> dst(rect.size());

    int src_line_byte_step = src.memory_layout()._line_byte_step;
    int dst_line_byte_step = dst.memory_layout()._line_byte_step;

    const void * src_ptr = nullptr;
    void * dst_ptr = dst.data();

    if constexpr (rows_vdir == vertical_direction::up)
    {
      src_ptr = move_ptr(src.data(), rect.bottom() * src_line_byte_step + rect.left() * src.pixel_byte_size());
    }
    else
    {
      static_assert(rows_vdir == vertical_direction::down);
      src_ptr = move_ptr(src.data(), (src.height() - rect.top() - 1) * src_line_byte_step + rect.left() * src.pixel_byte_size());
    }

    int count = rect.height();
    for (int i = 0; i < count; ++i)
    {
      memcpy(dst_ptr, src_ptr, dst_line_byte_step);
      src_ptr = move_ptr(src_ptr, src_line_byte_step);
      dst_ptr = move_ptr(dst_ptr, dst_line_byte_step);
    }

    return dst;
  }
}


// Dynamic images.
namespace ggo
{
  template <typename input_image_t, typename output_image_t, typename scan_t>
  struct crop_dispatch
  {
    template <pixel_type img_pixel_type>
    static void call()
    {
      ggo::rect_int r = ggo::rect_int::from_left_width_bottom_height(offset.x(), output_image.width(), offset.y(), output_image.height());

      if (r.clip(input_image.size()) == false || r.size() != output_image.size())
      {
        throw std::runtime_error("invalid crop parameters");
      }

      for (auto [x, y] : scan(r))
      {
        auto c = ggo::read_pixel<img_pixel_type>(input_image, x, y);
        ggo::write_pixel<img_pixel_type>(output_image, x, y, c);
      }
    }
  };

  template <typename input_image_t, typename output_image_t, typename scan_t>
  void crop(const input_image_t & input_image, ggo::pos2_i offset, output_image_t & output_image, scan_t scan)
  {
    if (input_image.pixel_type() != output_image.pixel_type())
    {
      throw std::runtime_error("pixel type pismatch");
    }

    dispatch_pixel_type<dispatch_src<input_image_t, output_image_t>>(src_image.pixel_type(), input_image, offset, output_image, scan);
  }
}

