#ifndef __GGO_SHAPES2D__
#define __GGO_SHAPES2D__

#include <vector>
#include <ggo_kernel.h>
#include <ggo_quadratic.h>
#include <ggo_vec.h>
#include <ggo_distance.h>
#include <ggo_rect_data.h>
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
    unknown
  };
}

//////////////////////////////////////////////////////////////////
// GEOMETRY
namespace ggo
{
  template <typename data_t>
  class movable_shape2d_abc
  {
  public:

    virtual	void move(const ggo::vec2<data_t> & m) = 0;
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

    virtual	data_t  dist_to_point(const ggo::pos2<data_t> & p) const = 0;
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
  template <typename data_t>
  class samplable_shape2d_abc
  {
  public:

    virtual bool  is_point_inside(const ggo::pos2<data_t> & p) const = 0;
  };

  template <typename data_t>
  class bounded_shape2d_abc
  {
  public:

    virtual rect_data<data_t> get_bounding_rect() const = 0;
  };

  template <typename data_t>
  class rect_intersect_shape2d_abc
  {
  public:

    virtual	rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const = 0;
  };

  template <typename data_t>
  class paintable_shape2d_abc : public samplable_shape2d_abc<data_t>, public bounded_shape2d_abc<data_t>, public rect_intersect_shape2d_abc<data_t>
  {
  public:
    
    using samplable_shape2d_abc<data_t>::is_point_inside;
  };
}

#include <ggo_basis2d.h>
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

#include <ggo_basis2d.imp.h>
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
