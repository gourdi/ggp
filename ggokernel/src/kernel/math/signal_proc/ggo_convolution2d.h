#ifndef __GGO_CONVOLUTION_2D__
#define __GGO_CONVOLUTION_2D__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_data_traits.h>
#include <kernel/memory/ggo_edges_management.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <kernel/memory/ggo_sparse_buffer2d.h>

/////////////////////////////////////////////////////////////////////
// Horizontal
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointers, with clipping.
  template <memory_lines_order lines_order, border_mode border_mode, typename data_traits, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    GGO_ASSERT_LE(clipping.left(), clipping.right());
    GGO_ASSERT_LE(clipping.bottom(), clipping.top());

    GGO_ASSERT_LE(0, clipping.left());
    GGO_ASSERT_LE(0, clipping.bottom());

    GGO_ASSERT_LT(clipping.right(), size.width());
    GGO_ASSERT_LT(clipping.top(), size.height());

    // For now, only duplicated edge is supported when input == output. Other border modes could be handled
    // but it would requires too much work for now to properly read pixels.
    if (input == output && border_mode != border_mode::duplicate_edge)
    {
      throw std::runtime_error("only duplicate edge border mode is supported when in-place");
    }

    auto read = [](const void * ptr)
    {
      return data_traits::from_data_to_processing(data_traits::read(ptr));
    };

    auto write = [](void * ptr, const auto & v)
    {
      return (data_traits::write(ptr, data_traits::from_processing_to_data(v)));
    };

    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      const void * input_ptr  = get_line_ptr<lines_order>(input, y, size.height(), input_line_byte_step);
      void * output_ptr = get_line_ptr<lines_order>(output, y, size.height(), output_line_byte_step);

      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        auto v = read(ptr_offset(input_ptr, x * data_traits::input_item_byte_size)) * kernel[0];

        for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
        {
          int left_i = handle_border_inf<border_mode>(x - i, size.width());
          auto left_v = read(ptr_offset(input_ptr, left_i * data_traits::input_item_byte_size));

          int right_i = handle_border_sup<border_mode>(x + i, size.width());
          auto right_v = read(ptr_offset(input_ptr, right_i * data_traits::input_item_byte_size));

          v += (left_v + right_v) * kernel[i];
        }

        write(ptr_offset(output_ptr, x * data_traits::output_item_byte_size), v);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_horz<ggo::memory_lines_order::bottom_up, border_mode>(input, input_line_byte_step, read,
      output, output_line_byte_step, write,
      size, ggo::rect_int::from_size(size),
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <memory_lines_order lines_order, border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const data_t * input, data_t * output,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_horz<lines_order, border_mode, base_data_traits<data_t>>(
      input, sizeof(data_t) * size.width(),
      output, sizeof(data_t) * size.width(),
      size, clipping,
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const data_t * input, data_t * output,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_horz<ggo::memory_lines_order::bottom_up, border_mode>(
      input, output, size, ggo::rect_int::from_size(size), kernel, kernel_radius);
  }
}

/////////////////////////////////////////////////////////////////////
// Vertical
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointers, with clipping.
  template <memory_lines_order lines_order, border_mode border_mode, typename data_traits, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    GGO_ASSERT_LE(clipping.left(), clipping.right());
    GGO_ASSERT_LE(clipping.bottom(), clipping.top());

    GGO_ASSERT_LE(0, clipping.left());
    GGO_ASSERT_LE(0, clipping.bottom());

    GGO_ASSERT_LT(clipping.right(), size.width());
    GGO_ASSERT_LT(clipping.top(), size.height());

    // For now, only duplicated edge is supported when input == output. Other border modes could be handled
    // but it would requires too much work for now to properly read pixels.
    if (input == output && border_mode != border_mode::duplicate_edge)
    {
      throw std::runtime_error("only duplicate edge border mode is supported when in-place");
    }

    auto input_ptr = [&](int x, int y)
    {
      const void * ptr = get_line_ptr<lines_order>(input, y, size.height(), input_line_byte_step);
      ptr = ptr_offset(ptr, x * data_traits::input_item_byte_size);
      return ptr;
    };

    auto output_ptr = [&](int x, int y)
    {
      void * ptr = get_line_ptr<lines_order>(output, y, size.height(), output_line_byte_step);
      ptr = ptr_offset(ptr, x * data_traits::output_item_byte_size);
      return ptr;
    };

    auto read = [](const void * ptr)
    {
      return data_traits::from_data_to_processing(data_traits::read(ptr));
    };

    auto write = [](void * ptr, const auto & v)
    {
      return (data_traits::write(ptr, data_traits::from_processing_to_data(v)));
    };

    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        auto v = read(input_ptr(x, y)) * kernel[0];

        for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
        {
          int bottom_i = handle_border_inf<border_mode>(y - i, size.height());
          auto bottom_v = read(input_ptr(x, bottom_i));

          int top_i = handle_border_sup<border_mode>(y + i, size.height());
          auto top_v = read(input_ptr(x, top_i));

          v += (bottom_v + top_v) * kernel[i];
        }

        write(output_ptr(x, y), v);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_vert<memory_lines_order::bottom_up, border_mode, base_data_traits<data_t>>(
      input, input_line_byte_step,
      output, output_line_byte_step,
      size, ggo::rect_int::from_size(size),
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <memory_lines_order lines_order, border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const data_t * input, data_t * output,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_vert<lines_order, border_mode, base_data_traits<data_t>>(
      input, sizeof(data_t) * size.width(),
      output, sizeof(data_t) * size.width(),
      size, clipping,
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const data_t * input, data_t * output,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_vert<ggo::memory_lines_order::bottom_up, border_mode>(
      input, output, size, ggo::rect_int::from_size(size), kernel, kernel_radius);
  }
}

