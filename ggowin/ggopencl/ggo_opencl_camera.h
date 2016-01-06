#include <ggo_point_camera.h>

class ggo_opencl_camera
{
public:

                            ggo_opencl_camera(int width, int height, int samples_count);

  ggo_point_camera &        get_camera();
  const ggo_point_camera &  get_camera() const;

  std::vector<float>        get_params() const;

private:

  ggo_point_camera  _camera; 
};
