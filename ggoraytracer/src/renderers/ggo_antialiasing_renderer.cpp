#include "ggo_antialiasing_renderer.h"
#include <ggo_raytracer.h>

//////////////////////////////////////////////////////////////
// ggo_mono_sampling_render_task
namespace ggo
{
  class antialiasing_render_task : public render_task_abc
  {
  public:
    
                antialiasing_render_task(ggo::antialiasing_camera_abc & camera) : _camera(camera) {}

    ggo::color_32f  render_pixel(int x, int y,
                                 const ggo::scene & scene,
                                 const ggo::raytrace_params & raytrace_params) const override;
    
    const ggo::antialiasing_camera_abc & _camera;
  };

  //////////////////////////////////////////////////////////////
  ggo::color_32f antialiasing_render_task::render_pixel(int x, int y,
                                                        const ggo::scene & scene,
                                                        const ggo::raytrace_params & raytrace_params) const
  {
    std::array<ggo::color_32f, 4> colors;
    
    auto first_pass_rays = _camera.get_first_pass_rays(x, y);
    
    colors[0] = ggo::raytracer::process(first_pass_rays[0], scene, raytrace_params);
    colors[1] = ggo::raytracer::process(first_pass_rays[1], scene, raytrace_params);
    colors[2] = ggo::raytracer::process(first_pass_rays[2], scene, raytrace_params);
    colors[3] = ggo::raytracer::process(first_pass_rays[3], scene, raytrace_params);
    
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
    result += ggo::raytracer::process(second_pass_rays[0], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[1], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[2], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[3], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[4], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[5], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[6], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[7], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[8], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[9], scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[10],scene, raytrace_params);
    result += ggo::raytracer::process(second_pass_rays[11],scene, raytrace_params);

    return result / 16.f;
  };
}

//////////////////////////////////////////////////////////////
// ggo_antialiasing_renderer
namespace ggo
{
  antialiasing_renderer::antialiasing_renderer(ggo::antialiasing_camera_abc & camera)
  :
  _camera(camera)
  {
  }

  //////////////////////////////////////////////////////////////
  std::shared_ptr<ggo::render_task_abc> antialiasing_renderer::create_render_task(const ggo::scene& scene) const
  {
    return std::make_shared<antialiasing_render_task>(_camera);
  }

  //////////////////////////////////////////////////////////////
  void antialiasing_renderer::on_start_rendering()
  {
    _camera.on_start_rendering_frame();
  }
}


