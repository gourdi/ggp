#ifndef __GGO_SHAPES3D__
#define __GGO_SHAPES3D__

#include <sstream>
#include <ggo_kernel.h>
#include <ggo_vec.h>
#include <ggo_distance.h>
#include <ggo_ray3d.h>
#include <ggo_basis3d.h>

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  struct vertex
  {
    vertex(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & normal) : _pos(pos), _normal(normal) { _normal.normalize(); }

    ggo::pos3<data_t> _pos;
    ggo::vec3<data_t> _normal;
  };
}

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  struct axis_aligned_box3d_data
  {
    axis_aligned_box3d_data(data_t x_min, data_t x_max, data_t y_min, data_t y_max, data_t z_min, data_t z_max)
      :
      _x_min(x_min),
      _x_max(x_max),
      _y_min(y_min),
      _y_max(y_max),
      _z_min(z_min),
      _z_max(z_max)
    {
    }

    static axis_aligned_box3d_data from(std::initializer_list<ggo::pos3<data_t>> points)
    {
      if (points.begin() == points.end())
      {
        throw std::runtime_error("empty points list");
      }

      auto it = points.begin();

      data_t x_min = it->x();
      data_t x_max = it->x();
      data_t y_min = it->y();
      data_t y_max = it->y();
      data_t z_min = it->z();
      data_t z_max = it->z();

      ++it;

      for (; it != points.end(); ++it)
      {
        x_min = std::min(x_min, it->x());
        x_max = std::max(x_max, it->x());
        y_min = std::min(y_min, it->y());
        y_max = std::max(y_max, it->y());
        z_min = std::min(z_min, it->z());
        z_max = std::max(z_max, it->z());
      }

      return { x_min, x_max, y_min, y_max, z_min, z_max };
    }

    data_t	_x_min;
    data_t	_x_max;
    data_t	_y_min;
    data_t	_y_max;
    data_t	_z_min;
    data_t	_z_max;
  };
}

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class influence_shape3d_abc
  {
  public:
      
    virtual                    ~influence_shape3d_abc() {}

    virtual std::vector<data_t> intersect_ray(const ggo::ray3d<data_t> & ray) const = 0;
    virtual data_t              hypot_to_center(const ggo::pos3<data_t> & p) const = 0;
    virtual data_t              get_influence_hypot() const = 0;
  };
}

//////////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class raytracable_shape3d_abc
  {
  public:

    virtual                    ~raytracable_shape3d_abc() {}

    virtual bool                intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const = 0;
    virtual bool                is_convex() const = 0;

    virtual ggo::pos3<data_t>   sample_point(const ggo::pos3<data_t> & target_pos, data_t random_variable1, data_t random_variable2) const { return ggo::pos3<data_t>(data_t(0), data_t(0), data_t(0)); }
    virtual ggo::ray3d<data_t>  sample_ray(data_t random_variable1, data_t random_variable2) const { return ggo::ray3d<data_t>(); }

    virtual std::optional<axis_aligned_box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using raytracable_shape3d_abc_double  = raytracable_shape3d_abc<double>;
  using raytracable_shape3d_abc_float   = raytracable_shape3d_abc<float>;
}

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
#include <ggo_line3d.h>
#include <ggo_cylinder3d.h>
#include <ggo_influence_plane3d.h>
#include <ggo_rectangle3d.h>

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
#include <ggo_line3d.imp.h>
#include <ggo_cylinder3d.imp.h>
#include <ggo_influence_plane3d.imp.h>
#include <ggo_rectangle3d.imp.h>

#endif
