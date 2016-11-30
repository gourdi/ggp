#ifndef __GGO_ANTIALIASING_RENDERER__
#define __GGO_ANTIALIASING_RENDERER__

#include "ggo_renderer_abc.h"
#include <ggo_object3d.h>
#include <ggo_camera_abc.h>

namespace ggo
{
  class antialiasing_renderer : public renderer_abc
  {
  public:

                                          antialiasing_renderer(const ggo::antialiasing_camera_abc & camera);

    std::shared_ptr<ggo::render_task_abc> create_render_task(const ggo::scene & scene) const override;

  private:

    const ggo::antialiasing_camera_abc & _camera;
  };
}

#endif
