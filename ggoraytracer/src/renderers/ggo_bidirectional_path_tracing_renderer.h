#ifndef __GGO_BIDIRECTIONAL_PATH_TRACING_RENDERER__
#define __GGO_BIDIRECTIONAL_PATH_TRACING_RENDERER__

#include <ggo_indirect_lighting_renderer_abc.h>

namespace ggo
{
  class multi_sampling_camera_abc;
  class raycaster_abc;
  
  class bidirectional_path_tracing_renderer : public indirect_lighting_renderer_abc
  {
  public:
  
                bidirectional_path_tracing_renderer(const multi_sampling_camera_abc & camera,
                                                    const ggo::raycaster_abc & raycaster,
                                                    int samples_count);
    
    ggo::color  render(int x, int y, const ggo::scene & scene) const override;
    
  private:
  
    const multi_sampling_camera_abc & _camera;
    const ggo::raycaster_abc &        _raycaster;
    int                               _samples_count;
    
  };
}

#endif
