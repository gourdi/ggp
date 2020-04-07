#pragma once

#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <2d/ggo_image.h>

// Static images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t>
  void scale_nearest_neighbor(const image_base_t<pt, lo, in_void_ptr_t> & input, image_t<pt, lo> & output)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;

    auto in = [&](int x, int y)
    {
      return input.read_pixel(x, y);
    };

    auto out = [&](int x, int y, const color_t & c)
    {
      output.write_pixel(x, y, c);
    };

    scale_2d_nearest_neighbor(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t>
  void scale_linear_interpolation(const image_base_t<pt, lo, in_void_ptr_t>& input, image_t<pt, lo>& output)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;
    using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      return ggo::convert_color_to<floating_point_color_t>(input.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t & c)
    {
      output.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_linear_interpolation(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t>
  void scale_cubic_interpolation(const image_base_t<pt, lo, in_void_ptr_t> & input, image_t<pt, lo> & output)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;
    using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      x = clamp(x, 0, input.width() - 1);
      y = clamp(y, 0, input.height() - 1);

      return ggo::convert_color_to<floating_point_color_t>(input.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t& c)
    {
      output.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_cubic_interpolation(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <pixel_type pt, lines_order lo, typename in_void_ptr_t>
  void scale_linear_integration(const image_base_t<pt, lo, in_void_ptr_t>& input, image_t<pt, lo>& output)
  {
    using color_t = ggo::pixel_type_traits<pt>::color_t;
    using floating_point_color_t = ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      return ggo::convert_color_to<floating_point_color_t>(input.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t& c)
    {
      output.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_linear_integration(in, input.width(), input.height(), out, output.width(), output.height());
  }
}

// Dynamic images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  struct scaler_nearest_neighbor
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t & input, output_image_t & output)
    {
      const_image_t<pixel_type, memory_lines_order> input_view(input.data(), input.size(), input.line_byte_step());
      image_t<pixel_type, memory_lines_order> output_view(output.data(), output.size(), output.line_byte_step());

      scale_nearest_neighbor(input_view, output_view);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t>
  image scale_nearest_neighbor(const image_base<void_ptr_t>& input, ggo::size output_size)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler_nearest_neighbor>(input.pixel_type(), input.memory_lines_order(), input, output, algo);

    return output;
  }

  //////////////////////////////////////////////////////////////
  struct scaler_linear_interpolation
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t& input, output_image_t& output)
    {
      const_image_t<pixel_type, memory_lines_order> input_view(input.data(), input.size(), input.line_byte_step());
      image_t<pixel_type, memory_lines_order> output_view(output.data(), output.size(), output.line_byte_step());

      scale_linear_interpolation(input_view, output_view);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t>
  image scale_linear_interpolation(const image_base<void_ptr_t>& input, ggo::size output_size)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler_linear_interpolation>(input.pixel_type(), input.memory_lines_order(), input, output);

    return output;
  }

  //////////////////////////////////////////////////////////////
  struct scaler_cubic_interpolation
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t& input, output_image_t& output)
    {
      const_image_t<pixel_type, memory_lines_order> input_view(input.data(), input.size(), input.line_byte_step());
      image_t<pixel_type, memory_lines_order> output_view(output.data(), output.size(), output.line_byte_step());

      scale_cubic_interpolation(input_view, output_view);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t>
  image scale_cubic_interpolation(const image_base<void_ptr_t>& input, ggo::size output_size)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler_cubic_interpolation>(input.pixel_type(), input.memory_lines_order(), input, output);

    return output;
  }

  //////////////////////////////////////////////////////////////
  struct scaler_linear_integration
  {
    template <pixel_type pixel_type, lines_order memory_lines_order, typename input_image_t, typename output_image_t>
    static void call(const input_image_t& input, output_image_t& output)
    {
      const_image_t<pixel_type, memory_lines_order> input_view(input.data(), input.size(), input.line_byte_step());
      image_t<pixel_type, memory_lines_order> output_view(output.data(), output.size(), output.line_byte_step());

      scale_linear_integration(input_view, output_view);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename void_ptr_t>
  image scale_linear_integration(const image_base<void_ptr_t>& input, ggo::size output_size)
  {
    image output(output_size, input.pixel_type(), input.memory_lines_order());

    dispatch_image_format<scaler_linear_integration>(input.pixel_type(), input.memory_lines_order(), input, output);

    return output;
  }
}

