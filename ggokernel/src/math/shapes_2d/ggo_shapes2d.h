#ifndef __GGO_SHAPES2D__
#define __GGO_SHAPES2D__

#include <vector>
#include <ggo_kernel.h>
#include <ggo_quadratic.h>
#include <ggo_vec.h>
#include <ggo_distance.h>
#include <ggo_linear_algebra2d.h>

//////////////////////////////////////////////////////////////////
// RECTANGLE_INTERSECTION
namespace ggo
{
  enum class rect_intersection
  {
    rect_in_shape,
    shape_in_rect,
    partial_overlap,
    disjoints,
    UNKNOWN
  };

  template <typename T>
  struct rect_data
  {
    pos2<T>  _pos;
    T        _width;
    T        _height;
  };

  template <typename T>
  rect_data<T> rect_data_union(const rect_data<T> & rect1, const rect_data<T> & rect2)
  {
    T left1 = rect1._pos.template get<0>();
    T bottom1 = rect1._pos.template get<1>();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.template get<0>();
    T bottom2 = rect2._pos.template get<1>();
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
    T left1 = rect1._pos.template get<0>();
    T bottom1 = rect1._pos.template get<1>();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.template get<0>();
    T bottom2 = rect2._pos.template get<1>();
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
  template <typename data_t>
  class movable_shape2d_abc
  {
  public:

    virtual	void move(data_t dx, data_t dy) = 0;
            void move(const ggo::vec2<data_t> & m) { move(m.template get<0>(), m.template get<1>()); }
  };

  template <typename data_t>
  class rotatable_shape2d_abc
  {
  public:

    virtual	void rotate(data_t angle, const ggo::pos2<data_t> & center) = 0;
  };

  template <typename data_t>
  class distancable_shape2d_abc
  {
  public:

    virtual	data_t  dist_to_point(data_t x, data_t y) const = 0;
            data_t  dist_to_point(const ggo::pos2<data_t> & p) const { return dist_to_point(p.template get<0>(), p.template get<1>()); }
  };

  template <typename T>
  class affine_shape2d_abc : public movable_shape2d_abc<T>, public rotatable_shape2d_abc<T>
  {
  public:

    using rotatable_shape2d_abc<T>::rotate;

    virtual ggo::pos2<T> get_center() const = 0;

    void rotate(T angle) { rotate(angle, get_center()); }
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
            bool  is_point_inside(const ggo::pos2<T> & p) const { return is_point_inside(p.template get<0>(), p.template get<1>()); }
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
