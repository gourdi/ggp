#include "ggo_renderer_abc.h"
#include <kernel/memory/ggo_array.h>
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_camera_abc.h>
#include <raytracer/raycasters/ggo_brute_force_raycaster.h>
#include <thread>
#include <mutex>
#include <stdlib.h>

namespace
{
  int			    global_y;
  std::mutex	mutex;
  
  //////////////////////////////////////////////////////////////
  void print_line_number(int line)
  {
    std::cout << "\rRendering line " << line;
    std::cout.flush();	
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void renderer_abc::render_thread_func(const ggo::renderer_abc * renderer,
                                        void * buffer,int width, int height, int line_step, ggo::image_format format,
                                        const ggo::scene * scene,
                                        const ggo::raycaster_abc * raycaster,
                                        int depth)
  {
    auto render_task = renderer->create_render_task(*scene);
      
    while (true)
    {
      mutex.lock();

      int y = global_y;

      if (global_y < height)
      {
        print_line_number(global_y + 1);
      }

      global_y++;
      
      mutex.unlock();
      
      if (y >= height)
      {
        break;
      }
      
      for (int x = 0; x < width; ++x)
      {
        const ggo::color_32f color = render_task->render_pixel(x, y, *scene, *raycaster, depth);

        switch (format)
        {
        case ggo::rgb_8u_yu:
          ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, line_step, ggo::convert_color_to<ggo::color_8u>(color));
          break;
        default:
          GGO_FAIL();
          break;
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void renderer_abc::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
                            const ggo::scene & scene,
                            const ggo::raytrace_params & raytrace_params)
  {
    std::cout << scene.lights().size() << " light(s) in the scene." << std::endl;

    // Create brute force raycaster if none is provided.
    std::unique_ptr<ggo::brute_force_raycaster> brute_force_raycaster;
    const ggo::raycaster_abc * raycaster = raytrace_params._raycaster;
    if (raycaster == nullptr)
    {
      std::vector<const ggo::object3d_abc *> objects;
      for (const auto & object : scene.solid_objects())
      {
        objects.push_back(object.get());
      }

      brute_force_raycaster.reset(new ggo::brute_force_raycaster(std::move(objects)));
      raycaster = brute_force_raycaster.get();
    }

    int threads_count = raytrace_params._threads_count;

    char * env_str = std::getenv("GGO_RAYTRACING_THREAD_COUNT");
    if (env_str == nullptr)
    {
      env_str = std::getenv("GGO_RTTC");
    }
    if (env_str != nullptr)
    {
      auto value = str_to<int>(env_str);
      if (value)
      {
        threads_count = *value;
      }
    }
    
    std::cout << "Rendering threads count: " << threads_count << std::endl;

    if (threads_count <= 1)
    {
      auto render_task = create_render_task(scene);
      
      for (int y = 0; y < height; ++y)
      {
        print_line_number(y + 1);

        for (int x = 0; x < width; ++x)
        {
          const ggo::color_32f color = render_task->render_pixel(x, y, scene, *raycaster, raytrace_params._depth);
          
          switch (format)
          {
          case ggo::rgb_8u_yu:
            ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, height, line_step, ggo::convert_color_to<ggo::color_8u>(color));
            break;
          default:
            GGO_FAIL();
            break;
          }
        }
      }
    }
    else
    {
      global_y = 0;
      
      print_line_number(1);
      
      std::vector<std::thread> threads;
      for (int i = 0; i < threads_count; ++i)
      {
        threads.push_back(std::thread(render_thread_func, this, buffer, width, height, line_step, format, &scene, raycaster, raytrace_params._depth));
      }
      for (auto & thread : threads)
      {
        thread.join();
      }
    }

    std::cout << std::endl;
  }
}
