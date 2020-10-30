#pragma once

#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

namespace ggo
{
  class memory_layout
  {
  public:

                          memory_layout(ggo::size s) : _size(s) {}

    virtual int           offset(int x, int y) const = 0;
    virtual int           buffer_byte_size() const = 0;

            void *        at(void * ptr, int x, int y) const { return move_ptr(ptr, offset(x, y)); }
            const void *  at(const void * ptr, int x, int y) const { return move_ptr(ptr, offset(x, y)); }

            const size &  size() const { return _size; }

  protected:

    ggo::size _size;
  };
}

namespace ggo
{
  enum class vertical_direction
  {
    up,
    down
  };

  template <int item_byte_size_, vertical_direction vdir_>
  struct rows_memory_layout : public memory_layout
  {
    static constexpr int item_byte_size = item_byte_size_;

    rows_memory_layout(ggo::size s) : memory_layout(s), _line_byte_step(item_byte_size * s.width()) {}
    rows_memory_layout(ggo::size s, int line_byte_step) : memory_layout(s), _line_byte_step(line_byte_step) {}

    template <typename void_ptr_t, typename func>
    void for_each(void_ptr_t ptr, rect_int rect, func && f) const
    {
      static_assert(std::is_same_v<void_ptr_t, void*> || std::is_same_v<void_ptr_t, const void*>);

      const int b = rect.bottom();
      const int t = rect.top();
      const int l = rect.left();
      const int r = rect.right();

      if constexpr (vdir_ == vertical_direction::up)
      {
        for (int y = b; y <= t; ++y)
        {
          void_ptr_t it = move_ptr(ptr, y * _line_byte_step + l * item_byte_size);

          for (int x = l; x <= r; ++x)
          {
            f(it, x, y);
            it = move_ptr<item_byte_size>(it);
          }
        }
      }
      else
      {
        for (int y = t; y >= b; ++y)
        {
          void_ptr_t it = move_ptr(ptr, (_size.height() - y - 1) * _line_byte_step + l * item_byte_size);

          for (int x = l; x <= r; ++x)
          {
            f(it, x, y);
            it = move_ptr<item_byte_size>(it);
          }
        }
      }
    }

    int offset(int x, int y) const override
    {
      if constexpr (vdir_ == vertical_direction::up)
      {
        return y * _line_byte_step + x * item_byte_size;
      }
      else
      {
        return (_size.height() - y - 1) * _line_byte_step + x * item_byte_size;
      }
    }

    int buffer_byte_size() const
    {
      return _line_byte_step * _size.height();
    }

    int _line_byte_step;
  };
}
