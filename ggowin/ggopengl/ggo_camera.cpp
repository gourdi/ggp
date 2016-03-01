#include "ggo_camera.h"

// When we apply a transform on the camera, the inverse transform is in fact
// applied to the world. his is why :
// - Rotation angles and translation vector are inverted.
// - Matrices multiplication order is inverted too.

//////////////////////////////////////////////////////////////
void ggo_camera::set_projection(float fovy, float aspect, float near, float far)
{
  _projection_matrix.set_projection(fovy, aspect, near, far);
}

//////////////////////////////////////////////////////////////
void ggo_camera::set_position(float dx, float dy, float dz)
{
  _view_matrix.set_translation(-dx, -dy, -dz); // Invert offset.
}

//////////////////////////////////////////////////////////////
void ggo_camera::rotate_x(float angle)
{
  ggo_matrix4x4 rotation;
  rotation.set_rotation_x(-angle); // Invert angle

  _view_matrix = _view_matrix * rotation; // Invert multiplication's order.
}

//////////////////////////////////////////////////////////////
void ggo_camera::rotate_y(float angle)
{
  ggo_matrix4x4 rotation;
  rotation.set_rotation_y(-angle); // Invert angle

  _view_matrix = _view_matrix * rotation; // Invert multiplication's order.
}

//////////////////////////////////////////////////////////////
void ggo_camera::rotate_z(float angle)
{
  ggo_matrix4x4 rotation;
  rotation.set_rotation_z(-angle); // Invert angle

  _view_matrix = _view_matrix * rotation; // Invert multiplication's order.
}

//////////////////////////////////////////////////////////////
void ggo_camera::rotate(const ggo::vector3d_float & axis, float angle)
{
  ggo_matrix4x4 rotation;
  rotation.set_rotation(axis, -angle); // Invert angle

  _view_matrix = _view_matrix * rotation; // Invert multiplication's order.
}

//////////////////////////////////////////////////////////////
const ggo_matrix4x4 & ggo_camera::get_view_matrix() const
{
  return _view_matrix;
}

//////////////////////////////////////////////////////////////
const ggo_matrix4x4 & ggo_camera::get_projection_matrix() const
{
  return _projection_matrix;
}
