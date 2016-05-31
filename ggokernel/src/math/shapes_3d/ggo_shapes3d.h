#ifndef __GGO_SHAPES3D__
#define __GGO_SHAPES3D__

#include <sstream>
#include <ggo_kernel.h>
#include <ggo_set3.h>
#include <ggo_distance3d.h>
#include <ggo_ray3d.h>

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  struct vertex
  {
    vertex(const ggo::set3<T> & pos, const ggo::set3<T> & normal) : _pos(pos), _normal(normal) { _normal.normalize(); }

    ggo::set3<T> _pos;
    ggo::set3<T> _normal;
  };
}

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class raytracable_shape3d_abc
  {
  public:
      
    virtual                ~raytracable_shape3d_abc() {}

    virtual bool            intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const = 0;
    virtual bool            is_convex() const = 0;

    virtual ggo::set3<T>    sample_point(const ggo::set3<T> & target_pos, T random_variable1, T random_variable2) const { return ggo::set3<T>(0, 0, 0); }
    virtual ggo::ray3d<T>   sample_ray(T random_variable1, T random_variable2) const { return ggo::ray3d<T>(); }

    virtual std::string     desc() const = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using raytracable_shape3d_abc_double  = raytracable_shape3d_abc<double>;
  using raytracable_shape3d_abc_float   = raytracable_shape3d_abc<float>;
}

//////////////////////////////////////////////////////////////
// I/O
namespace ggo
{
  template <typename T>
  std::ostream & operator<<(std::ostream & os, const raytracable_shape3d_abc<T> & shape)
  {
    os << shape.desc();
    return os;
  }
}

#include <ggo_basis3d.h>
#include <ggo_centered_sphere3d.h>
#include <ggo_sphere3d.h>
#include <ggo_segment3d.h>
#include <ggo_triangle3d.h>
#include <ggo_face3d.h>
#include <ggo_axis_aligned_box3d.h>
#include <ggo_polygon3d.h>
#include <ggo_plane3d.h>
#include <ggo_disc3d.h>
#include <ggo_parallelogram3d.h>
#include <ggo_metaball.h>

#include <ggo_basis3d.imp.h>
#include <ggo_centered_sphere3d.imp.h>
#include <ggo_sphere3d.imp.h>
#include <ggo_polygon3d.imp.h>
#include <ggo_plane3d.imp.h>
#include <ggo_face3d.imp.h>
#include <ggo_disc3d.imp.h>
#include <ggo_triangle3d.imp.h>
#include <ggo_axis_aligned_box3d.imp.h>
#include <ggo_parallelogram3d.imp.h>
#include <ggo_metaball.imp.h>

#endif
