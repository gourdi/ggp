#include "ggo_global_sampling_renderer.h"
#include <map>
#include <ggo_halton.h>
#include <ggo_raytracer.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_background3d_abc.h>
#include <ggo_shape_sampling.h>
#include <ggo_fog_abc.h>

//////////////////////////////////////////////////////////////
// ggo_global_sampling_render_task
namespace ggo
{
  class global_sampling_render_task : public render_task_abc
  {
  public:
    
                  global_sampling_render_task(ggo::multi_sampling_camera_abc & camera) : _camera(camera) {}

      ggo::color  render_pixel(int x, int y, const ggo::scene & scene, const ggo::raytrace_params & raytrace_params) const override;
        
      static ggo::color  process_ray(const ggo::ray3d_float & ray,
                                     const ggo::scene & scene,
                                     const ggo::raycaster_abc * raycaster,
                                     int depth, int sample,
                                     const ggo::object3d * previous_hit_object);
                                     
      static ggo::color  get_fog_ambient_color(const ggo::object3d * hit_object,
                                               const ggo::scene & scene,
                                               const ggo::ray3d_float & world_normal,
                                               const ggo::raycaster_abc * raycaster,
                                               int sample);

      int                                     _samples_count;
      const ggo::multi_sampling_camera_abc &  _camera;
  };
  
  //////////////////////////////////////////////////////////////
  ggo::color global_sampling_render_task::render_pixel(int x, int y, const ggo::scene & scene, const ggo::raytrace_params & raytrace_params) const
  {
    // Get camera rays (which are already shuffled).
    auto camera_rays = _camera.get_rays(x, y, _samples_count);
    GGO_ASSERT(camera_rays.size() >= _samples_count);

    // Raycaster.
    ggo::brute_force_raycaster brute_force_racaster(scene.objects());
    const ggo::raycaster_abc * raycaster = raytrace_params._raycaster ? raytrace_params._raycaster : &brute_force_racaster;

    // Proceed.
    ggo::color color(ggo::color::BLACK);
    
    for (int sample = 0; sample < _samples_count; ++sample)
    {
      std::vector<const::ggo::object3d*> inside_stack;
      color += process_ray(camera_rays[sample], scene, raycaster, raytrace_params._depth, sample, nullptr);
    }
    
    return color / static_cast<float>(_samples_count);
  }

