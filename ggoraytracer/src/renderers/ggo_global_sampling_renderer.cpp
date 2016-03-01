#include "ggo_global_sampling_renderer.h"
#include <ggo_best_candidate_sequence.h>
#include <ggo_raytracer.h>

//////////////////////////////////////////////////////////////
// ggo_global_sampling_render_task
namespace ggo
{
  class global_sampling_render_task : public render_task_abc
  {
  public:
    
                  global_sampling_render_task(ggo::multi_sampling_camera_abc & camera) : _camera(camera) {}

      ggo::color  render_pixel(int x, int y, const ggo::scene & scene, const ggo::raytrace_params & raytrace_params) const override;

      int                                     _samples_count;
      const ggo::multi_sampling_camera_abc &  _camera;
  };
  
  //////////////////////////////////////////////////////////////
  ggo::color global_sampling_render_task::render_pixel(int x, int y, const ggo::scene & scene, const ggo::raytrace_params & raytrace_params) const
  {
    // Get camera rays (which are already shuffled).
    auto camera_rays = _camera.get_rays(x, y, _samples_count);
    GGO_ASSERT(camera_rays.size() >= _samples_count);

    // Proceed.
    ggo::color color(ggo::color::BLACK);
    
    for (int sample = 0; sample < _samples_count; ++sample)
    {
      color += ggo::raytracer::process(camera_rays[sample], scene, raytrace_params, ggo::best_candidate_table[sample].x(), ggo::best_candidate_table[sample].y());
    }
    
    return color / static_cast<float>(_samples_count);
  }
}

//////////////////////////////////////////////////////////////
// ggo_global_sampling_renderer
namespace ggo
{
  global_sampling_renderer::global_sampling_renderer(ggo::multi_sampling_camera_abc & camera, int samples_count)
  :
  _camera(camera),
  _samples_count(samples_count)
  {
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::render_task_abc> global_sampling_renderer::create_render_task(const ggo::scene & scene) const
  {
    auto render_task = std::make_shared<global_sampling_render_task>(_camera);

    render_task->_samples_count = _samples_count;

    return render_task;
  }

  //////////////////////////////////////////////////////////////
  void global_sampling_renderer::on_start_rendering()
  {
    _camera.on_start_rendering_frame();
  }
}
