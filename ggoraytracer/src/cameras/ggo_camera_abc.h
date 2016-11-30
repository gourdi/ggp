#ifndef __GGO_CAMERA_ABC__
#define __GGO_CAMERA_ABC__

#include <array>
#include <ggo_shapes3d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  class camera_abc
  {
  public:

                                camera_abc(int width, int height);

    ggo::basis3d_float &        basis() { return _basis; }
    const ggo::basis3d_float &  basis() const { return _basis; }

    int                         get_width() const { return _width; }
    int                         get_height() const { return _height; }

  protected:

    ggo::basis3d_float  _basis;
    int                 _width;
    int                 _height;
  };

  //////////////////////////////////////////////////////////////
  class mono_sampling_camera_abc
  {
  public:

    virtual ggo::ray3d_float  get_ray(int x, int y) const = 0;
  };

  //////////////////////////////////////////////////////////////
  class antialiasing_camera_abc
  {
  public:

    virtual std::array<ggo::ray3d_float, 4>   get_first_pass_rays(int x, int y) const = 0;
    virtual std::array<ggo::ray3d_float, 12>  get_second_pass_rays(int x, int y) const = 0;
  };

  //////////////////////////////////////////////////////////////
  class multi_sampling_camera_abc
  {
  public:

    virtual std::vector<ggo::ray3d_float> get_rays(int x, int y, int samples_count) const = 0;
  };
}

#endif

