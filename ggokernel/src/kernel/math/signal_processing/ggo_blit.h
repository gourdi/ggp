#pragma once

#include <kernel/ggo_size.h>
#include <kernel/ggo_vec2.h>
#include <kernel/scan/ggo_scan.h>

namespace ggo
{
  inline bool blit_params(int dst_size, int src_size, int offset, int & dst_start, int & src_start, int & count)
  {
    if (offset >= dst_size || -offset >= src_size)
    {
      return false;
    }

    src_start = std::max(0, -offset);
    dst_start = std::max(0, offset);
    count = (offset + src_size < dst_size ? offset + src_size : dst_size) - dst_start;

    return true;
  }

  // 1d
  template <typename dst_t, typename src_t>
  void blit(dst_t && dst, int dst_size, src_t && src, int src_size, int offset)
  {
    int dst_start = 0;
    int src_start = 0;
    int count = 0;

    if (blit_params(dst_size, src_size, offset, dst_start, src_start, count) == true)
    {
      for (int i = 0; i < count; ++i)
      {
        dst(dst_start + i, src(src_start + i));
      } 
    }
  }

  // 2d
  template <typename dst_t, typename src_t, typename scan_t>
  void blit(dst_t && dst, ggo::size dst_size, src_t && src, ggo::size src_size, ggo::pos2_i offset, scan_t scan)
  {
    int dst_start_x = 0;
    int src_start_x = 0;
    int count_x = 0;

    int dst_start_y = 0;
    int src_start_y = 0;
    int count_y = 0;

    if (blit_params(dst_size.width(),  src_size.width(),  offset.x(), dst_start_x, src_start_x, count_x) == true &&
        blit_params(dst_size.height(), src_size.height(), offset.y(), dst_start_y, src_start_y, count_y) == true)
    {
      for (auto [x, y] : scan(rect_int::from_left_right_bottom_top(0, count_x - 1, 0, count_y - 1)))
      {
        dst(dst_start_x + x, dst_start_y + y, src(src_start_x + x, src_start_y + y));
      }
    }
  }
}

