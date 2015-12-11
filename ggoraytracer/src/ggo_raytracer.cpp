#include "ggo_raytracer.h"
#include <ggo_object3d.h>
#include <ggo_fog_abc.h>
#include <ggo_material_abc.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_background3d_abc.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::color raytracer::mono_sampling_raytrace(const ggo::ray3d_float & ray,
                                               const ggo::scene & scene,
                                               const ggo::raytrace_params & raytrace_params)
  {
    std::vector<const ggo::object3d*> inside_stack;

    ggo::brute_force_raycaster brute_force_racaster(scene.objects());

    const ggo::raycaster_abc * raycaster = raytrace_params._raycaster ? raytrace_params._raycaster : &brute_force_racaster;

    return mono_sampling_raytrace_recursive(ray, scene, raycaster, raytrace_params._depth, nullptr, nullptr);
  }

  //////////////////////////////////////////////////////////////
  ggo::color raytracer::mono_sampling_raytrace_recursive(const ggo::ray3d_float & ray,
                                                         const ggo::scene & scene,
                                                         const ggo::raycaster_abc * raycaster,
                                                         int depth, 
                                                         const ggo::object3d * inside_object,
                                                         const ggo::object3d * previous_hit_object)
  {
    // Handle self-intersection.
    ggo::ray3d_float safe_ray(ray);
    const ggo::object3d * exclude_object = nullptr;
    if (previous_hit_object != nullptr)
    {
      exclude_object = previous_hit_object->handle_self_intersection(safe_ray, inside_object != nullptr);
    }

    // Does the current ray hit an object?
    ggo::ray3d_float	local_normal, world_normal;
    float dist = 0;
    const ggo::object3d * hit_object = raycaster->hit_test(ray, dist, local_normal, world_normal, exclude_object);
    if (hit_object == nullptr)
    {
      ggo::color color = scene.background().get_color(ray);
      
      if (scene.fog())
      {
        color = scene.fog()->process_background_ray(ray, color);
      }
      
      return color;
    }
    GGO_ASSERT(local_normal.dir().is_normalized(0.0001));
    GGO_ASSERT(world_normal.dir().is_normalized(0.0001));

    // Hit object's color.
    ggo::color object_color(hit_object->material()->get_color(local_normal.pos()));
    ggo::color output_color(hit_object->get_emissive_color());

    for (const auto & light : scene.lights())
    {
      if (light.get() == hit_object)
      {
        continue;
      }

      output_color += ggo::raytracer::shade(ray, hit_object, object_color, world_normal, light.get(), light->basis().pos(), scene, raycaster);
    }
    
    // Ambient color.
    output_color += scene.ambient_color() * object_color;
    
    // Fog.
    if (scene.fog())
    {
      output_color = scene.fog()->process_segment(ray.pos(), world_normal.pos(), output_color);
    }
    
    // Process reflection (specular surfaces) if depth is not reached.
    if (depth > 0 && hit_object->get_reflection_factor() > 0)
    {
      ggo::vector3d_float reflected_dir(ray.dir() - 2 * ggo::dot(world_normal.dir(), ray.dir()) * world_normal.dir());
      GGO_ASSERT(specular_dir.is_normalized(0.001) == true);
      GGO_ASSERT(ggo::dot(specular_dir, world_normal.dir()) >= -0.001f); // Because of rounding errors, the dot product can be a little bit negative.
      
      ggo::ray3d_float reflected_ray(world_normal.pos(), reflected_dir, false);

      // Recursion.
      ggo::color reflexion_color = mono_sampling_raytrace_recursive(reflected_ray, scene, raycaster, depth - 1, inside_object, hit_object);
      output_color = (1 - hit_object->get_reflection_factor()) * output_color + hit_object->get_reflection_factor() * reflexion_color;
    }

    return output_color;
  }

  //////////////////////////////////////////////////////////////
  ggo::color raytracer::shade(const ggo::ray3d_float & ray, 
                              const ggo::object3d * hit_object,
                              const ggo::color & hit_object_color,
                              const ggo::ray3d_float & world_normal,
                              const ggo::object3d * light,
                              const ggo::point3d_float & light_point,
                              const ggo::scene & scene,
                              const ggo::raycaster_abc * raycaster)
  {
    // Check light position.
    ggo::vector3d_float dir_to_light(light_point - world_normal.pos()); 
    if (ggo::dot(world_normal.dir(), dir_to_light) < 0)
    {
      return ggo::color::BLACK;
    }
    
    // Get light ray, starting from the hit point and pointing to the light.
    ggo::ray3d_float ray_to_light = ggo::ray3d_float(world_normal.pos(), dir_to_light);
    
    // Handle self-intersection properly.
    const ggo::object3d * exclude_object = hit_object->handle_self_intersection(ray_to_light, false);
    
    // Check if there is another object between the light and the hit object.
    float dist_to_light = ggo::distance(world_normal.pos(), light_point);
    if (raycaster->hit_test(ray_to_light, dist_to_light, exclude_object, light) == true)
    {
      return ggo::color::BLACK;
    }
    
    // Get light color.
    ggo::color light_color = light->get_emissive_color();
    if (scene.fog() != nullptr)
    {
      light_color = scene.fog()->process_segment(light_point, world_normal.pos(), light_color);
    }
    
    return hit_object->shade(hit_object_color, light_color, ray, world_normal, ray_to_light);
  }
}