/////////////////////////////////////////////////////////////////////
// Horizontal & vertical
namespace ggo
{
  template <memory_lines_order lines_order, border_mode border_mode, typename data_traits, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    // Allocate temporary buffer.
    ggo::rect_int horz_rect = clipping;
    horz_rect.bottom() -= int(kernel_radius) - 1;
    horz_rect.top() += int(kernel_radius) - 1;
    horz_rect.clip(size);

    ggo::sparse_buffer2d<data_traits::data_t, ggo::memory_lines_order::bottom_up> temp(size, horz_rect);

    // Apply filter horizontally.
    {
      auto input_data = [](const void * ptr)
      {
        return data_traits::from_data_to_processing(data_traits::read(ptr));
      };

      for (int y = horz_rect.bottom(); y <= horz_rect.top(); ++y)
      {
        const void * input_ptr = get_line_ptr<lines_order>(input, y, size.height(), input_line_byte_step);
        auto * output_ptr = temp.get_line_ptr(y);

        for (int x = horz_rect.left(); x <= horz_rect.right(); ++x)
        {
          auto v = input_data(ptr_offset(input_ptr, x * data_traits::input_item_byte_size)) * kernel[0];

          for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
          {
            int left_i = handle_border_inf<border_mode>(x - i, size.width());
            auto left_v = input_data(ptr_offset(input_ptr, left_i * data_traits::input_item_byte_size));

            int right_i = handle_border_sup<border_mode>(x + i, size.width());
            auto right_v = input_data(ptr_offset(input_ptr, right_i * data_traits::input_item_byte_size));

            v += (left_v + right_v) * kernel[i];
          }

          output_ptr[x] = data_traits::from_processing_to_data(v);
        }
      }
    }

    // Vertical pass.
    {
      auto input_data = [&](int x, int y)
      {
        const data_traits::data_t * ptr = temp.get_line_ptr(y);
        return data_traits::from_data_to_processing(ptr[x]);
      };

      auto output_ptr = [&](int x, int y)
      {
        void * ptr = get_line_ptr<lines_order>(output, y, size.height(), output_line_byte_step);
        ptr = ptr_offset(ptr, x * data_traits::output_item_byte_size);
        return ptr;
      };

      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        for (int y = clipping.bottom(); y <= clipping.top(); ++y)
        {
          auto v = input_data(x, y) * kernel[0];

          for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
          {
            int bottom_i = handle_border_inf<border_mode>(y - i, size.height());
            auto bottom_v = input_data(x, bottom_i);

            int top_i = handle_border_sup<border_mode>(y + i, size.height());
            auto top_v = input_data(x, top_i);

            v += (bottom_v + top_v) * kernel[i];
          }

          data_traits::write(output_ptr(x, y), data_traits::from_processing_to_data(v));
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_traits, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d(const void * input, int input_line_byte_step,
    void * output, int output_line_byte_step,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d<memory_lines_order::bottom_up, border_mode, data_traits>(
      input, input_line_byte_step,
      output, output_line_byte_step,
      size, ggo::rect_int::from_size(size),
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <memory_lines_order lines_order, border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d(const data_t * input, data_t * output,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d<lines_order, border_mode, base_data_traits<data_t>>(
      input, sizeof(data_t) * size.width(),
      output, sizeof(data_t) * size.width(),
      size, clipping,
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d(const data_t * input, data_t * output,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d<ggo::memory_lines_order::bottom_up, border_mode>(
      input, output, size, ggo::rect_int::from_size(size), kernel, kernel_radius);
  }
}

#endif
