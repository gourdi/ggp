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


