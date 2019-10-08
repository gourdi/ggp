#ifndef __GGO_SCALING__
#define __GGO_SCALING__

#include <kernel/ggo_size.h>
#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <2d/ggo_image.h>

// Static images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t, bool in_owns_buffer, bool out_owns_buffer>
  void scale(const image_base_t<pt, lo, in_void_ptr_t, in_owns_buffer> & input, image_base_t<pt, lo, void *, out_owns_buffer> & output, ggo::scaling_algo algo)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;
    using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      auto c = ggo::pixel_type_traits<pt>::read(input.pixel_ptr(x, y));

      return ggo::convert_color_to<floating_point_color_t>(c);
    };

    auto out = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::pixel_type_traits<pt>::write(output.pixel_ptr(x, y), ggo::convert_color_to<color_t>(c));
    };

    switch (algo)
    {
    case ggo::scaling_algo::linear_integration:
      ggo::scale_2d<ggo::scaling_algo::linear_integration, ggo::scaling_algo::linear_integration, floating_point_color_t, float>(
        in, input.size().width(), input.size().height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_integration:
      ggo::scale_2d<ggo::scaling_algo::cubic_integration, ggo::scaling_algo::cubic_integration, floating_point_color_t, float>(
        in, input.size().width(), input.size().height(), out, output.width(), output.height());
      break;
    }
  }
}

// Dynamic images.
namespace ggo
{
  struct scaler
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t & input, output_image_t & output, ggo::scaling_algo algo)
    {
      const_image_view_t<pixel_type, memory_lines_order> input_view(input.data(), input.size());
      image_view_t<pixel_type, memory_lines_order> output_view(output.data(), output.size());

      scale(input_view, output_view, algo);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t, bool owns_buffer>
  image scale(const image_base<void_ptr_t, owns_buffer> & input, ggo::size output_size, ggo::scaling_algo algo)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler>(input.pixel_type(), input.memory_lines_order(), input, output, algo);

    return output;
  }
}

#endif
