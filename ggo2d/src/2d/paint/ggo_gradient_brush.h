#ifndef __GGO_RGB_GRADIENT_BRUSH__
#define __GGO_RGB_GRADIENT_BRUSH__

#include <kernel/ggo_ease.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/paint/ggo_brush.h>

//////////////////////////////////////////////////////////////
// Definition.
namespace ggo
{
  template <typename color_t>
  struct gradient_brush
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using data_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");
    
            gradient_brush(const pos2<data_t> & pos, const vec2<data_t> & dir) : _pos(pos), _dir(dir.get_normalized()) {}

    void    push_color(data_t x, const color_t & c) { _curve.push_point(x, ggo::convert_color_to<floating_point_color_t>(c)); }

    color_t operator()(int x, int y) const;

    ggo::cubic_curve<data_t, floating_point_color_t> _curve;
    ggo::pos2<data_t> _pos;
    ggo::vec2<data_t> _dir;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename color_t>
  color_t gradient_brush<color_t>::operator()(int x, int y) const
  {
    ggo::pos2<data_t> p = from_pixel_to_math<data_t>({ x, y });
    ggo::vec2<data_t> diff = p - _pos;

    return convert_color_to<color_t>(_curve.evaluate(ggo::dot(diff, _dir)));
  }
}

#endif

