#pragma once

#include <kernel/ggo_ease.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

// Definition.
namespace ggo
{
  template <typename color_t>
  struct gradient_brush
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using scalar_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<scalar_t>::value, "expecting floating point type");
    
            gradient_brush(const pos2<scalar_t> & pos, const vec2<scalar_t> & dir) : _pos(pos), _dir(normalize(dir)) {}

    void    push_color(scalar_t x, const color_t & c) { _curve[x] = ggo::convert_color_to<floating_point_color_t>(c); }

    color_t operator()(int x, int y) const;

    std::map<scalar_t, floating_point_color_t> _curve;
    const ggo::pos2<scalar_t> _pos;
    const ggo::vec2<scalar_t> _dir;
  };
}

// Implementation.
namespace ggo
{
  template <typename color_t>
  color_t gradient_brush<color_t>::operator()(int x, int y) const
  {
    ggo::pos2<scalar_t> p = get_pixel_center<scalar_t>({ x, y });
    ggo::vec2<scalar_t> diff = p - _pos;
    scalar_t v = std::clamp(ggo::dot(diff, _dir), 0.f, _curve.rbegin()->first);

    return convert_color_to<color_t>(ggo::cubic_interpolation1d(_curve.begin(), _curve.end(), v));
  }
}
