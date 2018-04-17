#ifndef __GGO_SHAPES3D__
#define __GGO_SHAPES3D__

#include <sstream>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_vec.h>
#include <kernel/math/ggo_distance.h>
#include <kernel/math/shapes_3d/ggo_ray3d.h>
#include <kernel/math/shapes_3d/ggo_basis3d.h>
#include <kernel/math/shapes_3d/ggo_box3d_data.h>

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

    using type = data_t;

    virtual                    ~raytracable_shape3d_abc() {}

    virtual bool                intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const = 0;
    virtual bool                is_convex() const = 0;

    virtual ggo::pos3<data_t>   sample_point(const ggo::pos3<data_t> & target_pos, data_t random_variable1, data_t random_variable2) const { return ggo::pos3<data_t>(data_t(0), data_t(0), data_t(0)); }
    virtual ggo::ray3d<data_t>  sample_ray(data_t random_variable1, data_t random_variable2) const { return ggo::ray3d<data_t>(); }

    virtual std::optional<box3d_data<data_t>>  get_bounding_box(const ggo::basis3d<data_t> & basis) const = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using raytracable_shape3d_abc_double  = raytracable_shape3d_abc<double>;
  using raytracable_shape3d_abc_float   = raytracable_shape3d_abc<float>;
}

#include <kernel/math/shapes_3d/ggo_centered_sphere3d.h>
#include <kernel/math/shapes_3d/ggo_sphere3d.h>
#include <kernel/math/shapes_3d/ggo_segment3d.h>
#include <kernel/math/shapes_3d/ggo_triangle3d.h>
#include <kernel/math/shapes_3d/ggo_face3d.h>
#include <kernel/math/shapes_3d/ggo_box3d.h>
#include <kernel/math/shapes_3d/ggo_polygon3d.h>
#include <kernel/math/shapes_3d/ggo_plane3d.h>
#include <kernel/math/shapes_3d/ggo_disc3d.h>
#include <kernel/math/shapes_3d/ggo_parallelogram3d.h>
#include <kernel/math/shapes_3d/ggo_metaball.h>
#include <kernel/math/shapes_3d/ggo_line3d.h>
#include <kernel/math/shapes_3d/ggo_cylinder3d.h>
#include <kernel/math/shapes_3d/ggo_influence_plane3d.h>
#include <kernel/math/shapes_3d/ggo_rectangle3d.h>

#include <kernel/math/shapes_3d/ggo_basis3d.imp.h>
#include <kernel/math/shapes_3d/ggo_centered_sphere3d.imp.h>
#include <kernel/math/shapes_3d/ggo_sphere3d.imp.h>
#include <kernel/math/shapes_3d/ggo_polygon3d.imp.h>
#include <kernel/math/shapes_3d/ggo_plane3d.imp.h>
#include <kernel/math/shapes_3d/ggo_face3d.imp.h>
#include <kernel/math/shapes_3d/ggo_disc3d.imp.h>
#include <kernel/math/shapes_3d/ggo_triangle3d.imp.h>
#include <kernel/math/shapes_3d/ggo_box3d.imp.h>
#include <kernel/math/shapes_3d/ggo_parallelogram3d.imp.h>
#include <kernel/math/shapes_3d/ggo_metaball.imp.h>
#include <kernel/math/shapes_3d/ggo_line3d.imp.h>
#include <kernel/math/shapes_3d/ggo_cylinder3d.imp.h>
#include <kernel/math/shapes_3d/ggo_influence_plane3d.imp.h>
#include <kernel/math/shapes_3d/ggo_rectangle3d.imp.h>

#endif
