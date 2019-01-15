#ifndef __GGO_MEAN_BOX_FILTER_1D__
#define __GGO_MEAN_BOX_FILTER_1D__

#include <kernel/ggo_borders.h>

namespace ggo
{
  template <ggo::border_mode border, typename create_read_iterator_t, typename create_write_iterator_t>
  void mean_box_filter(create_read_iterator_t create_read_iterator, create_write_iterator_t create_write_iterator, int size, int left, int right, int box_radius)
  {
    using data_t = float;// std::result_of_t();
    int read_index = left - box_radius;
    data_t sum = 0;

    // Init the fifo and the sum.
    auto re
    for (; read_index < 0; ++read_index)
    {
      auto v = read(inf_index<border>(read_index, size));
      sum += v;
    }

    for (;; ++read_index)
    {
      auto v = read(read_index);
      sum += v;
    }

    // Process.
    int write_index = left;
    write(write_index, sum);

    for (; ; ++read_index, ++write_index)
    {
      sum -= read();
      sum += read();
      write(write_index, sum);
    }

    for (; ; ++read_index, ++write_index)
    {
      sum -= read();
      sum += read();
      write(write_index, sum);
    }
  }
}

#endif
