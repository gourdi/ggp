#ifndef __GGO_CONVOLUTION_2D__
#define __GGO_CONVOLUTION_2D__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/memory/ggo_edges_management.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>

/////////////////////////////////////////////////////////////////////
// Horizontal
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointers, with clipping.
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    GGO_ASSERT_LE(clipping.left(), clipping.right());
    GGO_ASSERT_LE(clipping.bottom(), clipping.top());

    GGO_ASSERT_LE(0, clipping.left());
    GGO_ASSERT_LE(0, clipping.bottom());

    GGO_ASSERT_LT(clipping.right(), size.width());
    GGO_ASSERT_LT(clipping.top(), size.height());

    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      const void * input_line  = ptr_offset(input, y * input_line_byte_step);
      void * output_line = ptr_offset(output, y * output_line_byte_step);

      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        auto v = read(ptr_offset(input_line, x * input_item_byte_step)) * kernel[0];

        for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
        {
          int left_i = handle_border_inf<border_mode>(x - i, size.width());
          auto left_v = read(ptr_offset(input_line, left_i * input_item_byte_step));

          int right_i = handle_border_sup<border_mode>(x + i, size.width());
          auto right_v = read(ptr_offset(input_line, right_i * input_item_byte_step));

          v += (left_v + right_v) * kernel[i];
        }

        write(ptr_offset(output_line, x * output_item_byte_step), v);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_horz<border_mode>(input, input_item_byte_step, input_line_byte_step, read,
      output, output_item_byte_step, output_line_byte_step, write,
      size, ggo::rect_int::from_size(size),
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_horz(const data_t * input, data_t * output,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    auto read = [](const void * ptr) { return *static_cast<const data_t *>(ptr); };
    auto write = [](void * ptr, data_t v) { *static_cast<data_t *>(ptr) = v; };

    apply_symetric_kernel_2d_horz<border_mode>(input, sizeof(data_t), sizeof(data_t) * size.width(), read,
      output, sizeof(data_t), sizeof(data_t) * size.width(), write,
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
    apply_symetric_kernel_2d_horz<border_mode>(input, output, size, ggo::rect_int::from_size(size), kernel, kernel_radius);
  }
}

/////////////////////////////////////////////////////////////////////
// Vertical
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Raw pointers, with clipping.
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    GGO_ASSERT_LE(clipping.left(), clipping.right());
    GGO_ASSERT_LE(clipping.bottom(), clipping.top());

    GGO_ASSERT_LE(0, clipping.left());
    GGO_ASSERT_LE(0, clipping.bottom());

    GGO_ASSERT_LT(clipping.right(), size.width());
    GGO_ASSERT_LT(clipping.top(), size.height());

    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        auto v = read(ptr_offset(input, y * input_line_byte_step + x * input_item_byte_step)) * kernel[0];

        for (int i = 1; i < static_cast<int>(kernel_radius); ++i)
        {
          int bottom_i = handle_border_inf<border_mode>(y - i, size.height());
          auto bottom_v = read(ptr_offset(input, bottom_i * input_line_byte_step + x * input_item_byte_step));

          int top_i = handle_border_sup<border_mode>(y + i, size.height());
          auto top_v = read(ptr_offset(input, top_i * input_line_byte_step + x * input_item_byte_step));

          v += (bottom_v + top_v) * kernel[i];
        }

        write(ptr_offset(output, y * output_line_byte_step + x * output_item_byte_step), v);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  // Raw pointers, without clipping (all data is processed).
  template <border_mode border_mode, typename read_t, typename write_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const void * input, int input_item_byte_step, int input_line_byte_step, read_t read,
    void * output, int output_item_byte_step, int output_line_byte_step, write_t write,
    const ggo::size & size,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    apply_symetric_kernel_2d_vert<border_mode>(input, input_item_byte_step, input_line_byte_step, read,
      output, output_item_byte_step, output_line_byte_step, write,
      size, ggo::rect_int::from_size(size),
      kernel, kernel_radius);
  }

  /////////////////////////////////////////////////////////////////////
  // Typed pointers, with clipping.
  template <border_mode border_mode, typename data_t, typename kernel_t, typename kernel_radius_t>
  void apply_symetric_kernel_2d_vert(const data_t * input, data_t * output,
    const ggo::size & size, const ggo::rect_int & clipping,
    const kernel_t * kernel, const kernel_radius_t kernel_radius)
  {
    auto read = [](const void * ptr) { return *static_cast<const data_t *>(ptr); };
    auto write = [](void * ptr, data_t v) { *static_cast<data_t *>(ptr) = v; };

    apply_symetric_kernel_2d_vert<border_mode>(input, sizeof(data_t), sizeof(data_t) * size.width(), read,
      output, sizeof(data_t), sizeof(data_t) * size.width(), write,
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
    apply_symetric_kernel_2d_vert<border_mode>(input, output, size, ggo::rect_int::from_size(size), kernel, kernel_radius);
  }
}

#endif