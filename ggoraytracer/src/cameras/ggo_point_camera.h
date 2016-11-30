#ifndef __GGO_POINT_CAMERA_ABC__
#define __GGO_POINT_CAMERA_ABC__

#include <ggo_camera_abc.h>

// The camera is looking in the Z negative direction.

//////////////////////////////////////////////////////////////
namespace ggo
{
  class point_camera : public camera_abc
  {
  public:

            point_camera(int width, int height);
      
    void    set_aperture(float aperture) { _aperture = std::max(0.001f, aperture); }
    float   get_aperture() const { return _aperture; }
    float & get_aperture() { return _aperture; }
    
  protected:
    
    float _aperture = 0.5f;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class mono_sampling_point_camera : public point_camera, public mono_sampling_camera_abc
  {
  public:
    
                      mono_sampling_point_camera(int width, int height);

  private:

    ggo::ray3d_float  get_ray(int x, int y) const override;
    
  private:

    float       _offset_x = 0;
    float       _offset_y = 0;
    float       _opti = 0;
    ggo::pos3f  _center_focus_point;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class antialiasing_point_camera : public point_camera, public antialiasing_camera_abc
  {
  public:
    
                                      antialiasing_point_camera(int width, int height);

  private:

    std::array<ggo::ray3d_float, 4>   get_first_pass_rays(int x, int y) const override;
    std::array<ggo::ray3d_float, 12>  get_second_pass_rays(int x, int y) const override;

  private:

    float       _offset_x = 0;
    float       _offset_y = 0;
    float       _opti = 0;
    ggo::pos3f  _center_focus_point;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class multi_sampling_point_camera : public point_camera, public multi_sampling_camera_abc
  {
  public:
    
          multi_sampling_point_camera(int width, int height);
                                            
    void  set_depth_of_field_factor(float v) { _depth_of_field_factor = std::max(0.f, v); }
    void  set_depth_of_field(float v) { _depth_of_field = std::max(0.001f, v); }

    float get_depth_of_field_factor() const { return _depth_of_field_factor; }
    float get_depth_of_field() const { return _depth_of_field; }

  private:

    std::vector<ggo::ray3d_float> get_rays(int x, int y, int samples_count) const override;
    
  private:

    float _depth_of_field = 1;
    float _depth_of_field_factor = 0;
    
    float       _offset_x = 0;
    float       _offset_y = 0;
    float       _opti = 0;
    ggo::pos3f  _center_focus_point;
  };
}

#endif

