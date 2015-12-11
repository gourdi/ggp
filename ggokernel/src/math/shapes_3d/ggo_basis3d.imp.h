#include <ggo_linear_algebra3d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  basis3d<T>::basis3d()
  :
  _pos(0, 0, 0), _x(1,0,0), _y(0,1,0), _z(0,0,1)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::reset()
  {
    _pos = ggo::set3<T>(0, 0, 0);
    _x = ggo::set3<T>(1,0,0);
    _y = ggo::set3<T>(0,1,0);
    _z = ggo::set3<T>(0,0,1);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_x(T angle)
  {
    ggo::set3<T>	y(_y);
    ggo::set3<T>	z(_z);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _y = ggo::set3<T>(
        y.x()*cos_a + z.x()*sin_a,
        y.y()*cos_a + z.y()*sin_a,
        y.z()*cos_a + z.z()*sin_a);

    _z = ggo::set3<T>(
        -y.x()*sin_a + z.x()*cos_a,
        -y.y()*sin_a + z.y()*cos_a,
        -y.z()*sin_a + z.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_y(T angle)
  {
    ggo::set3<T>	x(_x);
    ggo::set3<T>	z(_z);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _x = ggo::set3<T>(
        x.x()*cos_a + z.x()*sin_a,
        x.y()*cos_a + z.y()*sin_a,
        x.z()*cos_a + z.z()*sin_a);

    _z = ggo::set3<T>(
        -x.x()*sin_a + z.x()*cos_a,
        -x.y()*sin_a + z.y()*cos_a,
        -x.z()*sin_a + z.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_z(T angle)
  {
    ggo::set3<T>	x(_x);
    ggo::set3<T>	y(_y);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _x = ggo::set3<T>(
        x.x()*cos_a + y.x()*sin_a,
        x.y()*cos_a + y.y()*sin_a,
        x.z()*cos_a + y.z()*sin_a);

    _y = ggo::set3<T>(
        -x.x()*sin_a + y.x()*cos_a,
        -x.y()*sin_a + y.y()*cos_a,
        -x.z()*sin_a + y.z()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::reset_rotation()
  {
    _x = ggo::set3<T>(1, 0, 0);
    _y = ggo::set3<T>(0, 1, 0);
    _z = ggo::set3<T>(0, 0, 1);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename T>
  ggo::set3<T> basis3d<T>::point_from_local_to_world(const ggo::set3<T> & point) const
  {
    return vector_from_local_to_world(point) + _pos;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename T>
  ggo::set3<T> basis3d<T>::point_from_world_to_local(const ggo::set3<T> & point) const
  {
    return vector_from_world_to_local(point - _pos);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename T>
  ggo::set3<T> basis3d<T>::vector_from_local_to_world(const ggo::set3<T> & vector) const
  {
    ggo::set3<T>	result;

    result.x() = _x.x()*vector.x() + _y.x()*vector.y() + _z.x()*vector.z();
    result.y() = _x.y()*vector.x() + _y.y()*vector.y() + _z.y()*vector.z();
    result.z() = _x.z()*vector.x() + _y.z()*vector.y() + _z.z()*vector.z();

    return result;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename T>
  ggo::set3<T> basis3d<T>::vector_from_world_to_local(const ggo::set3<T> & vector) const
  {
    ggo::set3<T>	result;

    result.x() = _x.x()*vector.x() + _x.y()*vector.y() + _x.z()*vector.z();
    result.y() = _y.x()*vector.x() + _y.y()*vector.y() + _y.z()*vector.z();
    result.z() = _z.x()*vector.x() + _z.y()*vector.y() + _z.z()*vector.z();

    return result;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename T>
  ggo::ray3d<T> basis3d<T>::ray_from_local_to_world(const ggo::ray3d<T> & ray) const
  {
    return ggo::ray3d<T>(point_from_local_to_world(ray.pos()), vector_from_local_to_world(ray.dir()), false);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename T>
  ggo::ray3d<T> basis3d<T>::ray_from_world_to_local(const ggo::ray3d<T> & ray) const
  {
    return ggo::ray3d<T>(point_from_world_to_local(ray.pos()), vector_from_world_to_local(ray.dir()), false);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate(const ggo::set3<T> & axis, T angle)
  {
    T	m[3][3];

    ggo::fill_rotation_matrix(axis, angle, m);

    _x = ggo::apply_matrix3d(m, _x);
    _y = ggo::apply_matrix3d(m, _y);
    _z = ggo::apply_matrix3d(m, _z);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate(const ggo::ray3d<T> & axis, T angle)
  {
    T	m[3][3];

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
  template <typename T>
  void basis3d<T>::look_at(const ggo::set3<T> & at)
  {
    _z = _pos - at;
    _z.normalize();
    _z.build_basis(_x, _y);
  }
}

