#include "ggo_point_camera.h"
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_halton.h>

//////////////////////////////////////////////////////////////
// ggo_point_camera
namespace ggo
{
  point_camera::point_camera(int width, int height, const ggo::basis3d_float & basis, float aperture)
  :
  camera_abc(width, height, basis),
  _aperture(aperture)
  {
    if (aperture <= 0)
    {
      throw std::runtime_error("invalid aperture");
    }
  }
}

//////////////////////////////////////////////////////////////
// ggo_mono_sampling_point_camera
namespace ggo
{
  mono_sampling_point_camera::mono_sampling_point_camera(int width, int height, const ggo::basis3d_float & basis, float aperture)
  :
  point_camera(width, height, basis, aperture),
  _center_focus_point(basis.pos() - basis.z()), // Don't forget the camera is looking in the z negative direction.
  _offset_x(0.5f - 0.5f * height - 0.5f * (width - height)),
  _offset_y(0.5f - 0.5f * height),
  _opti(aperture / (0.5f * height))
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::ray3d_float mono_sampling_point_camera::get_ray(int x, int y) const
  {
    float x_f = x + _offset_x;
    float y_f = y + _offset_y;

    // Focus point.
    ggo::vec3f dx = _basis.x() * (x_f * _opti);
    ggo::vec3f dy = _basis.y() * (y_f * _opti);
    ggo::pos3f focus_point(_center_focus_point + dx + dy);

    // Eye point.
    const ggo::pos3f & eye_point = _basis.pos();

    return ggo::ray3d_float(eye_point, focus_point - eye_point);
  }
}

//////////////////////////////////////////////////////////////
// ggo_antialiasing_point_camera
namespace ggo
{
  antialiasing_point_camera::antialiasing_point_camera(int width, int height, const ggo::basis3d_float & basis, float aperture)
  :
  point_camera(width, height, basis, aperture),
  _center_focus_point(basis.pos() - basis.z()), // Don't forget the camera is looking in the z negative direction.
  _offset_x(0.5f - 0.5f * height - 0.5f * (width - height)),
  _offset_y(0.5f - 0.5f * height),
  _opti(aperture / (0.5f * height))
  {
    
  }

  //////////////////////////////////////////////////////////////
  std::array<ggo::ray3d_float, 4> antialiasing_point_camera::get_first_pass_rays(int x, int y) const
  {
    ggo::vec3f x1 = _basis.x() * ((x + _offset_x - 3 / 8.f) * _opti);
    ggo::vec3f x4 = _basis.x() * ((x + _offset_x + 3 / 8.f) * _opti);
    ggo::vec3f y1 = _basis.y() * ((y + _offset_y - 3 / 8.f) * _opti);
    ggo::vec3f y4 = _basis.y() * ((y + _offset_y + 3 / 8.f) * _opti);

    // Focus points.
    ggo::pos3f focus_point11(_center_focus_point + x1 + y1);
    ggo::pos3f focus_point41(_center_focus_point + x4 + y1);
    
    ggo::pos3f focus_point14(_center_focus_point + x1 + y4);
    ggo::pos3f focus_point44(_center_focus_point + x4 + y4);

    // Eye point.
    const ggo::pos3f & eye_point = _basis.pos();

    return std::array<ggo::ray3d_float, 4> {ggo::ray3d_float(eye_point, focus_point11 - eye_point), 
                                           ggo::ray3d_float(eye_point, focus_point41 - eye_point),
                                           ggo::ray3d_float(eye_point, focus_point14 - eye_point),
                                           ggo::ray3d_float(eye_point, focus_point44 - eye_point)};
  }

