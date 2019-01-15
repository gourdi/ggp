#ifndef __GGO_MEAN_BOX_FILTER_2D__
#define __GGO_MEAN_BOX_FILTER_2D__

#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_borders.h>
#include <kernel/memory/ggo_circular_buffer.h>

/////////////////////////////////////////////////////////////////////
// Horizontal.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointer, with clipping.
  template <border_mode border_mode, typename read_t, typename write_t, typename divide_t, typename radius_t>
  void mean_box_filter_2d_horz(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size, const ggo::rect_int & clipping,
    divide_t divide, radius_t box_radius)
  {
    static_assert(std::is_integral<radius_t>::value);

    // For now, only duplicated edge is supported when input == output. Other border modes could be handled
    // but it would requires too much work for now to properly read pixels.
    if (input == output && border_mode != border_mode::duplicate_edge)
    {
      throw std::runtime_error("only duplicate edge border mode is supported when in-place");
    }

    using data_t = std::result_of<read_t(const void *)>::type;

    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      const void * input_line = ptr_offset(input, y * input_line_byte_step);
      void * output_line = ptr_offset(output, y * output_line_byte_step);

      circular_buffer<data_t> cb(box_radius + 1);

      data_t sum(0);

      for (int x = -box_radius; x <= 0; ++x)
      {
        int i = inf_index<border_mode>(clipping.left() + x, size.width());
        const auto v = read(ptr_offset(input_line, i * input_item_byte_step));
        cb.push(v);
        sum += v;
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "reading " << v << " at " << i << " and pushing it in the circular buffer: " << cb << ", sum is " << sum << std::endl;
#endif
      }

      for (int x = 1; x <= box_radius; ++x)
      {
        int i = clipping.left() + x;
        const auto v = read(ptr_offset(input_line, i * input_item_byte_step));
        sum += v;
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "reading " << v << " at " << x << ", sum is " << sum << std::endl;
#endif
      }

      const auto v = divide(sum);
      write(ptr_offset(output_line, clipping.left() * output_item_byte_step), v);
#ifdef DEBUG_MEAN_BOX_FILTER
      std::cout << "writing " << v << " at " << clipping.left() << std::endl << std::endl;
#endif

      for (int x = clipping.left() + 1; x <= clipping.right(); ++x)
      {
        int i = sup_index<border_mode>(x + box_radius, size.width());

        const auto v1 = cb.pop();
        sum -= v1;
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "processing x=" << x << std::endl;
        std::cout << "popping " << v1 << " from circular buffer: " << cb << ", sum is " << sum << std::endl;
#endif

        const auto v2 = read(ptr_offset(input_line, x * input_item_byte_step));
        cb.push(v2);
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "reading " << v2 << " at " << x << " and pushing it in the circular buffer: " << cb << ", sum is " << sum << std::endl;
#endif

        const auto v3 = read(ptr_offset(input_line, i * input_item_byte_step));
        sum += v3;
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "reading " << v3 << " at " << i << ", sum is " << sum << std::endl;
#endif

        const auto v4 = divide(sum);
        write(ptr_offset(output_line, x * output_item_byte_step), v4);
#ifdef DEBUG_MEAN_BOX_FILTER
        std::cout << "writing " << v4 << " at " << x << std::endl << std::endl;
#endif
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename divide_t, typename radius_t>
  void mean_box_filter_2d_horz(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size,
    divide_t divide, radius_t box_radius)
  {
    mean_box_filter_2d_horz<border_mode>(input, input_item_byte_step, input_line_byte_step, read,
      output, output_item_byte_step, output_line_byte_step, write,
      size, ggo::rect_int::from_size(size),
      divide, box_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <border_mode border_mode, typename data_t, typename radius_t>
  void mean_box_filter_2d_horz(const data_t * input, data_t * output, const ggo::size & size, const ggo::rect_int & clipping, radius_t box_radius)
  {
    const data_t inv = 1 / data_t(2 * box_radius + 1);

    auto read   = [](const void * ptr) { return *static_cast<const data_t *>(ptr); };
    auto write  = [](void * ptr, data_t v) { *static_cast<data_t *>(ptr) = v; };
    auto divide = [inv](data_t v) { return v * inv; };

    mean_box_filter_2d_horz<border_mode>(input, sizeof(data_t), sizeof(data_t) * size.width(), read,
      output, sizeof(data_t), sizeof(data_t) * size.width(), write,
      size, clipping,
      divide, box_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_t, typename radius_t>
  void mean_box_filter_2d_horz(const data_t * input, data_t * output, const ggo::size & size, radius_t box_radius)
  {
    mean_box_filter_2d_horz<border_mode>(input, output, size, ggo::rect_int::from_size(size), box_radius);
  }
}

/////////////////////////////////////////////////////////////////////
// Vertical.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointers, with clipping.
  template <border_mode border_mode, typename read_t, typename write_t, typename divide_t, typename radius_t>
  void mean_box_filter_2d_vert(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size, const ggo::rect_int & clipping,
    divide_t divide, radius_t box_radius)
  {
    auto flipped_clipping = ggo::rect_int::from_left_right_bottom_top(clipping.bottom(), clipping.top(), clipping.left(), clipping.right());

    mean_box_filter_2d_horz<border_mode>(input, input_line_byte_step, input_item_byte_step, read,
      output, output_line_byte_step, output_item_byte_step, write,
      { size.height(), size.width() }, flipped_clipping, divide, box_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename divide_t, typename radius_t>
  void mean_box_filter_2d_vert(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size,
    divide_t divide, radius_t box_radius)
  {
    mean_box_filter_2d_vert<border_mode>(input, input_item_byte_step, input_line_byte_step, read,
      output, output_item_byte_step, output_line_byte_step, write,
      size, ggo::rect_int::from_size(size),
      divide, box_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <border_mode border_mode, typename data_t, typename radius_t>
  void mean_box_filter_2d_vert(const data_t * input, data_t * output, const ggo::size & size, const ggo::rect_int & clipping, radius_t box_radius)
  {
    const data_t inv = 1 / data_t(2 * box_radius + 1);

    auto read   = [](const void * ptr) { return *static_cast<const data_t *>(ptr); };
    auto write  = [](void * ptr, data_t v) { *static_cast<data_t *>(ptr) = v; };
    auto divide = [inv](data_t v) { return v * inv; };

    mean_box_filter_2d_vert<border_mode>(input, sizeof(data_t), sizeof(data_t) * size.width(), read,
      output, sizeof(data_t), sizeof(data_t) * size.width(), write,
      size, clipping,
      divide, box_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_t, typename radius_t>
  void mean_box_filter_2d_vert(const data_t * input, data_t * output, const ggo::size & size, radius_t box_radius)
  {
    mean_box_filter_2d_vert<border_mode>(input, output, size, ggo::rect_int::from_size(size), box_radius);
  }
}

#endif
