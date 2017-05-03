#ifndef __GGO_MEMORY_LAYOUTS__
#define __GGO_MEMORY_LAYOUTS__

#include <ggo_kernel.h>
#include <ggo_memory.h>
#include <ggo_buffer_iterator.h>

// Typed buffers.
namespace ggo
{
  template <ggo::direction lines_order>
  struct lines_typed_memory_access
  {
    static_assert(lines_order == direction::up || lines_order == direction::down);

    static constexpr ggo::direction lines_direction = lines_order;

    template <typename data_t>
    static data_t * get_y_ptr(data_t * buffer, int y, int height, int line_byte_step)
    {
      GGO_ASSERT_PTR(buffer);
      GGO_ASSERT_BTW(y, 0, height - 1);
      return ptr_offset(buffer, (lines_order == direction::up ? y : height - y - 1) * line_byte_step);
    }

    template <typename data_t>
    static data_t * get_xy_ptr(data_t * buffer, int x, int y, int height, int line_byte_step)
    {
      GGO_ASSERT_BTW(x, 0, line_byte_step / sizeof(data_t) - 1);
      return get_y_ptr(buffer, y, height, line_byte_step) + x;
    }

    // Builds an iterator that will parse the row 'y' from 0 to positive x coordinate.
    template <typename data_accessor_t, typename data_t>
    static auto make_horizontal_iterator(data_t * ptr, int y, int height, int line_byte_step)
    {
      return typed_buffer_iterator<data_t, data_accessor_t>(get_y_ptr(ptr, y, height, line_byte_step));
    }

    // Builds an iterator that will move vertically from y = 0.
    template <typename data_accessor_t, typename data_t>
    static auto make_vertical_iterator(data_t * ptr, int x, int height, int line_byte_step)
    {
      ptr += x;

      if (lines_order == direction::up)
      {
        return item_byte_size_buffer_iterator<data_t, data_accessor_t>(ptr, line_byte_step);
      }
      else
      {
        ptr = ptr_offset(ptr, (height - 1) * line_byte_step);
        return item_byte_size_buffer_iterator<data_t, data_accessor_t>(ptr, -line_byte_step);
      }
    }
  };
}

// Raw buffers.
namespace ggo
{
  template <ggo::direction lines_order, int item_byte_step>
  struct lines_raw_memory_access
  {
    static_assert(lines_order == direction::up || lines_order == direction::down);
    static_assert(item_byte_step > 0);

    static constexpr ggo::direction lines_direction = lines_order;

    template <typename data_t>
    static data_t * get_y_ptr(data_t * buffer, int y, int height, int line_byte_step)
    {
      GGO_ASSERT_PTR(buffer);
      return ptr_offset(buffer, (lines_order == direction::up ? y : height - y - 1) * line_byte_step);
    }

    template <typename data_t>
    static data_t * get_xy_ptr(data_t * buffer, int x, int y, int height, int line_byte_step)
    {
      return ptr_offset(get_y_ptr(buffer, y, height, line_byte_step), x * item_byte_step);
    }

    // Builds an iterator that will parse the row 'y' from 0 to positive x coordinate.
    template <typename data_accessor_t, typename data_t>
    static auto make_horizontal_iterator(data_t * ptr, int y, int height, int line_byte_step)
    {
      return raw_buffer_iterator<item_byte_step, data_accessor_t, data_t>(get_y_ptr(ptr, y, height, line_byte_step));
    }

    template <typename data_accessor_t, typename data_t>
    static auto make_horizontal_const_iterator(const data_t * ptr, int y, int height, int line_byte_step)
    {
      return make_horizontal_iterator<data_accessor_t>(ptr, y, height, line_byte_step);
    }

    // Builds an iterator that will parse the column 'x' from 0 to positive y coordinate.
    template <typename data_accessor_t, typename data_t>
    static auto make_vertical_iterator(data_t * ptr, int x, int height, int line_byte_step)
    {
      ptr = ptr_offset(ptr, x * item_byte_step);

      if (lines_order == direction::up)
      {
        return item_byte_size_buffer_iterator<data_t, data_accessor_t>(ptr, line_byte_step);
      }
      else
      {
        ptr = ptr_offset(ptr, (height - 1) * line_byte_step);
        return item_byte_size_buffer_iterator<data_t, data_accessor_t>(ptr, -line_byte_step);
      }
    }

    template <typename data_accessor_t, typename data_t>
    static auto make_vertical_const_iterator(const data_t * ptr, int x, int height, int line_byte_step)
    {
      return make_vertical_iterator<data_accessor_t>(ptr, x, height, line_byte_step);
    }

    template <typename raw_buffer_t>
    static std::optional<raw_buffer_t> clip(const raw_buffer_t & buffer, const ggo::rect_int & clipping)
    {
      ggo::rect_int rect = ggo::rect_int::from_width_height(buffer._width, buffer._height);
      if (rect.clip(clipping) == false)
      {
        return {};
      }

      auto ptr = get_xy_ptr(buffer._buffer, rect.left(), lines_order == direction::up ? rect.bottom() : rect.top(), buffer._height, buffer._line_byte_step);

      return raw_buffer_t(rect.width(), rect.height(), buffer._line_byte_step, ptr);
    }
  };
}

#endif
