#ifndef __GGO_RGB_GRADIENT_BRUSH__
#define __GGO_RGB_GRADIENT_BRUSH__

#include <ggo_ease.h>
#include <ggo_brush.h>

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
    
            gradient_brush(const color_t & c1, const ggo::pos2<data_t> & p1, const color_t & c2, const ggo::pos2<data_t> & p2);

    color_t operator()(int x, int y) const;

    const floating_point_color_t _color1;
    const floating_point_color_t _color2;
    const ggo::pos2<data_t> _pos1;
    const ggo::pos2<data_t> _pos2;
    const ggo::vec2<data_t> _diff;
    const data_t _hypot;
  };
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename color_t>
  gradient_brush<color_t>::gradient_brush(const color_t & c1, const ggo::pos2<data_t> & p1, const color_t & c2, const ggo::pos2<data_t> & p2)
  :
  _color1(ggo::convert_color_to<floating_point_color_t>(c1)), _pos1(p1),
  _color2(ggo::convert_color_to<floating_point_color_t>(c2)), _pos2(p2),
  _diff(p2 - p1), _hypot(ggo::hypot(p1, p2))
  {

  }

  template <typename color_t>
  color_t gradient_brush<color_t>::operator()(int x, int y) const
  {
    const data_t x_f = static_cast<data_t>(x);
    const data_t y_f = static_cast<data_t>(y);

    const ggo::vec2<data_t> diff_cur(x_f - _pos1.x(), y_f - _pos1.y());
      
    const data_t dot_prod = ggo::dot(_diff, diff_cur);
      
    if (dot_prod <= 0)
    {
      return ggo::convert_color_to<color_t>(_color1);
    }
    else if (dot_prod >= _hypot)
    {
      return ggo::convert_color_to<color_t>(_color2);
    }
    else
    {
      const data_t v2 = ggo::ease_inout(dot_prod / _hypot);
      const data_t v1 = 1 - v2;
      return ggo::convert_color_to<color_t>(v1 * _color1 + v2 * _color2);
    }
  }
}

#endif

