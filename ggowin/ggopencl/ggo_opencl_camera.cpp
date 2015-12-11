#include "ggo_opencl_camera.h"
#include <ggo_array.h>

//////////////////////////////////////////////////////////////
ggo_opencl_camera::ggo_opencl_camera(int width, int height, int samples_count)
:
_camera(width, height)
{
  _camera.set_samples_count(samples_count);
}

//////////////////////////////////////////////////////////////
ggo_point_camera & ggo_opencl_camera::get_camera()
{
  return _camera;
}

//////////////////////////////////////////////////////////////
const ggo_point_camera & ggo_opencl_camera::get_camera() const
{
  return _camera;
}

//////////////////////////////////////////////////////////////
std::vector<float> ggo_opencl_camera::get_params() const
{
  std::vector<float> params;

  _camera.on_start_rendering_frame();

  // Position.
  params.push_back(_camera.basis().pos().x());
  params.push_back(_camera.basis().pos().y());
  params.push_back(_camera.basis().pos().z());

  // Basis x-axis.
  params.push_back(_camera.basis().x().x());
  params.push_back(_camera.basis().x().y());
  params.push_back(_camera.basis().x().z());

  // Basis y-axis.
  params.push_back(_camera.basis().y().x());
  params.push_back(_camera.basis().y().y());
  params.push_back(_camera.basis().y().z());

  // Basis z-axis.
  ggo_point3d_float center_focus_point = _camera.basis().pos() - _camera.basis().z() * _camera.get_depth_of_field();
  params.push_back(center_focus_point.x());
  params.push_back(center_focus_point.y());
  params.push_back(center_focus_point.z());

  // Opti.
  params.push_back(_camera.get_depth_of_field() * _camera.get_aperture() / (0.5f * _camera.get_height()));

  return params;
}
