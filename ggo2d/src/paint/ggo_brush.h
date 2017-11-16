#ifndef __GGO_BRUSH__
#define __GGO_BRUSH__

namespace ggo
{
  template <typename color_t>
  struct brush_abc
  {
    virtual color_t operator()(int x, int y) const = 0;
  };

  template <typename color_t>
  struct solid_color_brush : public brush_abc<color_t>
  {
    const color_t _brush_color;

    solid_color_brush(const color_t & c) : _brush_color(c) {}

    color_t operator()(int x, int y) const override
    {
      return _brush_color;
    }
  };

  template <typename color_t>
  struct white_brush : public brush_abc<color_t>
  {
    color_t operator()(int x, int y) const override
    {
      return ggo::white<color_t>();
    }
  };

  template <typename color_t>
  struct black_brush : public brush_abc<color_t>
  {
    color_t operator()(int x, int y) const override
    {
      return ggo::black<color_t>();
    }
  };

  template <typename color_t>
  solid_color_brush<color_t> make_solid_brush(const color_t & c)
  {
    return solid_color_brush<color_t>(c);
  }

  using black_brush_8u = black_brush<ggo::color_8u>;
  using white_brush_8u = white_brush<ggo::color_8u>;
}

#endif
