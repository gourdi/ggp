#pragma once

#include <kernel/math/signal_processing/ggo_scale2d.h>

// Static images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_nearest_neighbor_t(const input_image_t & input_image, output_image_t & output_image)
  {
    static_assert(std::is_same_v<input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename output_image_t::color_t;

    auto in  = [&](int x, int y) { return input_image.read_pixel(x, y); };
    auto out = [&](int x, int y, const color_t & c) { output_image.write_pixel(x, y, c); };

    scale_2d_nearest_neighbor(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_linear_interpolation_t(const input_image_t & input_image, output_image_t & output_image)
  {
    static_assert(std::is_same_v<input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename output_image_t::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      return ggo::convert_color_to<floating_point_color_t>(input_image.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t & c)
    {
      output_image.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_linear_interpolation(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_cubic_interpolation_t(const input_image_t & input_image, output_image_t & output_image)
  {
    static_assert(std::is_same_v<input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename output_image_t::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      x = clamp(x, 0, input.width() - 1);
      y = clamp(y, 0, input.height() - 1);

      return ggo::convert_color_to<floating_point_color_t>(input_image.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t& c)
    {
      output_image.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_cubic_interpolation(in, input.width(), input.height(), out, output.width(), output.height());
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_linear_integration_t(const input_image_t & input_image, output_image_t & output_image)
  {
    static_assert(std::is_same_v<input_image_t::color_t, output_image_t::color_t>);

    using color_t = typename output_image_t::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

    auto in = [&](int x, int y)
    {
      return ggo::convert_color_to<floating_point_color_t>(input_image.read_pixel(x, y));
    };

    auto out = [&](int x, int y, const floating_point_color_t& c)
    {
      output_image.write_pixel(x, y, ggo::convert_color_to<color_t>(c));
    };

    scale_2d_linear_integration(in, input.width(), input.height(), out, output.width(), output.height());
  }
}

// Dynamic images.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  struct scaler_nearest_neighbor
  {
    template <pixel_type img_pixel_type>
    static void call(const input_image_t & input_image, output_image_t & output_image)
    {
      using color_t = typename pixel_type_traits<img_pixel_type>::color_t;

      auto in  = [](int x, int y) { return read_pixel<img_pixel_type>(input_image, x, y); };
      auto out = [](int x, int y, const color_t & c) { return write_pixel<img_pixel_type>(x, y, c); };

      scale_nearest_neighbor(in, out);
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_nearest_neighbor(const input_image_t & input_image, output_image_t & output_image)
  {
    if (input_image.pixel_type() != output_image.pixel_type())
    {
      throw std::runtime_error("pixel_type mismatch");
    }

    dispatch_image_format<scaler_nearest_neighbor<input_image_t, output_image_t>>(input_image.pixel_type(), input_image, output_image);
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  struct scaler_linear_interpolation
  {
    template <pixel_type img_pixel_type>
    static void call(const input_image_t & input_image, output_image_t & output_image)
    {
      using color_t = typename pixel_type_traits<img_pixel_type>::color_t;
      using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

      auto in = [&](int x, int y)
      {
        return ggo::convert_color_to<floating_point_color_t>(read_pixel<img_pixel_type>(input_image, x, y));
      };

      auto out = [&](int x, int y, const floating_point_color_t & c)
      {
        write_pixel<img_pixel_type>(output_image, x, y, ggo::convert_color_to<color_t>(c));
      };

      scale_2d_linear_interpolation(in, input_image.width(), input_image.height(), out, output_image.width(), output_image.height());
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_linear_interpolation(const input_image_t & input_image, output_image_t & output_image)
  {
    if (input_image.pixel_type() != output_image.pixel_type())
    {
      throw std::runtime_error("pixel_type mismatch");
    }

    dispatch_pixel_type<scaler_linear_interpolation<input_image_t, output_image_t>>(input_image.pixel_type(), input_image, output_image);
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  struct scaler_cubic_interpolation
  {
    template <pixel_type img_pixel_type>
    static void call(const input_image_t & input_image, output_image_t & output_image)
    {
      using color_t = typename pixel_type_traits<img_pixel_type>::color_t;
      using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

      auto in = [&](int x, int y)
      {
        x = clamp(x, 0, input_image.width() - 1);
        y = clamp(y, 0, input_image.height() - 1);

        return ggo::convert_color_to<floating_point_color_t>(ggo::read_pixel<img_pixel_type>(input_image, x, y));
      };

      auto out = [&](int x, int y, const floating_point_color_t& c)
      {
        ggo::write_pixel<img_pixel_type>(output_image, x, y, ggo::convert_color_to<color_t>(c));
      };

      scale_2d_cubic_interpolation(in, input_image.width(), input_image.height(), out, output_image.width(), output_image.height());
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_cubic_interpolation(const input_image_t & input_image, output_image_t & output_image)
  {
    if (input_image.pixel_type() != output_image.pixel_type())
    {
      throw std::runtime_error("pixel_type mismatch");
    }

    dispatch_pixel_type<scaler_cubic_interpolation<input_image_t, output_image_t>>(input_image.pixel_type(), input_image, output_image);
  }

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  struct scaler_linear_integration
  {
    template <pixel_type img_pixel_type>
    static void call(const input_image_t & input_image, output_image_t & output_image)
    {
      using color_t = typename pixel_type_traits<img_pixel_type>::color_t;
      using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

      auto in = [&](int x, int y)
      {
        return ggo::convert_color_to<floating_point_color_t>(read_pixel<img_pixel_type>(input_image, x, y));
      };

      auto out = [&](int x, int y, const floating_point_color_t& c)
      {
        write_pixel<img_pixel_type>(output_image, x, y, ggo::convert_color_to<color_t>(c));
      };

      scale_2d_linear_integration(in, input_image.width(), input_image.height(), out, output_image.width(), output_image.height());
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename input_image_t, typename output_image_t>
  void scale_linear_integration(const input_image_t & input_image, output_image_t & output_image)
  {
    if (input_image.pixel_type() != output_image.pixel_type())
    {
      throw std::runtime_error("pixel_type mismatch");
    }

    dispatch_pixel_type<scaler_linear_integration<input_image_t, output_image_t>>(input_image.pixel_type(), input_image, output_image);
  }
}

