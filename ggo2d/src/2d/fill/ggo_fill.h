#ifndef __GGO_FILL__
#define __GGO_FILL__

#include <kernel/math/interpolation/ggo_curve.h>
#include <2d/ggo_color.h>
#include <2d/ggo_image_format.h>
#include <2d/fill/ggo_fill_generic.h>

// Fill with solid color.
namespace ggo
{
  template <image_format format>
  void fill_solid(void * buffer, int width, int height, int line_byte_step, const typename image_format_traits<format>::color_t & c);

  template <image_format format>
  void fill_solid(void * buffer, int width, int height, int line_byte_step, const typename image_format_traits<format>::color_t & c, const ggo::rect_int & clipping);
}

// Checker.
namespace ggo
{
  template <image_format format>
  void fill_checker(void * buffer, int width, int height, int line_byte_step, 
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2, 
    int tile_size);
}

// 4 colors.
namespace ggo
{
  template <image_format format>
  void fill_4_colors(void * buffer, int width, int height, int line_byte_step,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2,
    const typename image_format_traits<format>::color_t & c3,
    const typename image_format_traits<format>::color_t & c4,
    const ggo::rect_int & clipping);

  template <image_format format>
  void fill_4_colors(void * buffer, int width, int height, int line_byte_step,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2,
    const typename image_format_traits<format>::color_t & c3,
    const typename image_format_traits<format>::color_t & c4);
}

// Curve.
namespace ggo
{
  template <image_format format, typename curve_t>
  void fill_color_curve(void * buffer, int width, int height, int line_byte_step, const curve_t & curve);
}

// Perlin
namespace ggo
{
  template <image_format format>
  void fill_perlin(void * buffer, int width, int height, int line_byte_step, 
    typename color_traits<typename color_traits<typename image_format_traits<format>::color_t>::floating_point_color_t>::sample_t size,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2);
}

// Gaussian.
namespace ggo
{
  template <image_format format>
  void fill_gaussian(void * buffer, int width, int height, int line_byte_step, 
    typename color_traits<typename color_traits<typename image_format_traits<format>::color_t>::floating_point_color_t>::sample_t stddev,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Fill with solid color.
namespace ggo
{
  template <image_format format>
  void fill_solid(void * buffer, int width, int height, int line_byte_step, const typename image_format_traits<format>::color_t & c, const ggo::rect_int & clipping)
  {
    using format_traits = image_format_traits<format>;

    constexpr int pixel_byte_size = format_traits::pixel_byte_size;

    ggo::rect_int rect = ggo::rect_int::from_width_height(width, height);
    if (rect.clip(clipping) == true)
    {
      for (int y = rect.bottom(); y <= rect.top(); ++y)
      {
        void * ptr = ptr_offset(get_line_ptr<format_traits::lines_order>(buffer, y, height, line_byte_step), rect.left() * pixel_byte_size);
        void * end = ptr_offset(get_line_ptr<format_traits::lines_order>(buffer, y, height, line_byte_step), (rect.right() + 1) * pixel_byte_size);

        for (; ptr != end; ptr = ptr_offset<pixel_byte_size>(ptr))
        {
          write_pixel<format>(ptr, c);
        }
      }
    }
  }

  template <image_format format>
  void fill_solid(void * buffer, int width, int height, int line_byte_step, const typename image_format_traits<format>::color_t & c)
  {
    fill_solid<format>(buffer, width, height, line_byte_step, c, ggo::rect_int::from_width_height(width, height));
  }
}

// Checker.
namespace ggo
{
  template <image_format format>
  void fill_checker(void * buffer, int width, int height, int line_byte_step,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2,
    int tile_size)
  {
    using color_t = typename image_format_traits<format>::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c) {
      write_pixel<format>(buffer, x, y, height, line_byte_step, ggo::convert_color_to<color_t>(c));
    };

    ggo::fill_checker<floating_point_color_t>(width, height, c1_fp, c2_fp, tile_size, write_pixel_func);
  }
}

// 4 colors.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <image_format format>
  void fill_4_colors(void * buffer, int width, int height, int line_byte_step,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2,
    const typename image_format_traits<format>::color_t & c3,
    const typename image_format_traits<format>::color_t & c4,
    const ggo::rect_int & clipping)
  {
    using color_t = typename image_format_traits<format>::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;
    using real_t = typename ggo::color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);
    const floating_point_color_t c3_fp = ggo::convert_color_to<floating_point_color_t>(c3);
    const floating_point_color_t c4_fp = ggo::convert_color_to<floating_point_color_t>(c4);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c) {
      write_pixel<format>(buffer, x, y, height, line_byte_step, ggo::convert_color_to<color_t>(c));
    };

    ggo::fill_4_colors<floating_point_color_t, real_t>(width, height, c1_fp, c2_fp, c3_fp, c4_fp, clipping, write_pixel_func);
  }

  template <image_format format>
  void fill_4_colors(void * buffer, int width, int height, int line_byte_step,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2,
    const typename image_format_traits<format>::color_t & c3,
    const typename image_format_traits<format>::color_t & c4)
  {
    fill_4_colors<format>(buffer, width, height, line_byte_step, c1, c2, c3, c4, ggo::rect_int::from_width_height(width, height));
  }
}

// Curve.
namespace ggo
{
  template <image_format format, typename curve_t>
  void fill_color_curve(void * buffer, int width, int height, int line_byte_step, const curve_t & curve)
  {
    using color_t = typename image_format_traits<format>::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_color_t;
    using real_t = typename ggo::color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");
    static_assert(std::is_same<typename curve_t::coord_t, real_t>::value, "type mismatch");
    static_assert(std::is_same<typename curve_t::data_t, floating_point_color_t>::value, "execting floating point type");

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::write_pixel<format>(buffer, x, y, height, line_byte_step, ggo::convert_color_to<color_t>(c));
    };

    fill_color_curve<floating_point_color_t, real_t>(width, height, curve, write_pixel_func);
  }
}

// Perlin.
namespace ggo
{
  template <image_format format>
  void fill_perlin(void * buffer, int width, int height, int line_byte_step,
    typename color_traits<typename color_traits<typename image_format_traits<format>::color_t>::floating_point_color_t>::sample_t size,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2)
  {
    using color_t = typename image_format_traits<format>::color_t;
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");
    static_assert(std::is_same<real_t, decltype(size)>::value, "type mismatch");

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::write_pixel<format>(buffer, x, y, height, line_byte_step, ggo::convert_color_to<color_t>(c));
    };

    fill_perlin(width, height, size, c1_fp, c2_fp, write_pixel_func);
  }
}

// Gaussian.
namespace ggo
{
  template <image_format format>
  void fill_gaussian(void * buffer, int width, int height, int line_byte_step,
    typename color_traits<typename color_traits<typename image_format_traits<format>::color_t>::floating_point_color_t>::sample_t stddev,
    const typename image_format_traits<format>::color_t & c1,
    const typename image_format_traits<format>::color_t & c2)
  {
    using color_t = typename image_format_traits<format>::color_t;
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");
    static_assert(std::is_same<real_t, decltype(stddev)>::value, "type mismatch");

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::write_pixel<format>(buffer, x, y, height, line_byte_step, ggo::convert_color_to<color_t>(c));
    };

    fill_gaussian(width, height, stddev, c1_fp, c2_fp, write_pixel_func);
  }
}

#endif
