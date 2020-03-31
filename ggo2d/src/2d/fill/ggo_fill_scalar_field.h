#ifndef __GGO_FILL_SCALAR_FIELD__
#define __GGO_FILL_SCALAR_FIELD__

#include <kernel/math/ggo_discretization.h>
#include <kernel/math/scalar_fields_2d/ggo_perlin_noise_field_2d.h>
#include <kernel/math/scalar_fields_2d/ggo_gaussian_field_2d.h>
#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

namespace ggo
{
  template <typename image_t, typename scalar_field_t>
  void fill_scalar_field2d(image_t & image, const scalar_field_t & field,
    const typename image_t::color_t & c1, const typename image_t::color_t & c2)
  {
    using floating_point_color_t = typename color_traits<image_t::color_t>::floating_point_color_t;
    using floating_point_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point_v<floating_point_t>);

    const int w = image.width();
    const int h = image.height();
    const floating_point_color_t c1_fp = convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = convert_color_to<floating_point_color_t>(c2);

    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        auto s = get_pixel_center<floating_point_t>(ggo::pos2_i(x, y));

        auto val = field.sample(s.x(), s.y());

        image.write_pixel(x, y, ggo::convert_color_to<image_t::color_t>(val * c1_fp + (1 - val) * c2_fp));
      }
    }
  }
}

namespace ggo
{
  template <typename image_t>
  void fill_perlin(image_t & image, typename color_traits<typename color_traits<typename image_t::color_t>::floating_point_color_t>::sample_t size,
    const typename image_t::color_t & c1, const typename image_t::color_t & c2)
  {
    using floating_point_color_t = typename color_traits<typename image_t::color_t>::floating_point_color_t;
    using floating_point_t = typename color_traits<typename floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point_v<floating_point_t>);

    ggo::perlin_noise_field2d<floating_point_t> perlin_field2d;

    perlin_field2d.add_octave(size, 1);

    fill_scalar_field2d(image, perlin_field2d, c1, c2);
  }

  template <ggo::pixel_type pixel_type>
  void fill_perlin(void * buffer,
    int width, int height, int line_byte_step,
    typename color_traits<typename color_traits<typename pixel_type_traits<pixel_type>::color_t>::floating_point_color_t>::sample_t size,
    const typename pixel_type_traits<pixel_type>::color_t & c1, const typename pixel_type_traits<pixel_type>::color_t & c2)
  {
    ggo::image_view_t<ggo::memory_lines_order::up, pixel_type> image(buffer, { width, height }, line_byte_step);
    
    fill_perlin(image, size, c1, c2);
  }
}

namespace ggo
{
  template <typename image_t>
  void fill_gaussian(image_t & image, 
    typename color_traits<typename color_traits<typename image_t::color_t>::floating_point_color_t>::sample_t stddev,
    const typename image_t::color_t & c1,
    const typename image_t::color_t & c2)
  {
    using floating_point_color_t = typename color_traits<typename image_t::color_t>::floating_point_color_t;
    using floating_point_t = typename color_traits<typename floating_point_color_t>::sample_t;

    ggo::gaussian_field2d<floating_point_t> gaussian_field2d;

    gaussian_field2d._x = floating_point_t(0.5) * image.width();
    gaussian_field2d._y = floating_point_t(0.5) * image.height();
    gaussian_field2d._var = stddev * stddev;
    gaussian_field2d._amp = 1;

    fill_scalar_field2d(image, gaussian_field2d, c1, c2);
  }

  template <ggo::pixel_type pixel_type>
  void fill_gaussian(void * buffer,
    int width, int height, int line_byte_step,
    typename color_traits<typename color_traits<typename pixel_type_traits<pixel_type>::color_t>::floating_point_color_t>::sample_t stddev,
    const typename pixel_type_traits<pixel_type>::color_t & c1, const typename pixel_type_traits<pixel_type>::color_t & c2)
  {
    ggo::image_view_t<ggo::memory_lines_order::up, pixel_type> image(buffer, { width, height }, line_byte_step);

    fill_gaussian(image, stddev, c1, c2);
  }
}

#endif
