#pragma once

#include <tuple>
#include <kernel/ggo_rect_int.h>

namespace ggo
{
  struct scan_rows_up
  {
    struct iterator
    {
      int _x;
      int _y;
      const rect_int _rect;

      bool operator!=(int y_end) const { return _y != y_end; }
      void operator++() { if (_x == _rect.right()) { _x = _rect.left(); ++_y; } else { ++_x; } }
      auto operator*() { return std::make_tuple(_x, _y); }
    };

    struct range
    {
      rect_int _rect;

      iterator begin() const { return iterator{ _rect.left(), _rect.bottom(), _rect }; }
      int end() const { return _rect.top() + 1; }
    };

    auto operator()(rect_int rect) const
    {
      return range{rect};
    }
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename process_tile_func>
  void scan_tiles_up(const ggo::rect_int & rect, int tile_width, int tile_height, process_tile_func process_tile)
  {
    int x = rect.left();
    int y = rect.bottom();

    int x_end = std::min(x + tile_width - 1, rect.right());
    int y_end = std::min(y + tile_height - 1, rect.top());

    while (true)
    {
      process_tile(rect_int::from_left_right_bottom_top(x, x_end, y, y_end));

      // Move to the next block.
      if (x_end < rect.right())
      {
        x += tile_width;
        x_end = std::min(x + tile_width - 1, rect.right());
      }
      else
      {
        GGO_ASSERT(x_end == rect.right());

        if (y_end == rect.top())
        {
          // Done.
          break;
        }
        else
        {
          // Go to next line.
          x = rect.left();
          x_end = std::min(x + tile_width - 1, rect.right());

          y += tile_height;
          y_end = std::min(y + tile_height - 1, rect.top());
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename process_tile_func>
  void scan_tiles_down(const ggo::rect_int & rect, int tile_width, int tile_height, process_tile_func process_tile)
  {
    int x = rect.left();
    int y = rect.top();

    int x_end = std::min(x + tile_width - 1, rect.right());
    int y_end = std::max(y - tile_height + 1, rect.bottom());

    while (true)
    {
      process_tile(rect_int::from_left_right_bottom_top(x, x_end, y_end, y));

      // Move to the next block.
      if (x_end < rect.right())
      {
        x += tile_width;
        x_end = std::min(x + tile_width - 1, rect.right());
      }
      else
      {
        GGO_ASSERT(x_end == rect.right());

        if (y_end == rect.bottom())
        {
          // Done.
          break;
        }
        else
        {
          // Go to next line.
          x = rect.left();
          x_end = std::min(x + tile_width - 1, rect.right());

          y -= tile_height;
          y_end = std::max(y - tile_height + 1, rect.bottom());
        }
      }
    }
  }
}


