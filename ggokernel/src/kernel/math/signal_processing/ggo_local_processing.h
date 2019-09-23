#ifndef __GGO_LOCAL_PROCESSING__
#define __GGO_LOCAL_PROCESSING__

#include <kernel/ggo_borders.h>
#include <kernel/memory/ggo_array.h>

// 1D iterators.
namespace ggo
{
  template <typename data_t_>
  struct local_input_iterator
  {
    using data_t = typename data_t_;

    local_input_iterator(const data_t * ptr, std::ptrdiff_t offset) : _ptr(ptr), _offset(offset) {}

    const data_t * _ptr;
    const std::ptrdiff_t _offset;

    void operator++() { _ptr += _offset; }
    void operator--() { _ptr -= _offset; }
    local_input_iterator operator+(std::ptrdiff_t delta) { return local_input_iterator(_ptr + _offset * delta, _offset); }
    local_input_iterator operator-(std::ptrdiff_t delta) { return local_input_iterator(_ptr - _offset * delta, _offset); }
    bool operator==(const local_input_iterator & it) { return _ptr == it._ptr; }
    bool operator!=(const local_input_iterator & it) { return _ptr != it._ptr; }
    const data_t & operator()(std::ptrdiff_t delta) const { return _ptr[_offset * delta]; }
  };

  template <typename data_t>
  auto distance(const local_input_iterator<data_t> & it1, const local_input_iterator<data_t> & it2)
  {
    GGO_ASSERT_EQ(it1._offset, it2._offset);

    return (it2._ptr - it1._ptr) / it1._offset;
  }

  template <typename data_t>
  struct output_iterator
  {
    output_iterator(data_t * ptr, std::ptrdiff_t offset) : _ptr(ptr), _offset(offset) {}

    void operator()(const data_t & v) { *_ptr = v; _ptr += _offset; }

    data_t * _ptr;
    const std::ptrdiff_t _offset;
  };
}

// 1D signal processing.
namespace ggo
{
  template <typename input_iterator_t, typename left_t, typename right_t, typename output_t, typename processing_t>
  void processing_1d(input_iterator_t begin, input_iterator_t end,
    left_t handle_left_border, right_t handle_right_border, output_t output,
    int processing_size, processing_t processing)
  {
    GGO_ASSERT(ggo::is_odd(processing_size));

    auto it = begin;

    {
      auto it_left_end = begin + processing_size / 2;

      auto left_input = [&](std::ptrdiff_t delta)
      {
        auto pos = distance(begin, it) + delta;

        return pos < 0 ? handle_left_border(pos, begin, end) : it(delta);
      };

      for (; it != it_left_end; ++it)
      {
        output(processing(left_input));
      }
    }

    {
      auto it_inside_end = end - processing_size / 2;

      for (; it != it_inside_end; ++it)
      {
        output(processing(it));
      }
    }

    {
      auto right_input = [&](std::ptrdiff_t delta)
      {
        auto pos = distance(begin, it) + delta;

        return pos >= distance(begin, end) ? handle_right_border(pos, begin, end) : it(delta);
      };

      for (; it != end; ++it)
      {
        output(processing(right_input));
      }
    }
  }

  template <border_mode border_mode, typename input_iterator_t, typename output_t, typename processing_t>
  void processing_1d(input_iterator_t begin, input_iterator_t end, output_t output,
    int processing_size, processing_t processing)
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
        return begin(pos);
      }
    };

    processing_1d(begin, end, handle_border, handle_border, output, processing_size, processing);
  }

  template <border_mode border_mode, typename data_t, typename processing_t>
  void processing_1d(const data_t * begin, const data_t * end, data_t * output,
    int processing_size, processing_t processing)
  {
    processing_1d<border_mode>(ggo::local_input_iterator(begin, 1), ggo::local_input_iterator(end, 1),
      output_iterator(output, 1),
      processing_size, processing);
  }
}

// 1D  processings on 2D signals.
namespace ggo
{
  template <typename data_t>
  auto make_input_line_iterator(const ggo::array_2<data_t> & image, int y)
  {
    return local_input_iterator<data_t>(image.data() + y * image.width(), 1);
  }

  template <typename data_t>
  auto make_input_column_iterator(const ggo::array_2<data_t> & image, int x)
  {
    return local_input_iterator<data_t>(image.data() + x, image.width());
  }

  template <typename data_t>
  auto make_output_line_iterator(ggo::array_2<data_t> & image, int y)
  {
    return output_iterator(image.data() + y * image.width(), 1);
  }

  template <typename data_t>
  auto make_output_column_iterator(ggo::array_2<data_t> & image, int x)
  {
    return output_iterator(image.data() + x, image.width());
  }

  template <typename input_image_t, typename left_t, typename right_t, typename output_image_t, typename processing_t>
  void apply_horizontal_processing(const input_image_t & input,
    left_t handle_left_border, right_t handle_right_border,
    output_image_t & output,
    int processing_size, processing_t processing)
  {
    GGO_ASSERT_EQ(input.size(), output.size());

    for (int y = 0; y < input.height(); ++y)
    {
      auto input_begin = make_input_line_iterator(input, y);
      auto input_end = input_begin + input.width();
      auto output_it = make_output_line_iterator(output, y);
      ggo::processing_1d(input_begin, input_end, handle_left_border, handle_right_border, output_it, processing_size, processing);
    }
  }

  template <typename input_image_t, typename bottom_t, typename top_t, typename output_image_t, typename processing_t>
  void apply_vertical_processing(const input_image_t & input,
    bottom_t handle_bottom_border, top_t handle_top_border,
    output_image_t & output,
    int processing_size, processing_t processing)
  {
    GGO_ASSERT_EQ(input.size(), output.size());

    for (int x = 0; x < input.width(); ++x)
    {
      auto input_begin = make_input_column_iterator(input, x);
      auto input_end = input_begin + input.height();
      auto output_it = make_output_column_iterator(output, x);
      ggo::processing_1d(input_begin, input_end, handle_bottom_border, handle_top_border, output_it, processing_size, processing);
    }
  }

  template <border_mode border_mode, typename input_image_t, typename output_image_t, typename processing_t>
  void apply_horizontal_processing(const input_image_t & input, output_image_t & output,
    int processing_size, processing_t processing)
  {
    auto handle_border = [&](std::ptrdiff_t pos, auto begin, auto end)
    {
      if constexpr (border_mode == ggo::border_mode::zero)
      {
        return input_image_t::data_t(0);
      }
      else
      {
        pos = index<border_mode>(pos, distance(begin, end));
        return begin(pos);
      }
    };

    apply_horizontal_processing(input, handle_border, handle_border, output, processing_size, processing);
  }

  template <border_mode border_mode, typename input_image_t, typename output_image_t, typename processing_t>
  void apply_vertical_processing(const input_image_t & input, output_image_t & output,
    int processing_size, processing_t processing)
  {
    auto handle_border = [&](std::ptrdiff_t pos, auto begin, auto end)
    {
      if constexpr (border_mode == ggo::border_mode::zero)
      {
        return input_image_t::data_t(0);
      }
      else
      {
        pos = index<border_mode>(pos, distance(begin, end));
        return begin(pos);
      }
    };

    apply_vertical_processing(input, handle_border, handle_border, output, processing_size, processing);
  }
}

#endif

