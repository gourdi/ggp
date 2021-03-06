#pragma once

#include <2d/ggo_color.h>

namespace ggo
{
  template <typename color_t_>
  struct solid_color_brush
  {
    using color_t = color_t_;

    const color_t _brush_color;

    solid_color_brush(const color_t & c) : _brush_color(c) {}

    color_t operator()(int x, int y) const
    {
      return _brush_color;
    }
  };

  using solid_color_brush_rgb8u = solid_color_brush<ggo::rgb_8u>;
}

namespace ggo
{
  template <typename color_t>
  struct white_brush
  {
    color_t operator()(int x, int y) const
    {
      return ggo::white<color_t>();
    }
  };

  template <typename color_t>
  struct black_brush
  {
    color_t operator()(int x, int y) const
    {
      return ggo::black<color_t>();
    }
  };

  template <typename color_t>
  solid_color_brush<color_t> make_solid_brush(const color_t & c)
  {
    return solid_color_brush<color_t>(c);
  }

  using black_brush_8u = black_brush<ggo::rgb_8u>;
  using white_brush_8u = white_brush<ggo::rgb_8u>;
}
