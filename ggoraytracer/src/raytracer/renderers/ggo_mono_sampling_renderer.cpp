#include "ggo_mono_sampling_renderer.h"
#include <raytracer/ggo_raytracer.h>

namespace
{
  class mono_sampling_render_task : public ggo::render_task_abc
  {
  public:
  
                    mono_sampling_render_task(const ggo::mono_sampling_camera_abc &  camera) : _camera(camera) {}
    
    ggo::rgb_32f  render_pixel(int x, int y,
                               const ggo::scene & scene,
                               int depth,
                               const ggo::raycaster_abc & raycaster,
                               const ggo::indirect_lighting_abc * indirect_lighting) const override
    {
      ggo::raytracer raytracer(scene, raycaster);
      return raytracer.process(_camera.get_ray(x, y), depth, indirect_lighting, 0.f, 0.f);
    }

  public:

    const ggo::mono_sampling_camera_abc &  _camera;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  mono_sampling_renderer::mono_sampling_renderer(ggo::mono_sampling_camera_abc & camera)
  :
  _camera(camera)
  {
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<render_task_abc> mono_sampling_renderer::create_render_task(const ggo::scene & scene) const
  {
    return std::make_shared<mono_sampling_render_task>(_camera);
  }
}
