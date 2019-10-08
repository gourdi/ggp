#ifndef __GGO_2D_LOCAL_PROCESSING__
#define __GGO_2D_LOCAL_PROCESSING__

#include <kernel/math/signal_processing/ggo_local_processing.h>
#include <2d/ggo_image.h>

namespace ggo
{
  template <
    ggo::border_mode border_mode,
    ggo::pixel_type pt, ggo::lines_order lo, typename void_ptr_t1, bool ob1, typename void_ptr_t2, bool ob2, typename processing_t
  >
    void apply_horizontal_processing(
      const ggo::image_base_t<pt, lo, void_ptr_t1, ob1> & in, ggo::image_base_t<pt, lo, void_ptr_t2, ob2> & out,
      int processing_left_size, int processing_right_size, processing_t processing)
  {
    for (int y = 0; y < in.height(); ++y)
    {
      const void * input_ptr = ggo::move_ptr(in.data(), y * in.line_byte_step());
      void * output_ptr = ggo::move_ptr(out.data(), y * out.line_byte_step());

      ggo::input_pixel_type_iterator_t<pt> input_begin_it(input_ptr);
      ggo::input_pixel_type_iterator_t<pt> input_end_it = input_begin_it + in.width();
      ggo::output_pixel_type_iterator_t<pt> output_it(output_ptr);

      ggo::processing_1d<border_mode>(input_begin_it, input_end_it, output_it, processing_left_size, processing_right_size, processing);
    }
  }

  template <
    ggo::border_mode border_mode,
    ggo::pixel_type pt, ggo::lines_order lo, typename void_ptr_t1, bool ob1, typename void_ptr_t2, bool ob2,
    typename processing_up_t, typename processing_down_t
  >
    void apply_vertical_processing(
      const ggo::image_base_t<pt, lo, void_ptr_t1, ob1> & in, ggo::image_base_t<pt, lo, void_ptr_t2, ob2> & out,
      int processing_bottom_size, int processing_top_size, processing_up_t processing_up, processing_down_t processing_down)
  {
    for (int x = 0; x < in.width(); ++x)
    {
      const void * input_ptr = ggo::move_ptr(in.data(), x * ggo::pixel_type_traits<pt>::pixel_byte_size);
      void * output_ptr = ggo::move_ptr(out.data(), x * ggo::pixel_type_traits<pt>::pixel_byte_size);

      ggo::input_pixel_type_iterator<pt> input_begin_it(input_ptr, in.line_byte_step());
      ggo::input_pixel_type_iterator<pt> input_end_it = input_begin_it + in.height();
      ggo::output_pixel_type_iterator<pt> output_it(output_ptr, out.line_byte_step());

      if constexpr (lo == ggo::lines_order::up)
      {
        ggo::processing_1d<border_mode>(input_begin_it, input_end_it, output_it, processing_bottom_size, processing_top_size, processing_up);
      }
      else
      {
        ggo::processing_1d<border_mode>(input_begin_it, input_end_it, output_it, processing_top_size, processing_bottom_size, processing_down);
      }
    }
  }

  template <
    ggo::border_mode border_mode, ggo::pixel_type pt, ggo::lines_order lo, typename void_ptr_t1, bool ob1, typename void_ptr_t2, bool ob2, typename processing_t
  >
    void apply_vertical_symmetric_processing(
      const ggo::image_base_t<pt, lo, void_ptr_t1, ob1> & in, ggo::image_base_t<pt, lo, void_ptr_t2, ob2> & out,
      int processing_size, processing_t processing)
  {
    apply_vertical_processing<border_mode>(in, out, processing_size, processing_size, processing, processing);
  }
}

#endif
