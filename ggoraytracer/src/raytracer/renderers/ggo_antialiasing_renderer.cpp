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

    ggo::color_32f  render_pixel(int x, int y,
                                 const ggo::scene & scene,
                                 const ggo::raycaster_abc & raycaster,
                                 int depth) const override;
    
    const ggo::antialiasing_camera_abc & _camera;
  };

  //////////////////////////////////////////////////////////////
  ggo::color_32f antialiasing_render_task::render_pixel(int x, int y,
                                                        const ggo::scene & scene,
                                                        const ggo::raycaster_abc & raycaster,
                                                        int depth) const
  {
    std::array<ggo::color_32f, 4> colors;
    
    auto first_pass_rays = _camera.get_first_pass_rays(x, y);

    ggo::raytracer raytracer(scene, raycaster);
    
    colors[0] = raytracer.process(first_pass_rays[0], depth, 0.f, 0.f);
    colors[1] = raytracer.process(first_pass_rays[1], depth, 0.f, 0.f);
    colors[2] = raytracer.process(first_pass_rays[2], depth, 0.f, 0.f);
    colors[3] = raytracer.process(first_pass_rays[3], depth, 0.f, 0.f);
    
    float sum_r = colors[0].r() + colors[1].r() + colors[2].r() + colors[3].r();
    float sum_g = colors[0].g() + colors[1].g() + colors[2].g() + colors[3].g();
    float sum_b = colors[0].b() + colors[1].b() + colors[2].b() + colors[3].b();
    
    float mean_r = 0.25f * sum_r;
    float mean_g = 0.25f * sum_g;
    float mean_b = 0.25f * sum_b;
    
    float stddev_r = ggo::square(mean_r - colors[0].r()) + ggo::square(mean_r - colors[1].r()) + ggo::square(mean_r - colors[2].r()) + ggo::square(mean_r - colors[3].r());
    float stddev_g = ggo::square(mean_g - colors[0].g()) + ggo::square(mean_g - colors[1].g()) + ggo::square(mean_g - colors[2].g()) + ggo::square(mean_g - colors[3].g());
    float stddev_b = ggo::square(mean_b - colors[0].b()) + ggo::square(mean_b - colors[1].b()) + ggo::square(mean_b - colors[2].b()) + ggo::square(mean_b - colors[3].b());
    
    if (stddev_r + stddev_g + stddev_b < 0.1)
    {
      return ggo::color_32f(mean_r, mean_g, mean_b);
    }
    
    auto second_pass_rays = _camera.get_second_pass_rays(x, y);
    
    ggo::color_32f result(sum_r, sum_g, sum_b);
    result += raytracer.process(second_pass_rays[0],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[1],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[2],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[3],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[4],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[5],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[6],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[7],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[8],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[9],  depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[10], depth, 0.f, 0.f);
    result += raytracer.process(second_pass_rays[11], depth, 0.f, 0.f);

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


