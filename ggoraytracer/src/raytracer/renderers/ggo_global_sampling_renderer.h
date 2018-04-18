#ifndef _GGO_GLOBAL_SAMPLING_RENDERER__
#define _GGO_GLOBAL_SAMPLING_RENDERER__

#include <raytracer/renderers/ggo_renderer_abc.h>
#include <raytracer/cameras/ggo_camera_abc.h>

namespace ggo
{
  class global_sampling_renderer : public renderer_abc
  {
  public:

                                          global_sampling_renderer(ggo::multi_sampling_camera_abc & camera, int samples_count);
                                          
    std::shared_ptr<ggo::render_task_abc> create_render_task(const ggo::scene & scene) const override;
    
  private:

    int                               _samples_count;
    ggo::multi_sampling_camera_abc &  _camera;
  };
}

#endif