  //////////////////////////////////////////////////////////////
  std::array<ggo::ray3d_float, 12> antialiasing_point_camera::get_second_pass_rays(int x, int y) const
  {
    ggo::vec3f x1 = _basis.x() * ((x + _offset_x - 3 / 8.f) * _opti);
    ggo::vec3f x2 = _basis.x() * ((x + _offset_x - 1 / 8.f) * _opti);
    ggo::vec3f x3 = _basis.x() * ((x + _offset_x + 1 / 8.f) * _opti);
    ggo::vec3f x4 = _basis.x() * ((x + _offset_x + 3 / 8.f) * _opti);
    
    ggo::vec3f y1 = _basis.y() * ((y + _offset_y - 3 / 8.f) * _opti);
    ggo::vec3f y2 = _basis.y() * ((y + _offset_y - 1 / 8.f) * _opti);
    ggo::vec3f y3 = _basis.y() * ((y + _offset_y + 1 / 8.f) * _opti);
    ggo::vec3f y4 = _basis.y() * ((y + _offset_y + 3 / 8.f) * _opti);

    // Focus points.
    ggo::pos3f focus_point21(_center_focus_point + x2 + y1);
    ggo::pos3f focus_point31(_center_focus_point + x3 + y1);
    
    ggo::pos3f focus_point12(_center_focus_point + x1 + y2);
    ggo::pos3f focus_point22(_center_focus_point + x2 + y2);
    ggo::pos3f focus_point32(_center_focus_point + x3 + y2);
    ggo::pos3f focus_point42(_center_focus_point + x4 + y2);
    
    ggo::pos3f focus_point13(_center_focus_point + x1 + y3);
    ggo::pos3f focus_point23(_center_focus_point + x2 + y3);
    ggo::pos3f focus_point33(_center_focus_point + x3 + y3);
    ggo::pos3f focus_point43(_center_focus_point + x4 + y3);
    
    ggo::pos3f focus_point24(_center_focus_point + x2 + y4);
    ggo::pos3f focus_point34(_center_focus_point + x3 + y4);

    // Eye point.
    const ggo::pos3f & eye_point = _basis.pos();

    return std::array<ggo::ray3d_float, 12> {ggo::ray3d_float(eye_point, focus_point21 - eye_point), 
                                             ggo::ray3d_float(eye_point, focus_point31 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point12 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point22 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point32 - eye_point), 
                                             ggo::ray3d_float(eye_point, focus_point42 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point13 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point23 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point33 - eye_point), 
                                             ggo::ray3d_float(eye_point, focus_point43 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point24 - eye_point),
                                             ggo::ray3d_float(eye_point, focus_point34 - eye_point)};
  }
}

//////////////////////////////////////////////////////////////
// ggo_multi_sampling_point_camera
namespace ggo
{
  multi_sampling_point_camera::multi_sampling_point_camera(int width, int height, const ggo::basis3d_float & basis, float aperture, float depth_of_field, float depth_of_field_factor)
  :
  point_camera(width, height, basis, aperture),
  _depth_of_field(depth_of_field),
  _depth_of_field_factor(depth_of_field_factor),
  _center_focus_point(basis.pos() - depth_of_field * basis.z()), // Don't forget the camera is looking in the z negative direction.
  _offset_x(0.5f - 0.5f * height - 0.5f * (width - height)),
  _offset_y(0.5f - 0.5f * height),
  _opti(depth_of_field * aperture / (0.5f * height))
  {

  }

  //////////////////////////////////////////////////////////////
  std::vector<ggo::ray3d_float> multi_sampling_point_camera::get_rays(int x, int y, int samples_count) const
  {
    // Sample disc (radius=0.5) and rectangle (side=1).
    std::vector<ggo::pos2f> eye_samples2d(ggo::halton_disc_2d_table_2_3, ggo::halton_disc_2d_table_2_3 + samples_count);
    std::vector<ggo::pos2f> focus_samples2d(ggo::halton_rect_2d_table_2_3, ggo::halton_rect_2d_table_2_3 + samples_count);

    float x_f = x + _offset_x;
    float y_f = y + _offset_y;
    
    std::vector<ggo::ray3d_float> rays;

    for (int i = 0; i < samples_count; ++i)
    {
      // Focus point.
      ggo::pos3f focus_point(_center_focus_point);
      focus_point += ((x_f + focus_samples2d[i].x()) * _opti) * _basis.x();
      focus_point += ((y_f + focus_samples2d[i].y()) * _opti) * _basis.y();

      // Eye point.
      ggo::pos3f eye_point(_basis.pos());
      eye_point += (_depth_of_field_factor * eye_samples2d[i].x()) * _basis.x();
      eye_point += (_depth_of_field_factor * eye_samples2d[i].y()) * _basis.y();

      rays.emplace_back(eye_point, focus_point - eye_point);
    }

    return rays;
  }
}

