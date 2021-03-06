#include "ggo_cells_bitmap_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_seed_paint.h>
#include <ggo_artist.h>

namespace
{
  class cell
  {
  public:

    using real_t = float;

    cell(float left, float right, float top, float bottom, int render_width, int render_height);

    bool                  is_point_inside(float x, float y) const;
    ggo::rect_data<float> get_seed_rect() const;

  private:

    ggo::pos2_f	_centers[3];
    float			  _inv_width;
    float			  _inv_height;
  };

  struct polynom
  {
    float	_deg2;
    float	_deg1;
    float	_deg0;
  };


  //////////////////////////////////////////////////////////////
  cell::cell(float left, float right, float top, float bottom, int width, int height)
  {
    _inv_width = 1.f / width;
    _inv_height = 1.f / height;

    _centers[0] = ggo::pos2_f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
    _centers[1] = ggo::pos2_f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
    _centers[2] = ggo::pos2_f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
  }

  //////////////////////////////////////////////////////////////
  ggo::rect_data<float> cell::get_seed_rect() const
  {
    float left = _centers[0].x();
    float right = _centers[0].x();
    float top = _centers[0].y();
    float bottom = _centers[0].y();

    left = std::min(_centers[1].x(), left);
    right = std::max(_centers[1].x(), right);
    top = std::max(_centers[1].y(), top);
    bottom = std::min(_centers[1].y(), bottom);

    left = std::min(_centers[2].x(), left);
    right = std::max(_centers[2].x(), right);
    top = std::max(_centers[2].y(), top);
    bottom = std::min(_centers[2].y(), bottom);

    left /= _inv_width;
    right /= _inv_width;
    top /= _inv_height;
    bottom /= _inv_height;

    return { {left, bottom}, right - left, top - bottom };
  }

  //////////////////////////////////////////////////////////////
  bool cell::is_point_inside(float x, float y) const
  {
    float potential = 0;
    float x_map = x * _inv_width;
    float y_map = y * _inv_height;

    potential += 100 / (1 + 100 * ggo::distance(_centers[0], ggo::pos2_f(x_map, y_map)));
    potential += 100 / (1 + 100 * ggo::distance(_centers[1], ggo::pos2_f(x_map, y_map)));
    potential += 100 / (1 + 100 * ggo::distance(_centers[2], ggo::pos2_f(x_map, y_map)));

    return potential > 110 && potential < 120;
  }

  //////////////////////////////////////////////////////////////
  class cells_artist : public ggo::artist
  {
  public:

    //////////////////////////////////////////////////////////////
    cells_artist(int width, int height) : artist(width, height) {}

    //////////////////////////////////////////////////////////////
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_bitmap(void * buffer, int line_byte_step)
    {
      ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step);

      const int cells_count = 250;

      float hue1 = ggo::rand<float>();
      float sat1 = ggo::rand<float>();
      float hue2 = ggo::rand<float>();
      float sat2 = ggo::rand<float>();
      float hue3 = ggo::rand<float>();
      float sat3 = ggo::rand<float>();
      polynom	polynom1;
      polynom	polynom2;

      polynom1._deg2 = ggo::rand<float>(-2, 2);
      polynom1._deg1 = ggo::rand<float>(-2, 2);
      polynom1._deg0 = ggo::rand<float>(-2, 2);

      polynom2._deg2 = ggo::rand<float>(-2, 2);
      polynom2._deg1 = ggo::rand<float>(-2, 2);
      polynom2._deg0 = ggo::rand<float>(-2, 2);

      ggo::fill_solid(img, ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>()));

      for (int counter = 0; counter < cells_count; ++counter)
      {
        float blur_start = 0.005f * min_size();

        float pos_x = ggo::rand<float>();
        float pos_y = ggo::rand<float>();
        cell cell(pos_x - 0.02f, pos_x + 0.02f, pos_y + 0.02f, pos_y - 0.02f, width(), height());

        ggo::rgb_8u color;
        if (polynom1._deg2 * pos_x * pos_x + polynom1._deg1 * pos_x + polynom1._deg0 < pos_y)
        {
          color = ggo::from_hsv<ggo::rgb_8u>(hue1, sat1, ggo::rand<float>());
        }
        else if (polynom2._deg2 * pos_x * pos_x + polynom2._deg1 * pos_x + polynom2._deg0 < pos_y)
        {
          color = ggo::from_hsv<ggo::rgb_8u>(hue2, sat2, ggo::rand<float>());
        }
        else
        {
          color = ggo::from_hsv<ggo::rgb_8u>(hue3, sat3, ggo::rand<float>());
        }

        auto paint_pixel = [&](int x, int y)
        {
          bool done = true;
          const ggo::rgb_8u bkgd_color = img.read_pixel(x, y);
          ggo::color_accumulator<ggo::rgb_8u> acc;

          auto sample_shape = [&](float x_f, float y_f)
          {
            if (cell.is_point_inside(x_f, y_f) == true)
            {
              acc.add(color);
              done = false;
            }
            else
            {
              acc.add(bkgd_color);
            }
          };

          using sample_t = ggo::sampler<ggo::sampling_4x4>;

          sample_t::sample_pixel<float>(x, y, sample_shape);

          img.write_pixel(x, y, acc.div<sample_t::samples_count>());

          return !done;
        };

        ggo::paint_seed_shape(width(), height(), cell, paint_pixel);

        // Blur.
        if (counter % 10 == 0)
        {
          float stddev = 0.0005f * min_size();

          gaussian_blur(img, stddev);
        }
      }
    }
  };
}

//////////////////////////////////////////////////////////////
void ggo::cells_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  cells_artist artist(width, height);

  artist.render_bitmap<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, line_byte_step);
}
