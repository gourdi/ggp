#ifndef __GGO_POINT_CAMERA_ABC__
#define __GGO_POINT_CAMERA_ABC__

#include <raytracer/cameras/ggo_camera_abc.h>

// The camera is looking in the Z negative direction.

//////////////////////////////////////////////////////////////
namespace ggo
{
  class point_camera : public camera_abc
  {
  public:

            point_camera(int width, int height, const ggo::basis3d_f & basis, float aperture);
      
    float   get_aperture() const { return _aperture; }
    
  protected:
    
    const float _aperture;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class mono_sampling_point_camera : public point_camera, public mono_sampling_camera_abc
  {
  public:
    
                      mono_sampling_point_camera(int width, int height, const ggo::basis3d_f & basis, float aperture);

  private:

    ggo::ray3d_f  get_ray(int x, int y) const override;
    
  private:

    const float       _offset_x = 0;
    const float       _offset_y = 0;
    const float       _opti = 0;
    const ggo::pos3_f  _center_focus_point;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class antialiasing_point_camera : public point_camera, public antialiasing_camera_abc
  {
  public:
    
                                      antialiasing_point_camera(int width, int height, const ggo::basis3d_f & basis, float aperture);

  private:

    std::array<ggo::ray3d_f, 4>   get_first_pass_rays(int x, int y) const override;
    std::array<ggo::ray3d_f, 12>  get_second_pass_rays(int x, int y) const override;

  private:

    const float       _offset_x;
    const float       _offset_y;
    const float       _opti;
    const ggo::pos3_f  _center_focus_point;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class multi_sampling_point_camera : public point_camera, public multi_sampling_camera_abc
  {
  public:
    
          multi_sampling_point_camera(int width, int height, const ggo::basis3d_f & basis, float aperture, float depth_of_field, float depth_of_field_factor);

    float get_depth_of_field_factor() const { return _depth_of_field_factor; }
    float get_depth_of_field() const { return _depth_of_field; }

  private:

    std::vector<ggo::ray3d_f> get_rays(int x, int y, int samples_count) const override;
    
  private:

    const float _depth_of_field;
    const float _depth_of_field_factor;
    
    const float       _offset_x;
    const float       _offset_y;
    const float       _opti;
    const ggo::pos3_f  _center_focus_point;
  };
}

#endif

