#ifndef __GGO_SCALING__
#define __GGO_SCALING__

#include <kernel/ggo_size.h>
#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <2d/ggo_image.h>

// Static images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t>
  void scale(const image_base_t<pt, lo, in_void_ptr_t> & input, image_t<pt, lo> & output, ggo::scaling_algo algo)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;
    using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;
    using floating_point_t = ggo::color_traits<floating_point_color_t>::sample_t;
    using scanner2d_t = image_base_t<pt, lo, in_void_ptr_t>::scanner2d_t;
    using tiling_t = image_base_t<pt, lo, in_void_ptr_t>::tiling_t;

    static_assert(std::is_floating_point_v<floating_point_t>);

    auto in = [&](int x, int y)
    {
      x = ggo::clamp(x, 0, input.width() - 1);
      y = ggo::clamp(y, 0, input.height() - 1);
      return ggo::convert_color_to<floating_point_color_t>(input.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t & c)
    {
      output.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    // Scaling algorithm dispatch.
    switch (algo)
    {
    case ggo::scaling_algo::nearest_neighbor:
      ggo::scale_2d<ggo::scaling_algo::nearest_neighbor, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_integration:
      ggo::scale_2d<ggo::scaling_algo::linear_integration, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_integration:
      ggo::scale_2d<ggo::scaling_algo::linear_integration, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_resampling_1:
      ggo::scale_2d<ggo::scaling_algo::linear_resampling_1, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_resampling_2x2:
      ggo::scale_2d<ggo::scaling_algo::linear_resampling_2x2, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_resampling_4x4:
      ggo::scale_2d<ggo::scaling_algo::linear_resampling_4x4, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_resampling_8x8:
      ggo::scale_2d<ggo::scaling_algo::linear_resampling_8x8, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::linear_resampling_16x16:
      ggo::scale_2d<ggo::scaling_algo::linear_resampling_16x16, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_resampling_1:
      ggo::scale_2d<ggo::scaling_algo::nearest_neighbor, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_resampling_2x2:
      ggo::scale_2d<ggo::scaling_algo::cubic_resampling_2x2, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_resampling_4x4:
      ggo::scale_2d<ggo::scaling_algo::cubic_resampling_4x4, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_resampling_8x8:
      ggo::scale_2d<ggo::scaling_algo::cubic_resampling_8x8, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    case ggo::scaling_algo::cubic_resampling_16x16:
      ggo::scale_2d<ggo::scaling_algo::cubic_resampling_16x16, scanner2d_t, tiling_t, floating_point_t>(
        in, input.width(), input.height(), out, output.width(), output.height());
      break;
    default:
      GGO_FAIL();
      break;
    }
  }
}

// Dynamic images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  struct scaler
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t & input, output_image_t & output, ggo::scaling_algo algo)
    {
      const_image_t<pixel_type, memory_lines_order> input_view(input.data(), input.size(), input.line_byte_step());
      image_t<pixel_type, memory_lines_order> output_view(output.data(), output.size(), output.line_byte_step());

      scale(input_view, output_view, algo);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t>
  image scale(const image_base<void_ptr_t> & input, ggo::size output_size, ggo::scaling_algo algo)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler>(input.pixel_type(), input.memory_lines_order(), input, output, algo);

    return output;
  }
}

#endif
