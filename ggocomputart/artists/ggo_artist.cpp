#include "ggo_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::map_fill(const ggo::pos2f & point, float inf, float sup, int render_width, int render_height)
  {
    ggo::pos2f result;

    if (render_width >= render_height)
    {
      result.get<0>() = ggo::map(point.get<0>(), inf, sup, 0.f, static_cast<float>(render_width));
      result.get<1>() = ggo::map(point.get<1>(), inf, sup, 0.f, static_cast<float>(render_width));

      result.get<1>() -= 0.5f * (render_width - render_height);
    }
    else
    {
      result.get<0>() = ggo::map(point.get<0>(), inf, sup, 0.f, static_cast<float>(render_height));
      result.get<1>() = ggo::map(point.get<1>(), inf, sup, 0.f, static_cast<float>(render_height));

      result.get<0>() -= 0.5f * (render_height - render_width);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  float artist::map_fit(float value, float inf, float sup, int render_width, int render_height)
  {
    if (render_width >= render_height)
    {
      return ggo::map(value, inf, sup, 0.f, static_cast<float>(render_height));
    }
    else
    {
      return ggo::map(value, inf, sup, 0.f, static_cast<float>(render_width));
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::map_fit(const ggo::pos2f & point, float inf, float sup, int render_width, int render_height)
  {
    ggo::pos2f result;

    if (render_width >= render_height)
    {
      result.get<0>() = ggo::map(point.get<0>(), inf, sup, 0.f, static_cast<float>(render_height));
      result.get<1>() = ggo::map(point.get<1>(), inf, sup, 0.f, static_cast<float>(render_height));

      result.get<0>() += 0.5f * (render_width - render_height);
    }
    else
    {
      result.get<0>() = ggo::map(point.get<0>(), inf, sup, 0.f, static_cast<float>(render_width));
      result.get<1>() = ggo::map(point.get<1>(), inf, sup, 0.f, static_cast<float>(render_width));

      result.get<1>() += 0.5f * (render_height - render_width);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  void artist::map_fit(ggo::rect_float & rect, float inf, float sup, int render_width, int render_height)
  {
    if (render_width >= render_height)
    {
      rect.left() = ggo::map(rect.left(), inf, sup, 0.f, static_cast<float>(render_height));
      rect.bottom() = ggo::map(rect.bottom(), inf, sup, 0.f, static_cast<float>(render_height));
      rect.width() = ggo::map(rect.width(), inf, sup, 0.f, static_cast<float>(render_height));
      rect.height() = ggo::map(rect.height(), inf, sup, 0.f, static_cast<float>(render_height));

      rect.left() += 0.5f * (render_width - render_height);
    }
    else
    {
      rect.left() = ggo::map(rect.left(), inf, sup, 0.f, static_cast<float>(render_width));
      rect.bottom() = ggo::map(rect.bottom(), inf, sup, 0.f, static_cast<float>(render_width));
      rect.width() = ggo::map(rect.width(), inf, sup, 0.f, static_cast<float>(render_width));
      rect.height() = ggo::map(rect.height(), inf, sup, 0.f, static_cast<float>(render_width));

      rect.bottom() += 0.5f * (render_height - render_width);
    }
  }

  //////////////////////////////////////////////////////////////
  void artist::map_fit(ggo::disc_float & disc, float inf, float sup, int render_width, int render_height)
  {
    map_fit(disc.center(), inf, sup, render_width, render_height);

    disc.radius() = map_fit(disc.radius(), inf, sup, render_width, render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::get_random_point(int render_width, int render_height)
  {
    return artist::get_random_point(0.f, 0.f, 0.f, 0.f, render_width, render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::get_random_point(float margin, int render_width, int render_height)
  {
    return artist::get_random_point(margin, margin, margin, margin, render_width, render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom, int render_width, int render_height)
  {
    return ggo::pos2f(ggo::rand<float>(margin_left, render_width - margin_right), ggo::rand<float>(margin_bottom, render_height - margin_top));
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::map_fill(const ggo::pos2f & point, float inf, float sup) const
  {
    return map_fill(point, inf, sup, _render_width, _render_height);
  }

  //////////////////////////////////////////////////////////////
  float artist::map_fit(float value, float inf, float sup) const
  {
    return map_fit(value, inf, sup, _render_width, _render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::map_fit(const ggo::pos2f & point, float inf, float sup) const
  {
    return map_fit(point, inf, sup, _render_width, _render_height);
  }

  //////////////////////////////////////////////////////////////
  void artist::map_fit(ggo::rect_float & rect, float inf, float sup) const
  {
    map_fit(rect, inf, sup, _render_width, _render_height);
  }

  //////////////////////////////////////////////////////////////
  void artist::map_fit(ggo::disc_float & disc, float inf, float sup) const
  {
    map_fit(disc, inf, sup, _render_width, _render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::get_center() const
  {
    return ggo::pos2f(static_cast<float>(_render_width - 1) / 2, static_cast<float>(_render_height - 1) / 2);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::horz_mirror(const ggo::pos2f & p, int render_height)
  {
    return ggo::pos2f(p.get<0>(), render_height - p.get<1>());
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2f artist::vert_mirror(const ggo::pos2f & p, int render_width)
  {
    return ggo::pos2f(render_width - p.get<0>(), p.get<1>());
  }
}

