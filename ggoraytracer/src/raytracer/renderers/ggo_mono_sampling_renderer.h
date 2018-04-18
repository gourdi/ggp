#ifndef __GGO_MONO_RENDERER__
#define __GGO_MONO_RENDERER__

#include <raytracer/renderers/ggo_renderer_abc.h>
#include <raytracer/cameras/ggo_camera_abc.h>

// A single ray in casted per pixel. Assumes all lights are point lights.

namespace ggo
{
  class mono_sampling_renderer : public renderer_abc
  {
  public:

                                          mono_sampling_renderer(ggo::mono_sampling_camera_abc & camera);

    std::shared_ptr<ggo::render_task_abc> create_render_task(const ggo::scene & scene) const override;

  private:

    ggo::mono_sampling_camera_abc & _camera;
  };
}

#endif
