#ifndef __GGO_ISOMETRIC_CAMERA__
#define __GGO_ISOMETRIC_CAMERA__

#include <raytracer/cameras/ggo_camera_abc.h>

// The camera is looking in the Z negative direction.

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class isometric_camera : public camera_abc
  {
  public:

          isometric_camera(int width, int height, const ggo::basis3d_float & basis, float span);

  protected:

    const float _span;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class mono_sampling_isometric_camera : public isometric_camera, public mono_sampling_camera_abc
  {
  public:

                      mono_sampling_isometric_camera(int width, int height, const ggo::basis3d_float & basis, float span);
    
  private:

    ggo::ray3d_float  get_ray(int x, int y) const override;
    
  private:
    
    const float _offset_x;
    const float _offset_y;
    const float _ratio;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class multi_sampling_isometric_camera : public isometric_camera, public multi_sampling_camera_abc
  {
  public:

                                  multi_sampling_isometric_camera(int width, int height, const ggo::basis3d_float & basis, float span, float depth_of_field, float depth_of_field_factor);

  private:

    std::vector<ggo::ray3d_float> get_rays(int x, int y, int samples_count) const override;
    
  private:

    const float _depth_of_field;
    const float _depth_of_field_factor;

    const float _offset_x;
    const float _offset_y;
    const float _ratio;
  };
}

#endif

