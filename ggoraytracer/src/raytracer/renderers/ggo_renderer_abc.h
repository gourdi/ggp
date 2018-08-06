#ifndef __GGO_RENDERER_ABC__
#define __GGO_RENDERER_ABC__

#include <2d/ggo_image_format.h>
#include <raytracer/ggo_raytracer_global.h>
#include <raytracer/ggo_scene.h>
#include <memory>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class render_task_abc
  {
  public:

    virtual ggo::rgb_32f render_pixel(int x, int y,
                                      const ggo::scene & scene,
                                      int depth,
                                      const ggo::raycaster_abc & raycaster,
                                      const ggo::indirect_lighting_abc * indirect_lighting) const = 0;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class renderer_abc
  {
  public:

            void  render(void * buffer, int width, int height, int line_step, ggo::image_format format,
                         const ggo::scene & scene,
                         const ggo::raytrace_params & raytrace_params = ggo::raytrace_params());

  private:
    
    static  void                                  render_thread_func(const ggo::renderer_abc * renderer,
                                                                     void * buffer, int width, int height, int line_step, ggo::image_format format,
                                                                     const ggo::scene * scene,
                                                                     int depth,
                                                                     const ggo::raycaster_abc * raycaster,
                                                                     const ggo::indirect_lighting_abc * indirect_lighting);

    virtual std::shared_ptr<ggo::render_task_abc> create_render_task(const ggo::scene & scene) const = 0;
  };
}

#endif
