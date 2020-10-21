#pragma once

#include <kernel/ggo_size.h>
#include <kernel/ggo_vec2.h>

namespace ggo
{
  // 1d
  template <typename dst_t, typename src_t>
  void blit(dst_t && dst, int dst_size, src_t && src, int src_size, int offset)
  {
    const int src_beg = std::max(0, -offset);
    const int dst_beg = std::max(0, offset);
    const int dst_end = offset + src_size < dst_size ? offset + src_size : dst_size;
    const int count = dst_end - dst_beg;

    for (int i = 0; i < count; ++i)
    {
      dst(dst_beg + i, src(src_beg + i));
    }
  }

  // 2d
  template <typename dst_t, typename src_t>
  void blit(dst_t && dst, ggo::size dst_size, src_t && src, ggo::size src_size, ggo::pos2_i offset)
  {
    const int src_beg_x = std::max(0, -offset.x());
    const int dst_beg_x = std::max(0, offset.x());
    const int dst_end_x = offset.x() + src_size.width() < dst_size.width() ? offset.x() + src_size.width() : dst_size.width();
    const int count_x = dst_end_x - dst_beg_x;

    const int src_beg_y = std::max(0, -offset.y());
    const int dst_beg_y = std::max(0, offset.y());
    const int dst_end_y = offset.y() + src_size.height() < dst_size.height() ? offset.y() + src_size.height() : dst_size.height();
    const int count_y = dst_end_y - dst_beg_y;

    for (int i_y = 0; i_y < count_y; ++i_y)
    {
      for (int i_x = 0; i_x < count_x; ++i_x)
      {
        dst(dst_beg_x + i_x, dst_beg_y + i_y, src(src_beg_x + i_x, src_beg_y + i_y));
      }
    }
  }
}

