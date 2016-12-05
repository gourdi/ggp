#ifndef __GGO_FILL_BUFFER__
#define __GGO_FILL_BUFFER__

#include <stdint.h>
#include <ggo_curve.h>
#include <ggo_perlin_noise_field_2d.h>
#include <ggo_gaussian_field_2d.h>
#include <ggo_color.h>
#include <ggo_pixel_rect.h>

// Checker.
namespace ggo
{
  template <typename color_t, typename set_pixel_func_t>
  void fill_checker(int width, int height, const color_t & c1, const color_t & c2, int tile_size, set_pixel_func_t set_pixel_func);
}

// Curve.
namespace ggo
{
  template <typename color_t, typename real_t, typename curve_t, typename set_pixel_func_t>
  void fill_color_curve(int width, int height, const curve_t & curve, set_pixel_func_t set_pixel_func);
}

// Gaussian.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_gaussian(int width, int height, real_t stddev, const color_t & c1, const color_t & c2, set_pixel_func_t set_pixel_func);
}

// 4 colors.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_4_colors(int width, int height, const color_t & c1, const color_t & c2, const color_t & c3, const color_t & c4,
    const ggo::pixel_rect & clipping, set_pixel_func_t set_pixel_func);
}

// Perlin
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_perlin(int width, int height, real_t size, const color_t & c1, const color_t & c2, set_pixel_func_t set_pixel_func);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Field.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_scalar_field2d(
    int width, int height,
    const ggo::scalar_field_2d_abc<real_t> & scalar_field2d,
    const color_t & c1, const color_t & c2,
    set_pixel_func_t set_pixel_func)
  {
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        real_t val = scalar_field2d.evaluate(static_cast<real_t>(x), static_cast<real_t>(y));

        set_pixel_func(x, y, val * c1 + (1 - val) * c2);
      }
    }
  }
}

// Checker implementation.
namespace ggo
{
  template <typename color_t, typename set_pixel_func_t>
  void fill_checker(int width, int height, const color_t & c1, const color_t & c2, int tile_size, set_pixel_func_t set_pixel_func)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        int index_x = x / tile_size;
        int index_y = y / tile_size;

        set_pixel_func(x, y, ((index_x + index_y) % 2) ? c1 : c2);
      }
    }
  }
}

// Curve implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename curve_t, typename set_pixel_func_t>
  void fill_color_curve(int width, int height, const curve_t & curve, set_pixel_func_t set_pixel_func)
  {
    for (int y = 0; y < height; ++y)
    {
      const color_t c = curve.evaluate(y / real_t(height));

      for (int x = 0; x < width; ++x)
      {
        set_pixel_func(x, y, c);
      }
    }
  }
}

// Gaussian implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_gaussian(int width, int height, real_t stddev, const color_t & c1, const color_t & c2, set_pixel_func_t set_pixel_func)
  {
    ggo::gaussian_field2d<real_t> gaussian_field2d;

    gaussian_field2d._x = real_t(0.5) *width;
    gaussian_field2d._y = real_t(0.5) *height;
    gaussian_field2d._var = stddev * stddev;
    gaussian_field2d._amp = 1;

    fill_scalar_field2d<color_t>(width, height, gaussian_field2d, c1, c2, set_pixel_func);
  }
}

// 4 colors implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_4_colors(int width, int height, const color_t & c1, const color_t & c2, const color_t & c3, const color_t & c4,
    const ggo::pixel_rect & clipping, set_pixel_func_t set_pixel_func)
  {
    for (int y = clipping.bottom(); y <= clipping.top(); ++y)
    {
      color_t color5 = (static_cast<real_t>(y) * c1 + static_cast<real_t>(height - y) * c2) / static_cast<real_t>(height);
      color_t color6 = (static_cast<real_t>(y) * c3 + static_cast<real_t>(height - y) * c4) / static_cast<real_t>(height);

      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        set_pixel_func(x, y, (static_cast<real_t>(x) * color5 + static_cast<real_t>(width - x) * color6) / static_cast<real_t>(width));
      }
    }
  }
}

// Perlin implementation
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func_t>
  void fill_perlin(int width, int height, real_t size, const color_t & c1, const color_t & c2, set_pixel_func_t set_pixel_func)
  {
    ggo::perlin_noise_field2d<real_t> perlin_field2d;

    perlin_field2d.add_octave(size, 1);

    fill_scalar_field2d<color_t>(width, height, perlin_field2d, c1, c2, set_pixel_func);
  }
}

#endif
