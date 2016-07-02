#include "ggo_isometric_camera.h"
#include <ggo_halton.h>

//////////////////////////////////////////////////////////////
// ggo_isometric_camera
namespace ggo
{
  isometric_camera::isometric_camera(int width, int height)
  :
  camera_abc(width, height)
  {
  }
}

//////////////////////////////////////////////////////////////
// ggo_mono_sampling_isometric_camera
namespace ggo
{
  mono_sampling_isometric_camera::mono_sampling_isometric_camera(int width, int height)
  :
  isometric_camera(width, height)
  {
  }

  //////////////////////////////////////////////////////////////
  void mono_sampling_isometric_camera::on_start_rendering_frame()
  {
    _offset_x = 0.5f - 0.5f * _height - 0.5f * (_width - _height);
    _offset_y = 0.5f - 0.5f * _height;
    _ratio = _span / (0.5f * _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float mono_sampling_isometric_camera::get_ray(int x, int y) const
  {
    ggo::pos2f pixel_offset((x + _offset_x) * _ratio, (y + _offset_y) * _ratio);

    // Ray position.
    ggo::pos3f pos(_basis.pos());
    pos += pixel_offset.get<0>() * _basis.x();
    pos += pixel_offset.get<1>() * _basis.y();
      
    return ggo::ray3d_float(pos, -_basis.z(), false);
  }
}

//////////////////////////////////////////////////////////////
// ggo_multi_sampling_isometric_camera
namespace ggo
{
  multi_sampling_isometric_camera::multi_sampling_isometric_camera(int width, int height)
  :
  isometric_camera(width, height)
  {
  }

  //////////////////////////////////////////////////////////////
  void multi_sampling_isometric_camera::on_start_rendering_frame()
  {
    _offset_x = 0.5f - 0.5f * _height - 0.5f * (_width - _height);
    _offset_y = 0.5f - 0.5f * _height;
    _ratio = _span / (0.5f * _height);
  }

  //////////////////////////////////////////////////////////////
  std::vector<ggo::ray3d_float> multi_sampling_isometric_camera::get_rays(int x, int y, int samples_count) const
  {    
    // Sample disc (radius=0.5) and rectangle (side=1).
    std::vector<ggo::pos2f> eye_samples2d(ggo::halton_disc_2d_table_2_3, ggo::halton_disc_2d_table_2_3 + samples_count);
    std::vector<ggo::pos2f> focus_samples2d(ggo::halton_rect_2d_table_2_3, ggo::halton_rect_2d_table_2_3 + samples_count);
    
    // Compute data that does not depend on sample index.
    ggo::pos2f pixel_offset((x + _offset_x) * _ratio, (y + _offset_y) * _ratio);
    
    ggo::pos3f focus(_basis.pos() - _depth_of_field * _basis.z());
    focus += pixel_offset.get<0>() * _basis.x();
    focus += pixel_offset.get<1>() * _basis.y();
    
    ggo::pos3f eye(_basis.pos());
    eye += pixel_offset.get<0>() * _basis.x();
    eye += pixel_offset.get<1>() * _basis.y();
    
    // Build the rays.
    std::vector<ggo::ray3d_float> rays;
    
    for (int i = 0; i < samples_count; ++i)
    {
      // Focus point.
      ggo::pos3f focus_point(focus);
      focus_point += (_ratio * focus_samples2d[i].get<0>()) * _basis.x();
      focus_point += (_ratio * focus_samples2d[i].get<1>()) * _basis.y();
      
      // Eye point.
      ggo::pos3f eye_point(eye);
      eye_point += (_depth_of_field_factor * eye_samples2d[i].get<0>()) * _basis.x();
      eye_point += (_depth_of_field_factor * eye_samples2d[i].get<1>()) * _basis.y();

      rays.emplace_back(eye_point, focus_point - eye_point);
    }

    return rays;
  }
}
