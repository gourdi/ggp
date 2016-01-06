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

    // Compute the color of the hit object.
    float reflection_factor = hit_object->get_reflection_factor();
    ggo::color object_color(ggo::color::BLACK);

    // The hit object is transparent.
    if (hit_object->is_transparent() == true)
    {
      if (depth > 0)
      {
        float cos_input = ggo::dot(safe_ray.dir(), world_normal.dir());
        GGO_ASSERT(cos_input <= 0.001f); // Expected negative dot product.

                                         // Snell-Descartes's law.
        float sin_input = std::sqrt(1 - cos_input * cos_input);
        float current_density = inside_object ? inside_object->get_density() : 1.f;
        float sin_output = sin_input * current_density / hit_object->get_density();

        // The input ray is below the incidence angle => full reflection.
        if (sin_output >= 1.f || sin_output <= -1.f)
        {
          reflection_factor = 1.f;
        }
        // Transmission: the ray goes inside or leaves the hit object.
        else
        {
          const ggo::object3d * transmission_object = inside_object ? nullptr : hit_object;
          float next_density = transmission_object ? transmission_object->get_density() : 1.f;

          // Recursion.
          ggo::vector3d_float parallel_dir = safe_ray.dir() - cos_input * world_normal.dir();
          parallel_dir.set_length(sin_output);

          float cos_output = std::sqrt(1 - ggo::square(sin_output));
          ggo::vector3d_float orthogonal_dir = -world_normal.dir();
          orthogonal_dir.set_length(cos_output);

          ggo::ray3d_float transmitted_ray(world_normal.pos(), parallel_dir + orthogonal_dir, false);
          GGO_ASSERT(transmitted_ray.is_normalized());
          GGO_ASSERT_LE(ggo::dot(world_normal.dir(), transmitted_ray.dir()), 0.001f);

          object_color = mono_sampling_raytrace_recursive(transmitted_ray, scene, raycaster, depth - 1, transmission_object, hit_object);

          // Compute reflection factor with Slick's approximation.
          float num = current_density - next_density;
          float den = current_density + next_density;
          float parallel_reflection_factor = ggo::square(num / den);
          reflection_factor = parallel_reflection_factor + (1 - parallel_reflection_factor) * std::pow(1.f + cos_input, 5.f);
          GGO_ASSERT_BTW(reflection_factor, -0.001f, 0.001f);
        }
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

        // Shade the hit object, taking into account light visiblity.
        object_color += ggo::raytracer::shade(safe_ray, hit_object, hit_color, world_normal, light.get(), light->basis().pos(), scene, raycaster);
      }
    }
    
    // Process reflection.
    if (depth > 0 && reflection_factor > 0)
    {
      ggo::ray3d_float reflection_ray = hit_object->get_reflected_ray(safe_ray, world_normal);

      ggo::color reflexion_color = mono_sampling_raytrace_recursive(reflection_ray, scene, raycaster, depth - 1, inside_object, hit_object);

      object_color = (1 - reflection_factor) * object_color + reflection_factor * reflexion_color;
    }

    // Output color.
    ggo::color output_color(hit_object->get_emissive_color() + object_color + scene.ambient_color());
    
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

