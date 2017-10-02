#include "ggo_raytracer.h"
#include <ggo_shape_sampling.h>
#include <ggo_object3d.h>
#include <ggo_fog_abc.h>
#include <ggo_material_abc.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_background3d_abc.h>
#include <ggo_indirect_lighting_abc.h>
#include <ggo_raycaster_abc.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  raytracer::raytracer(const ggo::scene & scene, const ggo::raycaster_abc & raycaster, const ggo::indirect_lighting_abc * indirect_lighting)
    :
    _scene(scene), _raycaster(raycaster), _indirect_lighting(indirect_lighting)
  {
  }

  //////////////////////////////////////////////////////////////
  std::vector<light_sample> raytracer::filter_lights(const ggo::ray3d_float & world_normal, const ggo::object3d_abc * hit_object, float random_variable1, float random_variable2) const
  {
    std::vector<light_sample> result;

    // Process each light.
    for (const auto & light : _scene.lights())
    {
      if (light.get() == hit_object)
      {
        continue;
      }

      // Check light position.
      ggo::pos3f light_pos = light->sample_point(world_normal.pos(), random_variable1, random_variable2);
      ggo::vec3f dir_to_light(light_pos - world_normal.pos());
      if (ggo::dot(world_normal.dir(), dir_to_light) < 0)
      {
        continue;
      }

      // Get light ray, starting from the hit point and pointing to the light.
      ggo::ray3d_float ray_to_light = ggo::ray3d_float(world_normal.pos(), dir_to_light);

      // Handle self-intersection properly.
      const ggo::object3d_abc * exclude_object = hit_object->handle_self_intersection(ray_to_light);

      // Check if there is another object between the light and the hit object.
      float dist_to_light = ggo::distance(world_normal.pos(), light_pos);
      if (_raycaster.check_visibility(ray_to_light, dist_to_light, exclude_object, light.get()) == true)
      {
        continue;
      }

      // Get light color.
      ggo::color_32f light_color = light->get_emissive_color();
      if (_scene.fog() != nullptr)
      {
        light_color = _scene.fog()->process_segment(light_pos, world_normal.pos(), light_color);
      }

      result.emplace_back(ray_to_light, light_color);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::process(const ggo::ray3d_float & ray,
    int depth,
    float random_variable1,
    float random_variable2,
    const ggo::object3d_abc * exclude_object) const
  {
    if (depth < 0)
    {
      return ggo::black<ggo::color_32f>();
    }

    ggo::intersection_data intersection;
    const ggo::object3d_abc * hit_object = _raycaster.hit_test(ray, intersection._dist, intersection._local_normal, intersection._world_normal, exclude_object);
    if (hit_object == nullptr)
    {
      ggo::color_32f color = _scene.background().get_color(ray);

      if (_scene.fog())
      {
        color = _scene.fog()->process_background_ray(ray, color);
      }

      return color;
    }

    GGO_ASSERT_FLOAT_EQ(intersection._local_normal.dir().get_length(), 1.f);
    GGO_ASSERT_FLOAT_EQ(intersection._world_normal.dir().get_length(), 1.f);

    ggo::color_32f output_color = hit_object->process_ray(ray, intersection, *this, depth, random_variable1, random_variable2);

    if (_scene.fog())
    {
      output_color = _scene.fog()->process_segment(ray.pos(), intersection._world_normal.pos(), output_color);
    }

    if (_indirect_lighting != nullptr)
    {
      output_color += _indirect_lighting->process(ray, intersection._world_normal, *hit_object, output_color, random_variable1, random_variable2);
    }

    return output_color;
  }
}

