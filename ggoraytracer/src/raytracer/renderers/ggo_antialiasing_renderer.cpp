#include "ggo_antialiasing_renderer.h"
#include <raytracer/ggo_raytracer.h>

//////////////////////////////////////////////////////////////
// ggo_mono_sampling_render_task
namespace ggo
{
  class antialiasing_render_task : public render_task_abc
  {
  public:
    
                antialiasing_render_task(const ggo::antialiasing_camera_abc & camera) : _camera(camera) {}

    ggo::rgb_32f  render_pixel(int x, int y,
                               const ggo::scene & scene,
                               int depth,
                               const ggo::raycaster_abc & raycaster,
                               const ggo::indirect_lighting_abc * indirect_lighting) const override;
    
    const ggo::antialiasing_camera_abc & _camera;
  };

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f antialiasing_render_task::render_pixel(int x, int y,
                                                      const ggo::scene & scene,
                                                      int depth,
                                                      const ggo::raycaster_abc & raycaster,
                                                      const ggo::indirect_lighting_abc * indirect_lighting) const
  {
    std::array<ggo::rgb_32f, 4> colors;
    
    auto first_pass_rays = _camera.get_first_pass_rays(x, y);

    ggo::raytracer raytracer(scene, raycaster);
    
    colors[0] = raytracer.process(first_pass_rays[0], depth, indirect_lighting, 0.f, 0.f);
    colors[1] = raytracer.process(first_pass_rays[1], depth, indirect_lighting, 0.f, 0.f);
    colors[2] = raytracer.process(first_pass_rays[2], depth, indirect_lighting, 0.f, 0.f);
    colors[3] = raytracer.process(first_pass_rays[3], depth, indirect_lighting, 0.f, 0.f);
    
    float sum_r = colors[0]._r + colors[1]._r + colors[2]._r + colors[3]._r;
    float sum_g = colors[0]._g + colors[1]._g + colors[2]._g + colors[3]._g;
    float sum_b = colors[0]._b + colors[1]._b + colors[2]._b + colors[3]._b;
    
    float mean_r = 0.25f * sum_r;
    float mean_g = 0.25f * sum_g;
    float mean_b = 0.25f * sum_b;
    
    float stddev_r = ggo::square(mean_r - colors[0]._r) + ggo::square(mean_r - colors[1]._r) + ggo::square(mean_r - colors[2]._r) + ggo::square(mean_r - colors[3]._r);
    float stddev_g = ggo::square(mean_g - colors[0]._g) + ggo::square(mean_g - colors[1]._g) + ggo::square(mean_g - colors[2]._g) + ggo::square(mean_g - colors[3]._g);
    float stddev_b = ggo::square(mean_b - colors[0]._b) + ggo::square(mean_b - colors[1]._b) + ggo::square(mean_b - colors[2]._b) + ggo::square(mean_b - colors[3]._b);
    
    if (stddev_r + stddev_g + stddev_b < 0.1)
    {
      return ggo::rgb_32f(mean_r, mean_g, mean_b);
    }
    
    auto second_pass_rays = _camera.get_second_pass_rays(x, y);
    
    ggo::rgb_32f result(sum_r, sum_g, sum_b);
    result += raytracer.process(second_pass_rays[0],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[1],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[2],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[3],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[4],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[5],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[6],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[7],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[8],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[9],  depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[10], depth, indirect_lighting, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[11], depth, indirect_lighting, 0.f, 0.f);

    return result / 16.f;
  };
}

//////////////////////////////////////////////////////////////
// ggo_antialiasing_renderer
namespace ggo
{
  antialiasing_renderer::antialiasing_renderer(const ggo::antialiasing_camera_abc & camera)
  :
  _camera(camera)
  {
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::render_task_abc> antialiasing_renderer::create_render_task(const ggo::scene& scene) const
  {
    return std::make_shared<antialiasing_render_task>(_camera);
  }
}


