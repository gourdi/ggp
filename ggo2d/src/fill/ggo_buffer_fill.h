#ifndef __GGO_BUFFER_FILL__
#define __GGO_BUFFER_FILL__

#include <ggo_color.h>
#include <ggo_fill.h>
#include <ggo_pixel_buffer.h>

// Fill with solid color.
namespace ggo
{
  template <pixel_buffer_format pbf>
  void fill_solid(void * buffer, int width, int height, int line_step, const typename pixel_buffer_format_info<pbf>::color_t & c);
}

// 4 colors.
namespace ggo
{
  template <pixel_buffer_format pbf>
  void fill_4_colors(void * buffer, int width, int height, int line_step,
    const typename pixel_buffer_format_info<pbf>::color_t & c1,
    const typename pixel_buffer_format_info<pbf>::color_t & c2,
    const typename pixel_buffer_format_info<pbf>::color_t & c3,
    const typename pixel_buffer_format_info<pbf>::color_t & c4);
}

// Curve.
namespace ggo
{
  template <pixel_buffer_format pbf, typename curve_t>
  void fill_color_curve(void * buffer, int width, int height, int line_step, const curve_t & curve);
}

// Perlin
namespace ggo
{
  template <pixel_buffer_format pbf>
  void fill_perlin(void * buffer, int width, int height, int line_step, 
    typename color_traits<typename color_traits<typename pixel_buffer_format_info<pbf>::color_t>::floating_point_t>::sample_t size,
    const typename pixel_buffer_format_info<pbf>::color_t & c1,
    const typename pixel_buffer_format_info<pbf>::color_t & c2);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Fill with solid color.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void fill_solid(void * buffer, int width, int height, int line_step, const typename pixel_buffer_format_info<pbf>::color_t & c)
  {
    process_buffer<pbf>(buffer, width, height, line_step, [&](void * ptr) { write_pixel<pbf>(ptr, c); });
  }
}

// 4 colors.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void fill_4_colors(void * buffer, int width, int height, int line_step,
    const typename pixel_buffer_format_info<pbf>::color_t & c1,
    const typename pixel_buffer_format_info<pbf>::color_t & c2,
    const typename pixel_buffer_format_info<pbf>::color_t & c3,
    const typename pixel_buffer_format_info<pbf>::color_t & c4)
  {
    using color_t = typename pixel_buffer_format_info<pbf>::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_t;
    using real_t = typename ggo::color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);
    const floating_point_color_t c3_fp = ggo::convert_color_to<floating_point_color_t>(c3);
    const floating_point_color_t c4_fp = ggo::convert_color_to<floating_point_color_t>(c4);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c) {
      write_pixel<pbf>(buffer, x, y, height, line_step, ggo::convert_color_to<color_t>(c));
    };

    ggo::fill_4_colors<floating_point_color_t, real_t>(width, height, c1_fp, c2_fp, c3_fp, c4_fp, write_pixel_func);
  }
}

// Curve.
namespace ggo
{
  template <pixel_buffer_format pbf, typename curve_t>
  void fill_color_curve(void * buffer, int width, int height, int line_step, const curve_t & curve)
  {
    using color_t = typename pixel_buffer_format_info<pbf>::color_t;
    using floating_point_color_t = typename ggo::color_traits<color_t>::floating_point_t;
    using real_t = typename ggo::color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");
    static_assert(std::is_same<typename curve_t::coord_t, real_t>::value, "type mismatch");
    static_assert(std::is_same<typename curve_t::data_t, floating_point_color_t>::value, "execting floating point type");

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, ggo::convert_color_to<color_t>(c));
    };

    fill_color_curve<floating_point_color_t, real_t>(width, height, curve, write_pixel_func);
  }
}

// Perlin.
namespace ggo
{
  template <pixel_buffer_format pbf>
  void fill_perlin(void * buffer, int width, int height, int line_step,
    typename color_traits<typename color_traits<typename pixel_buffer_format_info<pbf>::color_t>::floating_point_t>::sample_t size,
    const typename pixel_buffer_format_info<pbf>::color_t & c1,
    const typename pixel_buffer_format_info<pbf>::color_t & c2)
  {
    using color_t = typename pixel_buffer_format_info<pbf>::color_t;
    using floating_point_color_t = typename color_traits<color_t>::floating_point_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "execting floating point type");
    static_assert(std::is_same<real_t, decltype(size)>::value, "type mismatch");

    const floating_point_color_t c1_fp = ggo::convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = ggo::convert_color_to<floating_point_color_t>(c2);

    auto write_pixel_func = [&](int x, int y, const floating_point_color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, ggo::convert_color_to<color_t>(c));
    };

    fill_perlin(width, height, size, c1_fp, c2_fp, write_pixel_func);
  }
}

#endif
