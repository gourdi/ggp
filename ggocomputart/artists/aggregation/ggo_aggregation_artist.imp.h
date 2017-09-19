#include <ggo_buffer_fill.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_gaussian_blur2d.h>

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf>
void ggo::aggregation_artist::render(void * buffer, int line_step) const
{
  ggo::fill_solid<pbf>(buffer, get_width(), get_height(), line_step, _background_color, ggo::rect_int::from_width_height(get_width(), get_height()));

  {
    std::vector<ggo::solid_color_shape<ggo::disc_float, ggo::color_8u>> shapes;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        shapes.emplace_back(ggo::disc_float(point._pos, 2.f * _threshold_dist), ggo::black<ggo::color_8u>());
      }
    }

    ggo::fill_solid<pbf>(buffer, get_width(), get_height(), line_step, _background_color, ggo::rect_int::from_width_height(get_width(), get_height()));

    ggo::paint_shapes<pbf, ggo::sampling_4x4>(buffer, get_width(), get_height(), line_step,
      shapes.begin(), shapes.end(), ggo::rect_int::from_width_height(get_width(), get_height()));
  }

  float stddev = 0.001f * get_min_size();
  ggo::gaussian_blur2d_mirror<pbf>(buffer, get_width(), get_height(), line_step, stddev);

  {
    std::vector<ggo::solid_color_shape<ggo::disc_float, ggo::color_8u>> shapes;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        float hue = point._hue + 0.25f / (1.f + 0.25f * point._counter);
        float sat = point._sat - 0.0015f * point._counter;
        ggo::color_8u c = ggo::from_hsv<ggo::color_8u>(hue, sat, point._val);
        shapes.emplace_back(ggo::disc_float(point._pos, _threshold_dist), c);
      }
    }

    ggo::paint_shapes<pbf, ggo::sampling_4x4>(buffer, get_width(), get_height(), line_step,
      shapes.begin(), shapes.end(),
      ggo::rect_int::from_width_height(get_width(), get_height()));
  }
}