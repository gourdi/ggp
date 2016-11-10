#ifndef __GGO_FILL_BUFFER__
#define __GGO_FILL_BUFFER__

#include <stdint.h>
#include <ggo_curve.h>
#include <ggo_perlin_noise_field_2d.h>
#include <ggo_gaussian_field_2d.h>
#include <ggo_color.h>

// Checker.
namespace ggo
{
  template <typename color_t, typename set_pixel_func>
  void fill_checker(int width, int height, const color_t & c1, const color_t & c2, int tile_size, set_pixel_func set_pixel);
}

// Curve.
namespace ggo
{
  template <typename color_t, typename real_t, typename curve_t, typename set_pixel_func>
  void fill_color_curve(int width, int height, const curve_t & curve, set_pixel_func set_pixel);
}

// Gaussian.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_gaussian(int width, int height, real_t var, const color_t & c1, const color_t & c2, set_pixel_func set_pixel);
}

// 4 colors.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_4_colors(int width, int height, const color_t & c1, const color_t & c2, const color_t & c3, const color_t & c4, set_pixel_func set_pixel);
}

// Perlin
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_perlin(int width, int height, real_t size, const color_t & c1, const color_t & c2, set_pixel_func set_pixel);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Field.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_scalar_field2d(
    int width, int height,
    const ggo::scalar_field_2d_abc<real_t> & scalar_field2d,
    const color_t & c1, const color_t & c2,
    set_pixel_func set_pixel)
  {
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        real_t val = scalar_field2d.evaluate(static_cast<real_t>(x), static_cast<real_t>(y));

        set_pixel(x, y, val * c1 + (1 - val) * c2);
      }
    }
  }
}

// Checker implementation.
namespace ggo
{
  template <typename color_t, typename set_pixel_func>
  void fill_checker(int width, int height, const color_t & c1, const color_t & c2, int tile_size, set_pixel_func set_pixel)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        int index_x = x / tile_size;
        int index_y = y / tile_size;

        set_pixelt(x, y, ((index_x + index_y) % 2) ? c1 : c2);
      }
    }
  }
}

// Curve implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename curve_t, typename set_pixel_func>
  void fill_color_curve(int width, int height, const curve_t & curve, set_pixel_func set_pixel)
  {
    for (int y = 0; y < height; ++y)
    {
      const color_t c = curve.evaluate(y / real_t(height));

      for (int x = 0; x < width; ++x)
      {
        set_pixel(x, y, c);
      }
    }
  }
}

// Gaussian implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_gaussian(int width, int height, real_t var, const color_t & c1, const color_t & c2, set_pixel_func set_pixel)
  {
    ggo::gaussian_field2d<real_t> gaussian_field2d;

    gaussian_field2d._x = real_t(0.5) * image.get_width();
    gaussian_field2d._y = real_t(0.5) * image.get_height();
    gaussian_field2d._var = var;
    gaussian_field2d._amp = 1;

    fill_scalar_field2d<color_t>(image, gaussian_field2d, c1, c2, set_pixel);
  }
}

// 4 colors implementation.
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_4_colors(int width, int height, const color_t & c1, const color_t & c2, const color_t & c3, const color_t & c4, set_pixel_func set_pixel)
  {
    for (int y = 0; y < height; ++y)
    {
      color_t color5 = (static_cast<real_t>(y) * c1 + static_cast<real_t>(height - y) * c2) / static_cast<real_t>(height);
      color_t color6 = (static_cast<real_t>(y) * c3 + static_cast<real_t>(height - y) * c4) / static_cast<real_t>(height);

      for (int x = 0; x < width; ++x)
      {
        set_pixel(x, y, (static_cast<real_t>(x) * color5 + static_cast<real_t>(width - x) * color6) / static_cast<real_t>(width));
      }
    }
  }
}

// Perlin implementation
namespace ggo
{
  template <typename color_t, typename real_t, typename set_pixel_func>
  void fill_perlin(int width, int height, real_t size, const color_t & c1, const color_t & c2, set_pixel_func set_pixel)
  {
    ggo::perlin_noise_field2d<real_t> perlin_field2d;

    perlin_field2d.add_octave(size, 1);

    fill_scalar_field2d<color_t>(width, height, perlin_field2d, c1, c2, set_pixel);
  }
}

#endif
