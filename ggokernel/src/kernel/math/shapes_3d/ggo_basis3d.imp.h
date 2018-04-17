#include <kernel/math/linear_algebra/ggo_linear_algebra3d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  basis3d<data_t>::basis3d()
  :
  _pos(data_t(0), data_t(0), data_t(0)),
  _x(data_t(1), data_t(0), data_t(0)),
  _y(data_t(0), data_t(1), data_t(0)),
  _z(data_t(0), data_t(0), data_t(1))
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  basis3d<data_t>::basis3d(const ggo::pos3<data_t> & pos)
  :
  _pos(pos),
  _x(data_t(1), data_t(0), data_t(0)),
  _y(data_t(0), data_t(1), data_t(0)),
  _z(data_t(0), data_t(0), data_t(1))
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::reset()
  {
    _pos = ggo::pos3<data_t>(data_t(0), data_t(0), data_t(0));
    _x = ggo::vec3<data_t>(data_t(1), data_t(0), data_t(0));
    _y = ggo::vec3<data_t>(data_t(0), data_t(1), data_t(0));
    _z = ggo::vec3<data_t>(data_t(0), data_t(0), data_t(1));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::rotate_x(data_t angle)
  {
    ggo::vec3<data_t>	y(_y);
    ggo::vec3<data_t>	z(_z);
    data_t cos_a = cos(angle);
    data_t sin_a = sin(angle);

    _y = ggo::vec3<data_t>(
        y.x()*cos_a + z.x()*sin_a,
        y.y()*cos_a + z.y()*sin_a,
        y.z()*cos_a + z.z()*sin_a);

    _z = ggo::vec3<data_t>(
        -y.x()*sin_a + z.x()*cos_a,
        -y.y()*sin_a + z.y()*cos_a,
        -y.z()*sin_a + z.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::rotate_y(data_t angle)
  {
    ggo::vec3<data_t>	x(_x);
    ggo::vec3<data_t>	z(_z);
    data_t cos_a = cos(angle);
    data_t sin_a = sin(angle);

    _x = ggo::vec3<data_t>(
        x.x()*cos_a + z.x()*sin_a,
        x.y()*cos_a + z.y()*sin_a,
        x.z()*cos_a + z.z()*sin_a);

    _z = ggo::vec3<data_t>(
        -x.x()*sin_a + z.x()*cos_a,
        -x.y()*sin_a + z.y()*cos_a,
        -x.z()*sin_a + z.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::rotate_z(data_t angle)
  {
    ggo::vec3<data_t>	x(_x);
    ggo::vec3<data_t>	y(_y);
    data_t cos_a = cos(angle);
    data_t sin_a = sin(angle);

    _x = ggo::vec3<data_t>(
        x.x()*cos_a + y.x()*sin_a,
        x.y()*cos_a + y.y()*sin_a,
        x.z()*cos_a + y.z()*sin_a);

    _y = ggo::vec3<data_t>(
        -x.x()*sin_a + y.x()*cos_a,
        -x.y()*sin_a + y.y()*cos_a,
        -x.z()*sin_a + y.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::reset_rotation()
  {
    _x = ggo::vec3<data_t>(1, 0, 0);
    _y = ggo::vec3<data_t>(0, 1, 0);
    _z = ggo::vec3<data_t>(0, 0, 1);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename data_t>
  ggo::pos3<data_t> basis3d<data_t>::point_from_local_to_world(const ggo::pos3<data_t> & point) const
  {
    return vector_from_local_to_world(point) + _pos;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename data_t>
  ggo::pos3<data_t> basis3d<data_t>::point_from_world_to_local(const ggo::pos3<data_t> & point) const
  {
    return vector_from_world_to_local(point - _pos);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename data_t>
  ggo::vec3<data_t> basis3d<data_t>::vector_from_local_to_world(const ggo::vec3<data_t> & vector) const
  {
    ggo::vec3<data_t>	result;

    result.x() = _x.x()*vector.x() + _y.x()*vector.y() + _z.x()*vector.z();
    result.y() = _x.y()*vector.x() + _y.y()*vector.y() + _z.y()*vector.z();
    result.z() = _x.z()*vector.x() + _y.z()*vector.y() + _z.z()*vector.z();

    return result;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename data_t>
  ggo::vec3<data_t> basis3d<data_t>::vector_from_world_to_local(const ggo::vec3<data_t> & vector) const
  {
    ggo::vec3<data_t>	result;

    result.x() = _x.x()*vector.x() + _x.y()*vector.y() + _x.z()*vector.z();
    result.y() = _y.x()*vector.x() + _y.y()*vector.y() + _y.z()*vector.z();
    result.z() = _z.x()*vector.x() + _z.y()*vector.y() + _z.z()*vector.z();

    return result;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename data_t>
  ggo::ray3d<data_t> basis3d<data_t>::ray_from_local_to_world(const ggo::ray3d<data_t> & ray) const
  {
    return ggo::ray3d<data_t>(point_from_local_to_world(ray.pos()), vector_from_local_to_world(ray.dir()), false);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename data_t>
  ggo::ray3d<data_t> basis3d<data_t>::ray_from_world_to_local(const ggo::ray3d<data_t> & ray) const
  {
    return ggo::ray3d<data_t>(point_from_world_to_local(ray.pos()), vector_from_world_to_local(ray.dir()), false);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::rotate(const ggo::vec3<data_t> & axis, data_t angle)
  {
    data_t	m[3][3];

    ggo::fill_rotation_matrix(axis, angle, m);

    _x = ggo::apply_matrix3d(m, _x);
    _y = ggo::apply_matrix3d(m, _y);
    _z = ggo::apply_matrix3d(m, _z);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::rotate(const ggo::ray3d<data_t> & axis, data_t angle)
  {
    data_t	m[3][3];

    ggo::fill_rotation_matrix(axis.dir(), angle, m);

    // Rotate the directions.
    _x		= ggo::apply_matrix3d(m, _x);
    _y		= ggo::apply_matrix3d(m, _y);
    _z		= ggo::apply_matrix3d(m, _z);

    // Rotate the position.
    _pos	-= axis.pos();
    _pos	= ggo::apply_matrix3d( m, _pos );
    _pos	+= axis.pos();
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void basis3d<data_t>::look_at(const ggo::pos3<data_t> & at)
  {
    _z = _pos - at;
    _z.normalize();
    auto b = build_basis(_z);
    _x = b.first;
    _y = b.second;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> basis3d<data_t>::project(const ggo::pos3<data_t> & p, data_t aperture, int screen_width, int screen_height) const
  {
    ggo::vec3<data_t> diff(p - _pos);

    // Origin unoptimized code.
    // T dot_z = -ggo::dot(diff, _z); 
    // T y = T(0.5) * screen_height + T(0.5) * screen_height * ggo::dot(diff, _y) / (dot_z * aperture);
    // T x = T(0.5) * screen_height + T(0.5) * screen_height * ggo::dot(diff, _x) / (dot_z * aperture) + T(0.5) * (screen_width - screen_height);

    // Optimized code.
    data_t den = -1 / (ggo::dot(diff, _z) * aperture); // The minus sign is here because the camera is looking is the negative z-axis.
    ggo::pos2<data_t> proj(1 + ggo::dot(diff, _x) * den, 1 + ggo::dot(diff, _y) * den);
    proj *= static_cast<data_t>(screen_height);
    proj.x() += static_cast<data_t>(screen_width - screen_height);
    proj /= 2;
    
    return proj;
  }
}

