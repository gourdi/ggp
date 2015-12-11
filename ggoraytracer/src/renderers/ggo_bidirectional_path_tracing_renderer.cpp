#include "ggo_bidirectional_path_tracing_renderer.h"
#include <ggo_camera_abc.h>
#include <ggo_raycaster_abc.h>
#include <ggo_raytracer.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  bidirectional_path_tracing_renderer::bidirectional_path_tracing_renderer(const multi_sampling_camera_abc & camera,
                                                                           const ggo::raycaster_abc & raycaster,
                                                                           int samples_count)
  :
  _camera(camera),
  _raycaster(raycaster),
  _samples_count(samples_count)
  {
  }
  
  //////////////////////////////////////////////////////////////
  ggo::color bidirectional_path_tracing_renderer::render(int x, int y, const ggo::scene & scene) const
  {
    ggo::color result(ggo::color::BLACK);
    
    auto camera_rays = _camera.get_rays(x, y, _samples_count);
    
    for (const auto & light : scene.lights())
    {
      auto light_rays = light->sample_rays(_samples_count);

      if (light_rays.size() < _samples_count)
      {
        GGO_TRACE("Not enough light samples");
        continue;
      }
      
      ggo::shuffle(light_rays);
      
      ggo::color current_light_color(ggo::color::BLACK);
      int normalization_factor = 0;
      
      for (int i = 0; i < _samples_count; ++i)
      {
        // Cast a ray from light.
        const auto & light_ray = light_rays[i];
        float light_dist = 0;
        ggo::ray3d_float light_local_normal, light_world_normal;
        const auto * light_hit_object = _raycaster.hit_test(light_ray, light_dist, light_local_normal, light_world_normal);
        if (light_hit_object == nullptr)
        {
          continue;
        }
        
        // Cast a ray from camera.
        const auto & camera_ray = camera_rays[i];
        float camera_dist = 0;
        ggo::ray3d_float camera_local_normal, camera_world_normal;
        const auto * camera_hit_object = _raycaster.hit_test(camera_ray, camera_dist, camera_local_normal, camera_world_normal);
        if (camera_hit_object == nullptr)
        {
          continue;
        }

        // Handle self-intersection
        auto exclude_light_object = light_hit_object->handle_self_intersection(light_world_normal, false);
        auto exclude_camera_object = camera_hit_object->handle_self_intersection(camera_world_normal, false);
        
        // Create a ray from camera hit object to light hit object that will close the path.
        ggo::vector3d_float diff(light_world_normal.pos() - camera_world_normal.pos());
        float length = diff.get_length();
        ggo::vector3d_float dir(diff / length);
        ggo::ray3d_float close_ray(camera_world_normal.pos(), dir, false);        
        
        // Increase normalization factor before checking visibility.
        normalization_factor += 1;

        if (_raycaster.hit_test(close_ray, length, exclude_light_object, exclude_camera_object) == true)
        {
          continue;
        }
        
        // Compute intensity of light.
        float dot_camera_out = ggo::dot(camera_world_normal.dir(), close_ray.dir());
        float dot_light_out = ggo::dot(light_world_normal.dir(), close_ray.dir());
        if (dot_camera_out < 0 || dot_light_out > 0)
        {
          continue;
        }
        
        float dot_light_in = ggo::dot(light_ray.dir(), light_world_normal.dir());
        GGO_ASSERT_BTW(dot_light_in, -1.0001, 0.0001);
        
        float intensity = dot_light_in * dot_light_out * dot_camera_out;
        GGO_ASSERT_GE(intensity, -0.0001);
        
        // Compute path color.
        ggo::color path_color(light->get_emissive_color());
        path_color *= light_hit_object->get_color(light_local_normal.pos());
        path_color *= intensity;

        current_light_color += path_color;
      }
      
      if (normalization_factor > 0)
      {
        result += current_light_color / static_cast<float>(normalization_factor);
      }
    }
    
    return result;
  }
}