#include "ggo_mandelbrot_bitmap_artist.h"
#include <kernel/math/interpolation/ggo_curve.h>
#include <2d/ggo_image.h>
#include <array>
#include <complex>

namespace
{
  struct block
  {
    std::complex<double> _center;
    double _range;
    double _deviation;
  };

  //////////////////////////////////////////////////////////////
  std::array<ggo::rgb_8u, 1000> setup_palette()
  {
    float hue_start = ggo::rand<float>();
    float hue_end = hue_start + ggo::rand<float>(0.5, 1);

    std::array<ggo::rgb_8u, 1000> palette;

    ggo::linear_curve_float sat_curve;
    sat_curve.push_point(0, 0);
    sat_curve.push_point(ggo::rand<float>(0, 0.5), 1);
    sat_curve.push_point(1, 0);

    ggo::linear_curve_float val_curve;
    val_curve.push_point(0, ggo::rand<float>(0, 0.5));
    val_curve.push_point(1, 1);

    int i = 0;
    for (auto & color : palette)
    {
      float x = i / float(palette.size());

      float hue = std::fmod(ggo::map(x, 0.f, 1.f, hue_start, hue_end), 1.f);
      float sat = sat_curve.evaluate(x);
      float val = val_curve.evaluate(x);

      color = ggo::from_hsv<ggo::rgb_8u>(hue, sat, val);

      ++i;
    }

    return palette;
  }


  //////////////////////////////////////////////////////////////
  int iterate(double x, double y, int max_iterations)
  {
    std::complex<double> z(x, y), c(x, y);

    int i;

    for (i = 0; i < max_iterations; ++i)
    {
      z = z * z + c;

      if (z.real() * z.real() + z.imag() * z.imag() > 10)
      {
        break;
      }
    }

    return i;
  }

