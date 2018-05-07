#include "ggo_raytracer.h"
#include <kernel/math/sampling/shape_sampling/ggo_shape_sampling.h>
#include <raytracer/materials/ggo_material_abc.h>
#include <raytracer/raycasters/ggo_raycaster_abc.h>
#include <raytracer/backgrounds/ggo_background3d_abc.h>
#include <raytracer/indirect_lighting/ggo_indirect_lighting_abc.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  raytracer::raytracer(const ggo::scene & scene, const raycaster_abc & raycaster)
  :
  _scene(scene), _raycaster(raycaster)
  {
  }

  //////////////////////////////////////////////////////////////
  std::vector<light_sample> raytracer::sample_lights(const ggo::ray3d_float & world_normal, const ggo::object3d_abc * hit_object, float random_variable1, float random_variable2) const
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
      if (_raycaster.check_visibility(world_normal.pos(), light_pos, exclude_object, light.get()) == true)
      {
        continue;
      }

      // Get light color.
      ggo::color_32f light_color = light->get_emissive_color();
      //for (const auto & volumetric_effect : _scene.get_volumetric_effects())
      //{
      //  light_color = volumetric_effect->process_segment(light_pos, world_normal.pos(), light_color);
      //}

      result.emplace_back(ray_to_light, light_color);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f raytracer::process(const ggo::ray3d_float & ray,
                                    int depth,
                                    const ggo::indirect_lighting_abc * indirect_lighting,
                                    float random_variable1,
                                    float random_variable2,
                                    const ggo::object3d_abc * exclude_object) const
  {
    if (depth < 0)
    {
      return ggo::black<ggo::color_32f>();
    }

    auto hit = _raycaster.hit_test(ray, exclude_object);
    if (hit.has_value() == false)
    {
      ggo::color_32f color = _scene.background().get_color(ray);

      for (const auto & volumetric_effect : _scene.volumetric_objects())
      {
        color = volumetric_effect->process_background_ray(ray, color);
      }

      return color;
    }

    GGO_ASSERT_FLOAT_EQ(hit->_intersection._local_normal.dir().get_length(), 1.f);
    GGO_ASSERT_FLOAT_EQ(hit->_intersection._world_normal.dir().get_length(), 1.f);

    ggo::color_32f output_color = hit->_object->process_ray(ray, hit->_intersection, *this, depth, indirect_lighting, random_variable1, random_variable2);

    for (const auto & volumetric_effect : _scene.volumetric_objects())
    {
      output_color = volumetric_effect->process_segment(ray.pos(), hit->_intersection._world_normal.pos(), output_color);
    }

    if (indirect_lighting != nullptr)
    {
      output_color += indirect_lighting->process(ray, hit->_intersection._world_normal, *hit->_object, output_color, random_variable1, random_variable2);
    }

    return output_color;
  }
}

