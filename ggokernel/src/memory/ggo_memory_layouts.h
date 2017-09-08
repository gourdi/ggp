#ifndef __GGO_MEMORY_LAYOUTS__
#define __GGO_MEMORY_LAYOUTS__

#include <ggo_kernel.h>
#include <ggo_memory.h>
#include <ggo_buffer_iterator.h>

namespace ggo
{
  template <ggo::direction lines_order, int item_byte_step>
  struct lines_memory_layout
  {
    static_assert(lines_order == direction::up || lines_order == direction::down);

    static constexpr ggo::direction lines_direction = lines_order;

    static void * get_y_ptr(void * buffer, int y, int height, int line_byte_step)
    {
      GGO_ASSERT_PTR(buffer);
      return ptr_offset(buffer, (lines_order == direction::up ? y : height - y - 1) * line_byte_step);
    }

    static const void * get_y_ptr(const void * buffer, int y, int height, int line_byte_step)
    {
      GGO_ASSERT_PTR(buffer);
      return ptr_offset(buffer, (lines_order == direction::up ? y : height - y - 1) * line_byte_step);
    }

    static void * get_xy_ptr(void * ptr, int x, int y, int height, int line_byte_step)
    {
      return ptr_offset(get_y_ptr(ptr, y, height, line_byte_step), item_byte_step * x);
    }

    static const void * get_xy_ptr(const void * ptr, int x, int y, int height, int line_byte_step)
    {
      return ptr_offset(get_y_ptr(ptr, y, height, line_byte_step), item_byte_step * x);
    }

    // Builds an input iterator that will parse the row 'y' from 0 to positive x coordinate.
    template <typename data_reader_t>
    static auto make_horizontal_read_only_iterator(const void * ptr, int y, int height, int line_byte_step)
    {
      return static_item_byte_step_buffer_read_only_iterator<item_byte_step, data_reader_t>(get_y_ptr(ptr, y, height, line_byte_step));
    }

    // Builds an output iterator that will parse the row 'y' from 0 to positive x coordinate.
    template <typename data_writer_t>
    static auto make_horizontal_write_only_iterator(void * ptr, int y, int height, int line_byte_step)
    {
      return static_item_byte_step_buffer_write_only_iterator<item_byte_step, data_writer_t>(get_y_ptr(ptr, y, height, line_byte_step));
    }

    // Builds an input iterator that will parse the column 'x' from 0 to positive y coordinate.
    template <typename data_reader_t>
    static auto make_vertical_read_only_iterator(const void * ptr, int x, int height, int line_byte_step)
    {
      ptr = ptr_offset(ptr, x * item_byte_step);

      if constexpr(lines_order == direction::up)
      {
        return dynamic_item_byte_size_buffer_read_only_iterator<data_reader_t>(ptr, line_byte_step);
      }
      else
      {
        ptr = ptr_offset(ptr, (height - 1) * line_byte_step);
        return dynamic_item_byte_size_buffer_read_only_iterator<data_reader_t>(ptr, -line_byte_step);
      }
    }

    // Builds an output iterator that will parse the column 'x' from 0 to positive y coordinate.
    template <typename data_writer_t>
    static auto make_vertical_write_only_iterator(void * ptr, int x, int height, int line_byte_step)
    {
      ptr = ptr_offset(ptr, x * item_byte_step);

      if constexpr(lines_order == direction::up)
      {
        return dynamic_item_byte_size_buffer_write_only_iterator<data_writer_t>(ptr, line_byte_step);
      }
      else
      {
        ptr = ptr_offset(ptr, (height - 1) * line_byte_step);
        return dynamic_item_byte_size_buffer_write_only_iterator<data_writer_t>(ptr, -line_byte_step);
      }
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
