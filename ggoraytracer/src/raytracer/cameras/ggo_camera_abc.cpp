#include "ggo_camera_abc.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  camera_abc::camera_abc(int width, int height, const ggo::basis3d_float & basis)
  :
  _width(width),
  _height(height),
  _basis(basis)
  {
  }
}