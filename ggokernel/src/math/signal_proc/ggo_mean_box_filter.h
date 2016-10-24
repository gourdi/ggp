#ifndef __GGO_MEAN_BOX_FILTER__
#define __GGO_MEAN_BOX_FILTER__

#include <ggo_circular_buffer.h>

/////////////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  template <typename iterator_t, typename left_t, typename right_t, typename count_t, typename divide_t>
  void mean_box_filter(iterator_t it, left_t left, right_t right, const count_t count, divide_t divide, const int box_half_size)
  {
    static_assert(std::is_integral<count_t>::value, "expecting integral type");

    using data_t = typename iterator_t::type;
   
    circular_buffer<data_t> cb(box_half_size + 1);

    data_t sum(0);

    for (int i = -box_half_size; i < 0; ++i)
    {
      const auto v = left(i);
      cb.push(v);
      sum += v;
    }

    for (int i = 0; i <= box_half_size; ++i)
    {
      sum += it.read(i);
    }

    cb.push(it.read());
    it.write(divide(sum));
    it.move_nxt();

    int i = 1;
    const int end = static_cast<int>(count) - box_half_size;
    for (; i < end; ++i)
    {
      sum -= cb.pop();
      sum += it.read(box_half_size);
      cb.push(it.read());
      it.write(divide(sum));
      it.move_nxt();
    }

    for (; i < count; ++i)
    {
      sum -= cb.pop();
      sum += right(i + box_half_size);
      it.write(divide(sum));
      it.move_nxt();
    }
  }
}

/////////////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply mean box filter horizontally.
  template <typename line_iterator_t, typename left_t, typename right_t, typename divide_t>
  void mean_box_filter_2d_horz(line_iterator_t line_iterator, left_t left, right_t right, const int width, const int height,
    divide_t divide, const int box_half_size)
  {
    for (int y = 0; y < height; ++y)
    {
      auto it = line_iterator(y);

      auto left_1d = [&](int x) { GGO_ASSERT(x < 0);  return left(x, y); };
      auto right_1d = [&](int x) { GGO_ASSERT(x >= width); return right(x, y); };

      mean_box_filter(it, left_1d, right_1d, width, divide, box_half_size);
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Apply mean box filter vertically.
  template <typename column_iterator_t, typename bottom_t, typename top_t, typename divide_t>
  void mean_box_filter_2d_vert(column_iterator_t column_iterator, bottom_t bottom, top_t top, int width, int height,
    divide_t divide, const int box_half_size)
  {
    for (int x = 0; x < width; ++x)
    {
      auto it = column_iterator(x);

      auto left_1d = [&](int y) { GGO_ASSERT(y < 0); return bottom(x, y); };
      auto right_1d = [&](int y) { GGO_ASSERT(y >= height); return top(x, y); };

      mean_box_filter(it, left_1d, right_1d, height, divide, box_half_size);
    }
  }
}

#endif