  //////////////////////////////////////////////////////////////
  std::tuple<std::complex<double>, double> setup_block(int max_iterations)
  {
    const int blocks_count = 5;

    int depth = 0;

    std::complex<double> center(0, 0);
    double range = 2;
    while (true)
    {
      // Compute iteration deviation for each of the sub blocks.
      std::vector<block> blocks;

      for (int block_y = -blocks_count; block_y <= blocks_count; ++block_y)
      {
        for (int block_x = -blocks_count; block_x <= blocks_count; ++block_x)
        {
          block block;

          // The current sub block position.
          block._range = 0.5 * range / blocks_count;
          double real = center.real() + block_x * range / blocks_count;
          double imag = center.imag() + block_y * range / blocks_count;

          // Shake it a bit.
          block._range *= ggo::rand<double>(0.9, 1.1);
          real += ggo::rand<double>(-block._range / 10, block._range / 10);
          imag += ggo::rand<double>(-block._range / 10, block._range / 10);

          block._center = std::complex<double>(real, imag);

          // Iterations sampling.
          std::vector<int> iterations;
          for (int y = -blocks_count; y <= blocks_count; ++y)
          {
            double y_f = block._center.imag() + y * block._range / blocks_count;

            for (int x = -blocks_count; x <= blocks_count; ++x)
            {
              double x_f = block._center.real() + x * block._range / blocks_count;

              iterations.push_back(iterate(x_f, y_f, max_iterations));
            }
          }

          // Compute mean and deviation.
          double mean = 0.f;
          for (int v : iterations)
          {
            mean += v;
          }
          mean /= iterations.size();

          block._deviation = 0;
          for (int v : iterations)
          {
            double delta = mean - v;
            block._deviation += delta * delta;
          }

          blocks.push_back(block);
        }
      }

      auto max_elmt = std::max_element(blocks.begin(), blocks.end(), [](const block & v1, const block & v2) { return v1._deviation < v2._deviation; });
      center = max_elmt->_center;
      range = max_elmt->_range;

      ++depth;
      if (depth == 4)
      {
        break;
      }
    }

    return std::make_tuple(center, range);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::mandelbrot_bitmap_artist::render_bitmap_t(void * buffer, int width, int height, int line_byte_step) const
{
  image_t<pixel_type, memory_lines_order> img(buffer, { width, height }, line_byte_step);

  auto palette = setup_palette();

  auto [center, range] = setup_block(static_cast<int>(palette.size()));

  for (int y = 0; y < height; ++y)
  {
    double range_x = width > height ? range * width / height : range;
    double range_y = width > height ? range : range * height / width;

    double y1 = ggo::map<double>(y - 3 / 8., 0, height, center.imag() - range_y, center.imag() + range_y);
    double y2 = ggo::map<double>(y - 1 / 8., 0, height, center.imag() - range_y, center.imag() + range_y);
    double y3 = ggo::map<double>(y + 1 / 8., 0, height, center.imag() - range_y, center.imag() + range_y);
    double y4 = ggo::map<double>(y + 3 / 8., 0, height, center.imag() - range_y, center.imag() + range_y);

    int iterations[16];

    for (int x = 0; x < width; ++x)
    {
      // Iterate and sample.
      double x1 = ggo::map<double>(x - 3 / 8., 0, width, center.real() - range_x, center.real() + range_x);
      double x2 = ggo::map<double>(x - 1 / 8., 0, width, center.real() - range_x, center.real() + range_x);
      double x3 = ggo::map<double>(x + 1 / 8., 0, width, center.real() - range_x, center.real() + range_x);
      double x4 = ggo::map<double>(x + 3 / 8., 0, width, center.real() - range_x, center.real() + range_x);

      iterations[0] = iterate(x1, y1, static_cast<int>(palette.size()));
      iterations[1] = iterate(x1, y4, static_cast<int>(palette.size()));
      iterations[2] = iterate(x4, y1, static_cast<int>(palette.size()));
      iterations[3] = iterate(x4, y4, static_cast<int>(palette.size()));

      if ((iterations[0] == iterations[1]) && (iterations[1] == iterations[2]) && (iterations[2] == iterations[3]))
      {
        int index = std::min(static_cast<int>(palette.size() - 1), iterations[0]);

        img.write_pixel(x, y, palette[index]);
      }
      else
      {
        iterations[4] = iterate(x1, y2, static_cast<int>(palette.size()));
        iterations[5] = iterate(x1, y3, static_cast<int>(palette.size()));

        iterations[6] = iterate(x2, y1, static_cast<int>(palette.size()));
        iterations[7] = iterate(x2, y2, static_cast<int>(palette.size()));
        iterations[8] = iterate(x2, y3, static_cast<int>(palette.size()));
        iterations[9] = iterate(x2, y4, static_cast<int>(palette.size()));

        iterations[10] = iterate(x3, y1, static_cast<int>(palette.size()));
        iterations[11] = iterate(x3, y2, static_cast<int>(palette.size()));
        iterations[12] = iterate(x3, y3, static_cast<int>(palette.size()));
        iterations[13] = iterate(x3, y4, static_cast<int>(palette.size()));

        iterations[14] = iterate(x4, y2, static_cast<int>(palette.size()));
        iterations[15] = iterate(x4, y3, static_cast<int>(palette.size()));

        int r = 0;
        int g = 0;
        int b = 0;
        for (int i = 0; i < 16; ++i)
        {
          int index = std::min(static_cast<int>(palette.size() - 1), iterations[i]);
          r += palette[index].r();
          g += palette[index].g();
          b += palette[index].b();
        }

        ggo::rgb_8u c_8u(uint8_t((r + 8) / 16), uint8_t((g + 8) / 16), uint8_t((b + 8) / 16));

        img.write_pixel(x, y, c_8u);
      }
    }
  }
}
	
//////////////////////////////////////////////////////////////
void ggo::mandelbrot_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    render_bitmap_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, width, height, line_byte_step);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    render_bitmap_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, width, height, line_byte_step);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    render_bitmap_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, width, height, line_byte_step);
  }
  else
  {
    GGO_FAIL();
  }
}
