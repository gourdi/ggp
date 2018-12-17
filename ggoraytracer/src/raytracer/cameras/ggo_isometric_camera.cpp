#include "ggo_isometric_camera.h"
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_halton.h>

//////////////////////////////////////////////////////////////
// ggo_isometric_camera
namespace ggo
{
  isometric_camera::isometric_camera(int width, int height, const ggo::basis3d_f & basis, float span)
  :
  camera_abc(width, height, basis),
  _span(span)
  {
  }
}

//////////////////////////////////////////////////////////////
// ggo_mono_sampling_isometric_camera
namespace ggo
{
  mono_sampling_isometric_camera::mono_sampling_isometric_camera(int width, int height, const ggo::basis3d_f & basis, float span)
  :
  isometric_camera(width, height, basis, span),
  _offset_x(0.5f - 0.5f * height - 0.5f * (width - height)),
  _offset_y(0.5f - 0.5f * height),
  _ratio(span / (0.5f * height))
  {

  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_f mono_sampling_isometric_camera::get_ray(int x, int y) const
  {
    ggo::pos2_f pixel_offset((x + _offset_x) * _ratio, (y + _offset_y) * _ratio);

    // Ray position.
    ggo::pos3_f pos(_basis.pos());
    pos += pixel_offset.x() * _basis.x();
    pos += pixel_offset.y() * _basis.y();
      
    return ggo::ray3d_f(pos, -_basis.z(), false);
  }
}

//////////////////////////////////////////////////////////////
// ggo_multi_sampling_isometric_camera
namespace ggo
{
  multi_sampling_isometric_camera::multi_sampling_isometric_camera(int width, int height, const ggo::basis3d_f & basis, float span, float depth_of_field, float depth_of_field_factor)
  :
  isometric_camera(width, height, basis, span),
  _depth_of_field(depth_of_field),
  _depth_of_field_factor(depth_of_field_factor),
  _offset_x(0.5f - 0.5f * height - 0.5f * (width - height)),
  _offset_y(0.5f - 0.5f * height),
  _ratio(span / (0.5f * height))
  {

  }

  //////////////////////////////////////////////////////////////
  std::vector<ggo::ray3d_f> multi_sampling_isometric_camera::get_rays(int x, int y, int samples_count) const
  {    
    // Sample disc (radius=0.5) and rectangle (side=1).
    std::vector<ggo::pos2_f> eye_samples2d(ggo::halton_disc_2d_table_2_3, ggo::halton_disc_2d_table_2_3 + samples_count);
    std::vector<ggo::pos2_f> focus_samples2d(ggo::halton_rect_2d_table_2_3, ggo::halton_rect_2d_table_2_3 + samples_count);
    
    // Compute data that does not depend on sample index.
    ggo::pos2_f pixel_offset((x + _offset_x) * _ratio, (y + _offset_y) * _ratio);
    
    ggo::pos3_f focus(_basis.pos() - _depth_of_field * _basis.z());
    focus += pixel_offset.x() * _basis.x();
    focus += pixel_offset.y() * _basis.y();
    
    ggo::pos3_f eye(_basis.pos());
    eye += pixel_offset.x() * _basis.x();
    eye += pixel_offset.y() * _basis.y();
    
    // Build the rays.
    std::vector<ggo::ray3d_f> rays;
    
    for (int i = 0; i < samples_count; ++i)
    {
      // Focus point.
      ggo::pos3_f focus_point(focus);
      focus_point += (_ratio * focus_samples2d[i].x()) * _basis.x();
      focus_point += (_ratio * focus_samples2d[i].y()) * _basis.y();
      
      // Eye point.
      ggo::pos3_f eye_point(eye);
      eye_point += (_depth_of_field_factor * eye_samples2d[i].x()) * _basis.x();
      eye_point += (_depth_of_field_factor * eye_samples2d[i].y()) * _basis.y();

      rays.emplace_back(eye_point, focus_point - eye_point);
    }

    return rays;
  }
}
