#ifndef __GGO_CAMERA__
#define __GGO_CAMERA__

#include "ggo_matrix4x4.h"
#include <ggo_set3.h>

class ggo_camera
{
public:

  void                  set_projection(float fovy, float aspect, float near, float far);
  void                  set_position(float dx, float dy, float dz);

  void                  rotate_x(float angle);
  void                  rotate_y(float angle);
  void                  rotate_z(float angle);
  void                  rotate(const ggo::vector3d_float & axis, float angle);

  const ggo_matrix4x4 & get_view_matrix() const;
  const ggo_matrix4x4 & get_projection_matrix() const;

private:

  ggo_matrix4x4 _view_matrix;
  ggo_matrix4x4 _projection_matrix;
};

#endif
