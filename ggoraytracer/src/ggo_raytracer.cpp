#include "ggo_raytracer.h"
#include <ggo_shape_sampling.h>
#include <ggo_object3d.h>
#include <ggo_fog_abc.h>
#include <ggo_material_abc.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_background3d_abc.h>
#include <ggo_indirect_lighting_abc.h>

namespace
{
  //////////////////////////////////////////////////////////////
  struct filtered_light
  {
    filtered_light(const ggo::ray3d_float &ray_to_light, const ggo::color_32f & color) : _ray_to_light(ray_to_light), _color(color) {}

    ggo::ray3d_float  _ray_to_light;
    ggo::color_32f    _color;
  };

  //////////////////////////////////////////////////////////////
  template <typename SampleLightFunc>
  std::vector<filtered_light> filter_lights(const ggo::scene & scene,
                                            const ggo::raycaster_abc & raycaster,
                                            const ggo::ray3d_float & world_normal,
                                            const ggo::object3d * hit_object, 
                                            const SampleLightFunc & sample_light_func)
  {
    std::vector<filtered_light> result;

    // Process each light.
    for (const auto & light : scene.lights())
    {
      if (light.get() == hit_object)
      {
        continue;
      }

      // Check light position.
      ggo::pos3f light_pos = sample_light_func(*light, world_normal.pos());
      ggo::vec3f dir_to_light(light_pos - world_normal.pos());
      if (ggo::dot(world_normal.dir(), dir_to_light) < 0)
      {
        continue;
      }

      // Get light ray, starting from the hit point and pointing to the light.
      ggo::ray3d_float ray_to_light = ggo::ray3d_float(world_normal.pos(), dir_to_light);

      // Handle self-intersection properly.
      const ggo::object3d * exclude_object = hit_object->handle_self_intersection(ray_to_light, false);

      // Check if there is another object between the light and the hit object.
      float dist_to_light = ggo::distance(world_normal.pos(), light_pos);
      if (raycaster.check_visibility(ray_to_light, dist_to_light, exclude_object, light.get()) == true)
      {
        continue;
      }

      // Get light color.
      ggo::color_32f light_color = light->get_emissive_color();
      if (scene.fog() != nullptr)
      {
        light_color = scene.fog()->process_segment(light_pos, world_normal.pos(), light_color);
      }

      result.emplace_back(ray_to_light, light_color);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  template <typename SampleLightFunc, typename SampleReflectedRayFunc, typename IndirectLightingFunc>
  ggo::color_32f process_aux(const ggo::ray3d_float & ray,
                             const ggo::scene & scene,
                             const ggo::raycaster_abc & raycaster,
                             const ggo::indirect_lighting_abc * indirect_lighting,
                             int depth,
                             const ggo::object3d * previous_hit_object,
                             const SampleLightFunc & sample_light_func,
                             const SampleReflectedRayFunc & sample_reflected_ray_func,
                             const IndirectLightingFunc & indirect_lighting_func)
  {
    if (depth < 0)
    {
      return ggo::black<ggo::color_32f>();
    }

    // Handle self-intersection.
    ggo::ray3d_float safe_ray(ray);
    const ggo::object3d * exclude_object = nullptr;
    if (previous_hit_object != nullptr)
    {
      exclude_object = previous_hit_object->handle_self_intersection(safe_ray, false);
    }

    // Does the current ray hit an object?
    ggo::ray3d_float local_normal, world_normal;
    float dist = 0;
    const ggo::object3d * hit_object = raycaster.hit_test(safe_ray, dist, local_normal, world_normal, exclude_object);
    if (hit_object == nullptr)
    {
      ggo::color_32f color = scene.background().get_color(safe_ray);

      if (scene.fog())
      {
        color = scene.fog()->process_background_ray(safe_ray, color);
      }

      return color;
    }
    GGO_ASSERT_FLOAT_EQ(local_normal.dir().get_length(), 1.f);
    GGO_ASSERT_FLOAT_EQ(world_normal.dir().get_length(), 1.f);
    
    ggo::color_32f hit_color(hit_object->get_color(local_normal.pos()));

    // Handle the simple color shading.
    if (hit_object->get_shading() == ggo::shading::simple_color)
    {
      return scene.fog() ? scene.fog()->process_segment(safe_ray.pos(), world_normal.pos(), hit_color) : hit_color;
    }

    // Let's go : do the recursion.
    float reflection_factor = hit_object->get_reflection_factor();
    ggo::color_32f output_color(ggo::black<ggo::color_32f>());

    auto filtered_lights = filter_lights(scene, raycaster, world_normal, hit_object, sample_light_func);

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
        output_color = process_aux(transmitted_ray, scene, raycaster, indirect_lighting, depth - 1, hit_object, sample_light_func, sample_reflected_ray_func, indirect_lighting_func);

        reflection_factor = ggo::raytracer::compute_reflection_factor(safe_ray, world_normal, 1.f, hit_object->get_density());
      }
    }
    // If the object is not transparent, process diffuse shading.
    else
    {
      for (const auto & filtered_light : filtered_lights)
      {
        output_color += ggo::raytracer::diffuse_shading(hit_color, filtered_light._color, world_normal, filtered_light._ray_to_light);
      }
    }

    // Process reflection.
    if (depth > 0 && reflection_factor > 0)
    {
      ggo::ray3d_float reflection_ray = sample_reflected_ray_func(*hit_object, safe_ray, world_normal);

      ggo::color_32f reflection_color = process_aux(reflection_ray, scene, raycaster, indirect_lighting, depth - 1, hit_object, sample_light_func, sample_reflected_ray_func, indirect_lighting_func);

      output_color = (1 - reflection_factor) * output_color + reflection_factor * reflection_color;
    }

    // Specular shading.
    for (const auto & filtered_light : filtered_lights)
    {
      float phong_factor = hit_object->get_phong_factor();
      float phong_shininess = hit_object->get_phong_shininess();
      output_color += ggo::raytracer::specular_shading(phong_factor, phong_shininess, filtered_light._color, safe_ray, world_normal, filtered_light._ray_to_light);
    }

    // Indirect lighting.
    if (indirect_lighting != nullptr)
    {
      output_color += indirect_lighting_func(*indirect_lighting, *hit_object, hit_color, safe_ray, world_normal);
    }

    // Emissive and ambiant color.
    output_color += hit_object->get_emissive_color() + scene.ambient_color();

    // Fog.
    if (scene.fog() != nullptr)
    {
      // Handle attenuation.
      output_color = scene.fog()->process_segment(safe_ray.pos(), world_normal.pos(), output_color);
    }

    // Extra-processing.
    //output_color += extra_processing_func(scene, safe_ray, *hit_object, world_normal, raycaster);

    return output_color;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::process(const ggo::ray3d_float & ray,
                                    const ggo::scene & scene,
                                    const ggo::raytrace_params & raytrace_params)
  {
    ggo::brute_force_raycaster brute_force_racaster(scene.objects());

    const ggo::raycaster_abc * raycaster = raytrace_params._raycaster ? raytrace_params._raycaster : &brute_force_racaster;

    auto sample_light_func = [](const ggo::object3d & light,
                                const ggo::pos3f & target_pos)
    {
      return light.basis().pos();
    };

    auto sample_reflected_ray_func = [](const ggo::object3d & hit_object,
                                        const ggo::ray3d_float & ray,
                                        const ggo::ray3d_float & world_normal)
    {
      return hit_object.get_reflected_ray(ray, world_normal);
    };

    auto indirect_lighting_func = [](const ggo::indirect_lighting_abc & indirect_lighting,
                                     const ggo::object3d & hit_object,
                                     const ggo::color_32f & hit_color,
                                     const ggo::ray3d_float & ray,
                                     const ggo::ray3d_float & world_normal)
    {
      return ggo::black<ggo::color_32f>();
    };

    return process_aux(ray, scene, *raycaster, raytrace_params._indirect_lighting, raytrace_params._depth, nullptr, sample_light_func, sample_reflected_ray_func, indirect_lighting_func);
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::process(const ggo::ray3d_float & ray,
                                    const ggo::scene & scene,
                                    const ggo::raytrace_params & raytrace_params,
                                    float random_variable1,
                                    float random_variable2)
  {
    ggo::brute_force_raycaster brute_force_racaster(scene.objects());

    const ggo::raycaster_abc * raycaster = raytrace_params._raycaster ? raytrace_params._raycaster : &brute_force_racaster;

    auto sample_light_func = [&](const ggo::object3d & light,
                                 const ggo::pos3f & target_pos)
    {
      return light.sample_point(target_pos, random_variable1, random_variable2);
    };

    auto sample_reflected_ray_func = [&](const ggo::object3d & hit_object,
                                         const ggo::ray3d_float & ray,
                                         const ggo::ray3d_float & world_normal)
    {
      return hit_object.sample_reflection_ray(ray, world_normal, random_variable1, random_variable2);
    };

    auto indirect_lighting_func = [&](const ggo::indirect_lighting_abc & indirect_lighting,
                                      const ggo::object3d & hit_object,
                                      const ggo::color_32f & hit_color,
                                      const ggo::ray3d_float & ray,
                                      const ggo::ray3d_float & world_normal)
    {
      return indirect_lighting.process(ray, world_normal, hit_object, hit_color, random_variable1, random_variable2);
    };

    // We can see the fog as a global light, so to get fog ambient color, we create ray form the current hit
    // point and let the fog indicate how much radiance it provides.
    auto extra_processing_func = [&](const ggo::scene & scene, 
                                     const ggo::ray3d_float & ray,
                                     const ggo::ray3d_float & world_normal,
                                     const ggo::object3d & hit_object,
                                     const ggo::raycaster_abc & raycaster)
    {
      ggo::vec3f ambient_dir = ggo::hemisphere_sampling(world_normal.dir(), random_variable1, random_variable2);
      ggo::ray3d_float ambient_ray(world_normal.pos(), ambient_dir, false);

      // If the new ray hit point is very close to current hit point, the fog does not add light,
      // this is why the fog is asked to process black color.
      auto * exclude_object = hit_object.handle_self_intersection(ambient_ray, false);

      ggo::ray3d_float ambient_local_normal, ambient_world_normal;
      float dist = 0;
      if (raycaster.hit_test(ambient_ray, dist, ambient_local_normal, ambient_world_normal, exclude_object) != nullptr)
      {
        return scene.fog()->process_segment(ambient_ray.pos(), ambient_world_normal.pos(), ggo::black<ggo::color_32f>());
      }
      else
      {
        return scene.fog()->process_background_ray(ambient_ray, ggo::black<ggo::color_32f>());
      }
    };

    return process_aux(ray, scene, *raycaster, raytrace_params._indirect_lighting, raytrace_params._depth, nullptr, sample_light_func, sample_reflected_ray_func, indirect_lighting_func);
  }

  //////////////////////////////////////////////////////////////
  bool raytracer::transmit_ray(ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float current_density, float next_density)
  {
    float cos_input = ggo::dot(ray.dir(), world_normal.dir());
    GGO_ASSERT(cos_input <= 0.001f); // Expected negative dot product.

    // Snell-Descartes's law.
    float sin_input = std::sqrt(1 - cos_input * cos_input);
    float sin_output = sin_input * current_density / next_density;

    // The input ray is below the incidence angle => full reflection.
    if (sin_output >= 1.f || sin_output <= -1.f)
    {
      return false;
    }

    // Transmission: the ray passes thgough.
    ggo::vec3f parallel_dir = ray.dir() - cos_input * world_normal.dir();
    parallel_dir.set_length(sin_output);

    float cos_output = std::sqrt(1 - ggo::square(sin_output));
    ggo::vec3f orthogonal_dir = -world_normal.dir();
    orthogonal_dir.set_length(cos_output);

    ray.set_dir(parallel_dir + orthogonal_dir);
    ray.pos() = world_normal.pos() + 0.001f * ray.dir();  // Avoid self-intersection.

    GGO_ASSERT(ray.is_normalized());
    GGO_ASSERT_LE(ggo::dot(world_normal.dir(), ray.dir()), 0.001f);

    return true;
  }

  //////////////////////////////////////////////////////////////
  // Compute reflection factor with Slick's approximation.
  float raytracer::compute_reflection_factor(const ggo::ray3d_float& ray, const ggo::ray3d_float& world_normal, float current_density, float next_density)
  {
    float cos_input = ggo::dot(ray.dir(), world_normal.dir());
    float num = current_density - next_density;
    float den = current_density + next_density;
    float parallel_reflection_factor = ggo::square(num / den);
    float reflection_factor = parallel_reflection_factor + (1 - parallel_reflection_factor) * std::pow(1.f + cos_input, 5.f);
    GGO_ASSERT_BTW(reflection_factor, -0.001f, 1.001f);

    return reflection_factor;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::diffuse_shading(const ggo::color_32f & object_color,
                                            const ggo::color_32f & light_color,
                                            const ggo::ray3d_float & world_normal,
                                            const ggo::ray3d_float & ray_to_light)
  {
    GGO_ASSERT_BTW(world_normal.pos().get<0>() - ray_to_light.pos().get<0>(), -0.01, 0.01);
    GGO_ASSERT_BTW(world_normal.pos().get<1>() - ray_to_light.pos().get<1>(), -0.01, 0.01);
    GGO_ASSERT_BTW(world_normal.pos().get<2>() - ray_to_light.pos().get<2>(), -0.01, 0.01);
    GGO_ASSERT_GE(ggo::dot(world_normal.dir(), ray_to_light.dir()), -0.01);

    return ggo::dot(world_normal.dir(), ray_to_light.dir()) * object_color * light_color;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::specular_shading(float phong_factor,
                                             float phong_shininess,
                                             const ggo::color_32f & light_color,
                                             const ggo::ray3d_float & ray,
                                             const ggo::ray3d_float & world_normal,
                                             const ggo::ray3d_float & ray_to_light)
  {
    if (phong_factor <= 0)
    {
      return ggo::black<ggo::color_32f>();
    }

    ggo::vec3f reflection_dir(2 * ggo::dot(world_normal.dir(), ray_to_light.dir()) * world_normal.dir() - ray_to_light.dir());
    GGO_ASSERT(reflection_dir.is_normalized(0.0001f));

    ggo::vec3f viewer_dir(ray.pos() - world_normal.pos());
    viewer_dir.normalize();

    float phong = ggo::dot(reflection_dir, viewer_dir);
    if (phong <= 0)
    {
      return ggo::black<ggo::color_32f>();
    }

    float specular = phong_factor * std::pow(phong, phong_shininess);
    return light_color * specular;
  }
}

