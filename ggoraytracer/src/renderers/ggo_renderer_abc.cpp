#include "ggo_renderer_abc.h"
#include "ggo_raytracer.h"
#include "ggo_camera_abc.h"
#include "ggo_object3d.h"
#include "ggo_indirect_lighting_abc.h"
#include <ggo_helpers.h>
#include <ggo_array.h>
#ifndef GGO_CYGWIN
#include <thread>
#include <mutex>
#endif
#include <stdlib.h>

namespace
{
  int			    global_x;
  int			    global_y;
#ifndef GGO_CYGWIN
  std::mutex	mutex;
#endif
  
  //////////////////////////////////////////////////////////////
  void print_line_number(int line)
  {
    std::cout << "\rRendering line " << line;
    std::cout.flush();	
  }
}

namespace ggo
{
#ifndef GGO_CYGWIN
  //////////////////////////////////////////////////////////////
  void renderer_abc::render_thread_func(const ggo::renderer_abc * renderer,
                                        void * buffer,int width, int height, int line_step, ggo::pixel_buffer_format pbf,
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
      
      const ggo::color_32f color = render_task->render_pixel(x, y, *scene, *raytrace_params);

      switch (pbf)
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
#endif

  //////////////////////////////////////////////////////////////
  void renderer_abc::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                            const ggo::scene_builder & scene_builder,
                            const ggo::raytrace_params & raytrace_params)
  {
    auto scene = scene_builder.build_scene();

    render(buffer, width, height, line_step, pbf, scene, raytrace_params);
  }

  //////////////////////////////////////////////////////////////
  void renderer_abc::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                            const ggo::scene & scene,
                            const ggo::raytrace_params & raytrace_params)
  {
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

        for (int x = 0; x < width; ++x)
        {
          if (x == 200 && y == 200)
          {
            int t = 0;
          }

          const ggo::color_32f color = render_task->render_pixel(x, y, scene, raytrace_params);
          
          switch (pbf)
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
#ifdef GGO_CYGWIN
      std::cerr << "Multi-threading not supported on cygwin" << std::endl;
#else
      global_x = 0;
      global_y = 0;
      
      print_line_number(1);
      
      std::vector<std::thread> threads;
      for (int i = 0; i < threads_count; ++i)
      {
        threads.push_back(std::thread(render_thread_func, this, buffer, width, height, line_step, pbf, &scene, &raytrace_params));
      }
      for (auto & thread : threads)
      {
        thread.join();
      }
#endif
    }

    std::cout << std::endl;
  }
}
