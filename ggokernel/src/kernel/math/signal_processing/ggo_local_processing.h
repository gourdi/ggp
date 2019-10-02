#ifndef __GGO_LOCAL_PROCESSING__
#define __GGO_LOCAL_PROCESSING__

#include <kernel/ggo_borders.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/memory/ggo_iterator.h>

// 1D signal processing.
namespace ggo
{
  template <typename input_iterator_t, typename left_t, typename right_t, typename output_iterator_t, typename processing_t>
  void processing_1d(input_iterator_t begin, input_iterator_t end,
    left_t handle_left_border, right_t handle_right_border, output_iterator_t output,
    int processing_left_size, int processing_right_size, processing_t processing)
  {
    auto it = begin;

    {
      auto it_left_end = begin + processing_left_size;

      auto left_input = [&](std::ptrdiff_t delta)
      {
        auto pos = distance(begin, it) + delta;

        return pos < 0 ? handle_left_border(pos, begin, end) : it.get(delta);
      };

      for (; it != it_left_end; ++it)
      {
        output.set(processing(left_input));
        ++output;
      }
    }

    {
      auto it_inside_end = end - processing_right_size;

      auto center_input = [&](std::ptrdiff_t delta)
      {
        return it.get(delta);
      };

      for (; it != it_inside_end; ++it)
      {
        output.set(processing(center_input));
        ++output;
      }
    }

    {
      auto right_input = [&](std::ptrdiff_t delta)
      {
        auto pos = distance(begin, it) + delta;

        return pos >= distance(begin, end) ? handle_right_border(pos, begin, end) : it.get(delta);
      };

      for (; it != end; ++it)
      {
        output.set(processing(right_input));
        ++output;
      }
    }
  }

  template <border_mode border_mode, typename input_iterator_t, typename output_iterator_t, typename processing_t>
  void processing_1d(input_iterator_t begin, input_iterator_t end, output_iterator_t output,
    int processing_left_size, int processing_right_size, processing_t processing)
  {
    auto handle_border = [&](std::ptrdiff_t pos, auto begin, auto end)
    {
      if constexpr (border_mode == ggo::border_mode::zero)
      {
        return input_iterator_t::data_t(0);
      }
      else
      {
        pos = index<border_mode>(pos, distance(begin, end));
        return begin.get(pos);
      }
    };

    processing_1d(begin, end, handle_border, handle_border, output, processing_left_size, processing_right_size, processing);
  }

  template <border_mode border_mode, typename data_t, typename processing_t>
  void processing_1d(const data_t * begin, const data_t * end, data_t * output,
    int processing_left_size, int processing_right_size, processing_t processing)
  {
    processing_1d<border_mode>(ggo::input_iterator(begin, 1), ggo::input_iterator(end, 1),
      output_iterator(output, 1),
      processing_left_size, processing_right_size, processing);
  }
}

#endif