  //////////////////////////////////////////////////////////////
  ggo::color global_sampling_render_task::process_ray(const ggo::ray3d_float & ray,
                                                      const ggo::scene & scene,
                                                      const ggo::raycaster_abc * raycaster,
                                                      int depth, int sample,
                                                      const ggo::object3d * previous_hit_object)
  {
    if (depth < 0)
    {
      return ggo::color::BLACK;
    }

    // Handle self-intersection.
    ggo::ray3d_float safe_ray(ray);
    const ggo::object3d * exclude_object = nullptr;
    if (previous_hit_object != nullptr)
    {
      exclude_object = previous_hit_object->handle_self_intersection(safe_ray, false);
    }

    // Does the current ray hit an object? If not, process background.
    ggo::ray3d_float local_normal, world_normal;
    float dist = 0;
    const ggo::object3d * hit_object = raycaster->hit_test(safe_ray, dist, local_normal, world_normal, exclude_object);
    if (hit_object == nullptr)
    {
      ggo::color color = scene.background().get_color(safe_ray);
      
      if (scene.fog() != nullptr)
      {
        color = scene.fog()->process_background_ray(safe_ray, color);
      }
      
      return color;
    }
    GGO_ASSERT(local_normal.dir().is_normalized(0.0001f));
    GGO_ASSERT(world_normal.dir().is_normalized(0.0001f));

    float random_variable1 = ggo::best_candidate_table[sample].x();
    float random_variable2 = ggo::best_candidate_table[sample].y();

    // Compute the color of the hit object.
    float reflection_factor = hit_object->get_reflection_factor();
    ggo::color object_color(ggo::color::BLACK);

    // The hit object is transparent.
    if (hit_object->is_transparent() == true)
    {
      ggo::ray3d_float transmitted_ray(safe_ray);

      // The input ray is below the incidence angle => full reflection.
      if (hit_object->transmit_ray(transmitted_ray, world_normal, depth) == false)
      {
        reflection_factor = 1.f;
      }
      // The ray went through the object: recursion.
      else
      {
        object_color = process_ray(transmitted_ray, scene, raycaster, depth - 1, sample, hit_object);

        reflection_factor = ggo::raytracer::compute_reflexion_factor(safe_ray, world_normal, 1.f, hit_object->get_density());
      }
    }
    // If the object is not transparent, process diffuse shading.
    else
    {
      ggo::color hit_color(hit_object->get_color(local_normal.pos()));

      // Process each light.
      for (const auto & light : scene.lights())
      {
        if (light.get() == hit_object)
        {
          continue;
        }

        // Sample the light from the hit position.
        ggo::point3d_float light_pos = light->sample_point(world_normal.pos(), random_variable1, random_variable2);

        // Shade the hit object, taking into account light visiblity.
        object_color += ggo::raytracer::shade(safe_ray, hit_object, hit_color, world_normal, light.get(), light_pos, scene, raycaster);
      }
    }

    // Process reflection.
    if (depth > 0 && reflection_factor > 0)
    {
      ggo::ray3d_float reflection_ray = hit_object->sample_reflection_ray(safe_ray, world_normal, random_variable1, random_variable2);

      ggo::color reflexion_color = process_ray(reflection_ray, scene, raycaster, depth - 1, sample, hit_object);

      object_color = (1 - reflection_factor) * object_color + reflection_factor * reflexion_color;
    }

    // Output color.
    ggo::color output_color(hit_object->get_emissive_color() + object_color + scene.ambient_color());

    // Fog.
    if (scene.fog() != nullptr)
    {
      // Ambient color.
      output_color += output_color * get_fog_ambient_color(hit_object, scene, world_normal, raycaster, sample);
      
      // Handle attenuation.
      output_color = scene.fog()->process_segment(safe_ray.pos(), world_normal.pos(), output_color);
    }

    return output_color;
  }
  
  //////////////////////////////////////////////////////////////
  // We can see the fog as a global light, so to get fog ambient color, we create ray form the current hit
  // point and let the fog indicate how much radiance it provides.
  ggo::color global_sampling_render_task::get_fog_ambient_color(const ggo::object3d * hit_object,
                                                                const ggo::scene & scene,
                                                                const ggo::ray3d_float & world_normal,
                                                                const ggo::raycaster_abc * raycaster,
                                                                int sample)
  {
    ggo::vector3d_float basis1, basis2;
    world_normal.dir().build_basis(basis1, basis2);

    float random_variable1 = ggo::best_candidate_table[sample].x();
    float random_variable2 = ggo::best_candidate_table[sample].y();

    ggo::vector3d_float ambient_dir = ggo::hemisphere_sampling(world_normal.dir(), random_variable1, random_variable2);
    ggo::ray3d_float ambient_ray(world_normal.pos(), ambient_dir, false);
    
    // If the new ray hit point is very close to current hit point, the fog does not add light,
    // this is why the fog is asked to process black color.
    auto * exclude_object = hit_object->handle_self_intersection(ambient_ray, false);

    ggo::ray3d_float ambient_local_normal, ambient_world_normal;
    float dist = 0;
    if (raycaster->hit_test(ambient_ray, dist, ambient_local_normal, ambient_world_normal, exclude_object) != nullptr)
    {
      return scene.fog()->process_segment(ambient_ray.pos(), ambient_world_normal.pos(), ggo::color::BLACK);
    }
    else
    {
      return scene.fog()->process_background_ray(ambient_ray, ggo::color::BLACK);
    }
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

  //////////////////////////////////////////////////////////////
  void global_sampling_renderer::on_start_rendering()
  {
    _camera.on_start_rendering_frame();
  }
}
