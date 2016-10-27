#ifndef __GGO_BRUSH__
#define __GGO_BRUSH__

namespace ggo
{
  template <typename color_t>
  struct solid_brush
  {
    solid_brush(const color_t & c) : _color(c) {}

    const color_t & operator()(int x, int y) const {
      return _color;
    }

    const color_t _color;
  };

  template <typename color_t>
  solid_brush<color_t> make_solid_brush(const color_t & c)
  {
    return solid_brush<color_t>(c);
  }
}

#endif
