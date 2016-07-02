#include <ggo_linear_algebra3d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  basis3d<T>::basis3d()
  :
  _pos(T(0), T(0), T(0)),
  _x(T(1), T(0), T(0)),
  _y(T(0), T(1), T(0)),
  _z(T(0), T(0), T(1))
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::reset()
  {
    _pos = ggo::pos3<T>(T(0), T(0), T(0));
    _x = ggo::vec3<T>(T(1), T(0), T(0));
    _y = ggo::vec3<T>(T(0), T(1), T(0));
    _z = ggo::vec3<T>(T(0), T(0), T(1));
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_x(T angle)
  {
    ggo::vec3<T>	y(_y);
    ggo::vec3<T>	z(_z);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _y = ggo::vec3<T>(
        y.template get<0>()*cos_a + z.template get<0>()*sin_a,
        y.template get<1>()*cos_a + z.template get<1>()*sin_a,
        y.template get<2>()*cos_a + z.template get<2>()*sin_a);

    _z = ggo::vec3<T>(
        -y.template get<0>()*sin_a + z.template get<0>()*cos_a,
        -y.template get<1>()*sin_a + z.template get<1>()*cos_a,
        -y.template get<2>()*sin_a + z.template get<2>()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_y(T angle)
  {
    ggo::vec3<T>	x(_x);
    ggo::vec3<T>	z(_z);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _x = ggo::vec3<T>(
        x.template get<0>()*cos_a + z.template get<0>()*sin_a,
        x.template get<1>()*cos_a + z.template get<1>()*sin_a,
        x.template get<2>()*cos_a + z.template get<2>()*sin_a);

    _z = ggo::vec3<T>(
        -x.template get<0>()*sin_a + z.template get<0>()*cos_a,
        -x.template get<1>()*sin_a + z.template get<1>()*cos_a,
        -x.template get<2>()*sin_a + z.template get<2>()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::rotate_z(T angle)
  {
    ggo::vec3<T>	x(_x);
    ggo::vec3<T>	y(_y);
    T			cos_a = cos(angle);
    T			sin_a = sin(angle);

    _x = ggo::vec3<T>(
        x.template get<0>()*cos_a + y.template get<0>()*sin_a,
        x.template get<1>()*cos_a + y.template get<1>()*sin_a,
        x.template get<2>()*cos_a + y.template get<2>()*sin_a);

    _y = ggo::vec3<T>(
        -x.template get<0>()*sin_a + y.template get<0>()*cos_a,
        -x.template get<1>()*sin_a + y.template get<1>()*cos_a,
        -x.template get<2>()*sin_a + y.template get<2>()*cos_a);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void basis3d<T>::reset_rotation()
  {
    _x = ggo::vec3<T>(1, 0, 0);
    _y = ggo::vec3<T>(0, 1, 0);
    _z = ggo::vec3<T>(0, 0, 1);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename T>
  ggo::pos3<T> basis3d<T>::point_from_local_to_world(const ggo::pos3<T> & point) const
  {
    return vector_from_local_to_world(point) + _pos;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename T>
  ggo::pos3<T> basis3d<T>::point_from_world_to_local(const ggo::pos3<T> & point) const
  {
    return vector_from_world_to_local(point - _pos);
  }

  //////////////////////////////////////////////////////////////
  // The argument is in local coordinates and the result is 
  // in world coordinates.
  template <typename T>
  ggo::vec3<T> basis3d<T>::vector_from_local_to_world(const ggo::vec3<T> & vector) const
  {
    ggo::vec3<T>	result;

    result.template get<0>() = _x.template get<0>()*vector.template get<0>() + _y.template get<0>()*vector.template get<1>() + _z.template get<0>()*vector.template get<2>();
    result.template get<1>() = _x.template get<1>()*vector.template get<0>() + _y.template get<1>()*vector.template get<1>() + _z.template get<1>()*vector.template get<2>();
    result.template get<2>() = _x.template get<2>()*vector.template get<0>() + _y.template get<2>()*vector.template get<1>() + _z.template get<2>()*vector.template get<2>();

    return result;
  }

  //////////////////////////////////////////////////////////////
  // The argument is in world coordinates and the result is 
  // in local coordinates.
  template <typename T>
  ggo::vec3<T> basis3d<T>::vector_from_world_to_local(const ggo::vec3<T> & vector) const
  {
    ggo::vec3<T>	result;

    result.template get<0>() = _x.template get<0>()*vector.template get<0>() + _x.template get<1>()*vector.template get<1>() + _x.template get<2>()*vector.template get<2>();
    result.template get<1>() = _y.template get<0>()*vector.template get<0>() + _y.template get<1>()*vector.template get<1>() + _y.template get<2>()*vector.template get<2>();
    result.template get<2>() = _z.template get<0>()*vector.template get<0>() + _z.template get<1>()*vector.template get<1>() + _z.template get<2>()*vector.template get<2>();

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
  void basis3d<T>::rotate(const ggo::vec3<T> & axis, T angle)
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
  void basis3d<T>::look_at(const ggo::pos3<T> & at)
  {
    _z = _pos - at;
    _z.normalize();
    auto b = build_basis(_z);
    _x = b.first;
    _y = b.second;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  ggo::pos2<T> basis3d<T>::project(const ggo::pos3<T> & p, T aperture, int screen_width, int screen_height) const
  {
    ggo::vec3<T> diff(p - _pos);

    // Origin unoptimized code.
    // T dot_z = -ggo::dot(diff, _z); 
    // T y = T(0.5) * screen_height + T(0.5) * screen_height * ggo::dot(diff, _y) / (dot_z * aperture);
    // T x = T(0.5) * screen_height + T(0.5) * screen_height * ggo::dot(diff, _x) / (dot_z * aperture) + T(0.5) * (screen_width - screen_height);

    // Optimized code.
    T den = -1 / (ggo::dot(diff, _z) * aperture); // The minus sign is here because the camera is looking is the negative z-axis.
    ggo::pos2<T> proj(1 + ggo::dot(diff, _x) * den, 1 + ggo::dot(diff, _y) * den);
    proj *= static_cast<T>(screen_height);
    proj.template get<0>() += static_cast<T>(screen_width - screen_height);
    proj /= 2;
    
    return proj;
  }
}

