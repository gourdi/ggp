#ifndef __GGO_RENDERER_ABC__
#define __GGO_RENDERER_ABC__

#include <ggo_raytracer_global.h>
#include <ggo_scene.h>
#include <memory>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class render_task_abc
  {
  public:

    virtual ggo::color_32f render_pixel(int x, int y,
                                        const ggo::scene & scene,
                                        const ggo::raytrace_params & raytrace_params) const = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class renderer_abc
  {
  public:

            void  render(uint8_t * buffer, int width, int height,
                         const ggo::scene_builder & scene_builder,
                         const ggo::raytrace_params & raytrace_params = ggo::raytrace_params());

            void  render(uint8_t * buffer, int width, int height,
                         const ggo::scene & scene,
                         const ggo::raytrace_params & raytrace_params = ggo::raytrace_params());

  private:
    
    static  void                                  render_thread_func(const ggo::renderer_abc * renderer,
                                                                     uint8_t * buffer,
                                                                     int width,
                                                                     int height,
                                                                     const ggo::scene * scene,
                                                                     const ggo::raytrace_params * raytrace_params);

    virtual std::shared_ptr<ggo::render_task_abc> create_render_task(const ggo::scene & scene) const = 0;
                                                                     
    virtual void                                  on_start_rendering() = 0;
  };
}

#endif
