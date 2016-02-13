#include "ggo_mono_sampling_renderer.h"
#include <ggo_raytracer.h>

namespace
{
  class mono_sampling_render_task : public ggo::render_task_abc
  {
  public:
  
                mono_sampling_render_task(const ggo::mono_sampling_camera_abc &  camera) : _camera(camera) {}
    
    ggo::color  render_pixel(int x, int y,
                             const ggo::scene & scene,
                             const ggo::raytrace_params & raytrace_params) const override
    {
      return ggo::raytracer::process(_camera.get_ray(x, y), scene, raytrace_params);
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

  //////////////////////////////////////////////////////////////
  void mono_sampling_renderer::on_start_rendering()
  {
    _camera.on_start_rendering_frame();
  }
}
