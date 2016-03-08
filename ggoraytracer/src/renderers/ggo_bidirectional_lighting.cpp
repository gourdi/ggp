#include "ggo_bidirectional_lighting.h"
#include <ggo_camera_abc.h>
#include <ggo_raycaster_abc.h>
#include <ggo_raytracer.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  bidirectional_lighting::bidirectional_lighting(const ggo::raycaster_abc & raycaster, const ggo::scene & scene)
    :
    _raycaster(raycaster),
    _scene(scene)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color bidirectional_lighting::process(const ggo::ray3d_float & ray,
                                             const ggo::ray3d_float & world_normal,
                                             const ggo::object3d & hit_object,
                                             const ggo::color & hit_color,
                                             float random_variable1,
                                             float random_variable2) const
  {
    ggo::color result(ggo::color::BLACK);

    for (const auto & light : _scene.lights())
    {
      ggo::color current_light_color(ggo::color::BLACK);

      // Cast a ray from light.
      const auto & light_ray = light->sample_ray(random_variable1, random_variable2);
      float light_dist = 0;
      ggo::ray3d_float light_local_normal, light_world_normal;
      const auto * light_hit_object = _raycaster.hit_test(light_ray, light_dist, light_local_normal, light_world_normal);
      if (light_hit_object == nullptr)
      {
        continue;
      }

      // Handle self-intersection
      ggo::ray3d_float camera_world_normal(world_normal);
      auto exclude_light_object = light_hit_object->handle_self_intersection(light_world_normal, false);
      auto exclude_camera_object = hit_object.handle_self_intersection(camera_world_normal, false);

      // Create a ray from camera hit object to light hit object that will close the path.
      ggo::vector3d_float diff(light_world_normal.pos() - camera_world_normal.pos());
      float length = diff.get_length();
      ggo::vector3d_float dir(diff / length);
      ggo::ray3d_float close_ray(camera_world_normal.pos(), dir, false);

      // Check visibility.
      if (_raycaster.check_visibility(close_ray, length, exclude_light_object, exclude_camera_object) == true)
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

      result += path_color;
    }

    return result;
  }
}