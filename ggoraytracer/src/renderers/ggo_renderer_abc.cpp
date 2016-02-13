#include "ggo_renderer_abc.h"
#include "ggo_raytracer.h"
#include "ggo_camera_abc.h"
#include "ggo_object3d.h"
#include "ggo_indirect_lighting_abc.h"
#include <ggo_helpers.h>
#include <ggo_array.h>
#include <thread>
#include <mutex>
#include <stdlib.h>

namespace
{
  int			    global_x;
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
                                        uint8_t * buffer,
                                        int width,
                                        int height,
                                        const ggo::scene * scene,
                                        const ggo::raytrace_params * raytrace_params)
  {
    auto render_task = renderer->create_render_task(*scene);
      
    while (true)
    {
      mutex.lock();

      int x = global_x;
      int y = global_y;

      ++global_x;
      
      if (global_x >= width)
      { 
        ++global_y;
        global_x = 0;
          
        if (global_y < height)
        {
          print_line_number(global_y + 1);
        }
      }
      
      mutex.unlock();
      
      if (y >= height)
      {
        break;
      }
      
      ggo::color color = render_task->render_pixel(x, y, *scene, *raytrace_params);

      uint8_t * ptr = buffer + 3 * ((height - y - 1) * width + x);
        
      *ptr++ = color.r8();
      *ptr++ = color.g8();
      *ptr++ = color.b8();
    }
  }

  //////////////////////////////////////////////////////////////
  void renderer_abc::render(uint8_t * buffer, int width, int height,
                            const ggo::scene_builder & scene_builder,
                            const ggo::raytrace_params & raytrace_params)
  {
    auto scene = scene_builder.build_scene();

    render(buffer, width, height, scene, raytrace_params);
  }

  //////////////////////////////////////////////////////////////
  void renderer_abc::render(uint8_t * buffer,
                            int width,
                            int height,
                            const ggo::scene & scene,
                            const ggo::raytrace_params & raytrace_params)
  {
    on_start_rendering();

    std::cout << scene.lights().size() << " light(s) in the scene." << std::endl;

    int threads_count = raytrace_params._threads_count;

    char * env_str = std::getenv("GGO_RAYTRACING_THREAD_COUNT");
    if (env_str == nullptr)
    {
      env_str = std::getenv("GGO_RTTC");
    }
    if (env_str != nullptr)
    {
      int value = 0;
      if (ggo::str2val(env_str, value) == true)
      {
        threads_count = value;
      }
    }
    
    std::cout << "Rendering threads count: " << threads_count << std::endl;

    if (threads_count <= 1)
    {
      auto render_task = create_render_task(scene);
      
      for (int y = 0; y < height; ++y)
      {
        print_line_number(y + 1);
        
        uint8_t * line = buffer + 3 * (height - y - 1) * width;

        for (int x = 0; x < width; ++x)
        {
          ggo::color color = render_task->render_pixel(x, y, scene, raytrace_params);

          *line++ = color.r8();
          *line++ = color.g8();
          *line++ = color.b8();
        }
      }
    }
    else
    {
      global_x = 0;
      global_y = 0;
      
      print_line_number(1);
      
      std::vector<std::thread> threads;
      for (int i = 0; i < threads_count; ++i)
      {
        threads.push_back(std::thread(render_thread_func, this, buffer, width, height, &scene, &raytrace_params));
      }
      for (auto & thread : threads)
      {
        thread.join();
      }
    }

    std::cout << std::endl;
  }
}
