#include "ggo_global_sampling_renderer.h"
#include <kernel/ggo_kernel.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.h>
#include <raytracer/ggo_raytracer.h>

//////////////////////////////////////////////////////////////
// ggo_global_sampling_render_task
namespace ggo
{
  class global_sampling_render_task : public render_task_abc
  {
  public:
    
                      global_sampling_render_task(ggo::multi_sampling_camera_abc & camera) : _camera(camera) {}

      ggo::color_32f  render_pixel(int x, int y,
                                   const ggo::scene & scene,
                                   const ggo::raycaster_abc & raycaster,
                                   int depth) const override;

      int                                     _samples_count;
      const ggo::multi_sampling_camera_abc &  _camera;
  };
  
  //////////////////////////////////////////////////////////////
  ggo::color_32f global_sampling_render_task::render_pixel(int x, int y,
                                                           const ggo::scene & scene,
                                                           const ggo::raycaster_abc & raycaster,
                                                           int depth) const
  {
    // Get camera rays (which are already shuffled).
    auto camera_rays = _camera.get_rays(x, y, _samples_count);
    GGO_ASSERT(camera_rays.size() >= _samples_count);

    // Proceed.
    ggo::color_32f color(ggo::black<ggo::color_32f>());
    ggo::raytracer raytracer(scene, raycaster);
    
    auto uniform_samples2d = best_candidate_table.data() + ggo::rand(size_t(0), best_candidate_table.size() / 2);

    for (int sample = 0; sample < _samples_count; ++sample)
    {
      color += raytracer.process(camera_rays[sample], depth, uniform_samples2d[sample].get<0>(), uniform_samples2d[sample].get<1>());
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
}
