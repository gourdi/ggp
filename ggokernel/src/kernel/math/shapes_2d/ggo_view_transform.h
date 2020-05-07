#pragma once

#include <kernel/ggo_vec2.h>
#include <kernel/ggo_size.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  template <typename scalar_t>
  struct view_transform
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    const scalar_t _scaling;
    const pos2<scalar_t> _translation;

    scalar_t operator()(scalar_t distance) const
    { 
      return _scaling * distance;
    }

    ggo::pos2<scalar_t> operator()(ggo::pos2<scalar_t> p) const
    { 
      return _scaling * p + _translation;
    }

    // The transform makes the view the largest one that entirely fits in the render size.
    static view_transform make_fit(ggo::rect<scalar_t> view, ggo::size render_size)
    {
      scalar_t render_width   = static_cast<scalar_t>(render_size.width());
      scalar_t render_height  = static_cast<scalar_t>(render_size.height());
      scalar_t view_width     = view.width();
      scalar_t view_height    = view.height();

      // Add padding on the left and the right.
      if (render_width / render_height > view_width / view_height)
      {
        scalar_t scaling = render_height / view_height;
        scalar_t t_x = (render_width - (view.left() + view.right()) * scaling) / 2;
        scalar_t t_y = -scaling * view.bottom();
        return { scaling, { t_x, t_y } };
      }
      else // Add padding on the bottom and the top.
      {
        scalar_t scaling = render_width / view_width;
        scalar_t t_x = -scaling * view.left();
        scalar_t t_y = (render_height - (view.bottom() + view.top()) * scaling) / 2;
        return { scaling, { t_x, t_y } };
      }
    }

    // The transform makes the view the largest one that entirely fits in the render size.
    static view_transform make_fill(ggo::rect<scalar_t> view, ggo::size render_size)
    {
      scalar_t render_width = static_cast<scalar_t>(render_size.width());
      scalar_t render_height = static_cast<scalar_t>(render_size.height());
      scalar_t view_width = view.width();
      scalar_t view_height = view.height();

      // Add padding on the left and the right.
      if (view_width / view_height > render_width / render_height)
      {
        scalar_t scaling = render_height / view_height;
        scalar_t t_x = (render_width - (view.left() + view.right()) * scaling) / 2;
        scalar_t t_y = -scaling * view.bottom();
        return { scaling, { t_x, t_y } };
      }
      else // Add padding on the bottom and the top.
      {
        scalar_t scaling = render_width / view_width;
        scalar_t t_x = -scaling * view.left();
        scalar_t t_y = (render_height - (view.bottom() + view.top()) * scaling) / 2;
        return { scaling, { t_x, t_y } };
      }
    }
  };
}
