#ifndef __GGO_SHAPES2D__
#define __GGO_SHAPES2D__

#include <vector>
#include <ggo_kernel.h>
#include <ggo_quadratic.h>
#include <ggo_set2.h>
#include <ggo_distance2d.h>
#include <ggo_linear_algebra2d.h>

//////////////////////////////////////////////////////////////////
// RECTANGLE_INTERSECTION
namespace ggo
{
  enum class rect_intersection
  {
    RECT_IN_SHAPE,
    SHAPE_IN_RECT,
    PARTIAL_OVERLAP,
    DISJOINTS,
    UNKNOWN
  };

  template <typename T>
  struct rect_data
  {
    set2<T>  _pos;
    T        _width;
    T        _height;
  };

  template <typename T>
  rect_data<T> rect_data_union(const rect_data<T> & rect1, const rect_data<T> & rect2)
  {
    T left1 = rect1._pos.x();
    T bottom1 = rect1._pos.y();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.x();
    T bottom2 = rect2._pos.y();
    T right2 = left2 + rect2._width;
    T top2 = bottom2 + rect2._height;

    T left = std::min(left1, left2);
    T bottom = std::min(bottom1, bottom2);
    T right = std::max(right1, right2);
    T top = std::max(top1, top2);

    return { { left, bottom }, right - left, top - bottom };
  }

  template <typename T>
  bool rect_data_intersection(const rect_data<T> & rect1, const rect_data<T> & rect2, rect_data<T> & result)
  {
    T left1 = rect1._pos.x();
    T bottom1 = rect1._pos.y();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.x();
    T bottom2 = rect2._pos.y();
    T right2 = left2 + rect2._width;
    T top2 = bottom2 + rect2._height;

    if (right1 < left2 || right2 < left1)
    {
      return false;
    }

    if (bottom1 > top2 || bottom2 > top1)
    {
      return false;
    }

    T left = std::max(left1, left2);
    T bottom = std::max(bottom1, bottom2);
    T right = std::min(right1, right2);
    T top = std::min(top1, top2);

    result = { { left, bottom }, right - left, top - bottom };

    return true;
  }
}

//////////////////////////////////////////////////////////////////
// GEOMETRY

namespace ggo
{
  template <typename T>
  class movable_shape2d_abc
  {
  public:

    virtual	void	move(T dx, T dy) = 0;
            void	move(const ggo::set2<T> & m) { move(m.x(), m.y()); }
  };

  template <typename T>
  class rotatable_shape2d_abc
  {
  public:

    virtual	void  rotate(T angle) = 0;
  };

  template <typename T> 
  class distancable_shape2d_abc
  {
  public:

    virtual	T dist_to_point(T x, T y) const = 0;
            T	dist_to_point(const ggo::set2<T> & p) const { return dist_to_point(p.x(), p.y()); }
  };
}

//////////////////////////////////////////////////////////////////
// PHYSICS

namespace ggo
{
  template <typename T>
  class physics_shape2d_abc : public movable_shape2d_abc<T>, public rotatable_shape2d_abc<T>
  {
  public:

    using movable_shape2d_abc<T>::move;
    using rotatable_shape2d_abc<T>::rotate;

    virtual const ggo::set2<T> &      get_center() const = 0;
    virtual std::vector<ggo::set2<T>> get_draw_points() const = 0;

            void                      rotate(T angle, const ggo::set2<T> & center)
            {
              ggo::set2<T> diff(get_center() - center);
              move(-diff);
              diff.rotate(angle);
              rotate(angle);
              move(diff);
            }
  };
}

//////////////////////////////////////////////////////////////////
// PAINT

namespace ggo
{
  template <typename T>
  class samplable_shape2d_abc
  {
  public:

    virtual bool  is_point_inside(T x, T y) const = 0;
            bool  is_point_inside(const ggo::set2<T> & p) const { return is_point_inside(p.x(), p.y()); }
  };

  template <typename T>
  class bounded_shape2d_abc
  {
  public:

    virtual rect_data<T> get_bounding_rect() const = 0;
  };

  template <typename T>
  class rect_intersect_shape2d_abc
  {
  public:

    virtual	rect_intersection	get_rect_intersection(const rect_data<T> & rect_data) const = 0;
  };

  template <typename T>
  class paintable_shape2d_abc : public samplable_shape2d_abc<T>, public bounded_shape2d_abc<T>, public rect_intersect_shape2d_abc<T>
  {
  public:
    
    using samplable_shape2d_abc<T>::is_point_inside;
  };

  template <typename T>
  class seed_paintable_shape2d_abc : public ggo::samplable_shape2d_abc<T>
  {
  public:

    virtual	rect_data<T> get_seed_rect() const = 0;
  };
}

#include <ggo_rect.h>
#include <ggo_segment.h>
#include <ggo_circle.h>
#include <ggo_disc.h>
#include <ggo_extended_segment.h>
#include <ggo_spline.h>
#include <ggo_polygon.h>
#include <ggo_triangle2d.h>
#include <ggo_half_plane.h>
#include <ggo_oriented_box.h>
#include <ggo_multi_shape.h>

#include <ggo_rect.imp.h>
#include <ggo_segment.imp.h>
#include <ggo_circle.imp.h>
#include <ggo_disc.imp.h>
#include <ggo_extended_segment.imp.h>
#include <ggo_spline.imp.h>
#include <ggo_polygon.imp.h>
#include <ggo_triangle2d.imp.h>
#include <ggo_half_plane.imp.h>
#include <ggo_oriented_box.imp.h>
#include <ggo_multi_shape.imp.h>

#endif
