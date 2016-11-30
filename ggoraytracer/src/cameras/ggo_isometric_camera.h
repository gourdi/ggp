#ifndef __GGO_ISOMETRIC_CAMERA__
#define __GGO_ISOMETRIC_CAMERA__

#include <ggo_camera_abc.h>

// The camera is looking in the Z negative direction.

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class isometric_camera : public camera_abc
  {
  public:

          isometric_camera(int width, int height);

    void	set_span(float span) { _span = std::max(0.001f, span); }

  protected:

    float _span = 1;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class mono_sampling_isometric_camera : public isometric_camera, public mono_sampling_camera_abc
  {
  public:

                      mono_sampling_isometric_camera(int width, int height);
    
  private:

    ggo::ray3d_float  get_ray(int x, int y) const override;
    
  private:
    
    float _offset_x = 0;
    float _offset_y = 0;
    float _ratio = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{  
  class multi_sampling_isometric_camera : public isometric_camera, public multi_sampling_camera_abc
  {
  public:

                                  multi_sampling_isometric_camera(int width, int height);

    void	                        set_depth_of_field_factor(float v) { _depth_of_field_factor = std::max(0.f, v); }
    void	                        set_depth_of_field(float v) { _depth_of_field = std::max(0.001f, v); }
    
  private:

    std::vector<ggo::ray3d_float> get_rays(int x, int y, int samples_count) const override;
    
  private:

    float _depth_of_field;
    float _depth_of_field_factor;

    float _offset_x = 0;
    float _offset_y = 0;
    float _ratio = 0;
  };
}

#endif

