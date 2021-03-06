#include "ggo_bidirectional_lighting.h"
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_camera_abc.h>
#include <raytracer/raycasters/ggo_raycaster_abc.h>

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
  ggo::rgb_32f bidirectional_lighting::process(const ggo::ray3d_f & ray,
                                               const ggo::ray3d_f & world_normal,
                                               const ggo::object3d_abc & hit_object,
                                               const ggo::rgb_32f & hit_color,
                                               float random_variable1,
                                               float random_variable2) const
  {
    ggo::rgb_32f result = ggo::black_32f();

    for (const auto & light : _scene.lights())
    {
      ggo::rgb_32f current_light_color(ggo::black_32f());

      // Cast a ray from light.
      const auto & light_ray = light->sample_ray(random_variable1, random_variable2);
      auto light_hit_object = _raycaster.hit_test(light_ray);
      if (light_hit_object.has_value() == false)
      {
        continue;
      }

      // Handle self-intersection
      ggo::ray3d_f camera_world_normal(world_normal);
      auto exclude_light_object = light_hit_object->_object->handle_self_intersection(light_hit_object->_intersection._world_normal);
      auto exclude_camera_object = hit_object.handle_self_intersection(camera_world_normal);

      // Create a ray from camera hit object to light hit object that will close the path.
      ggo::vec3_f diff(light_hit_object->_intersection._world_normal.pos() - camera_world_normal.pos());
      ggo::vec3_f dir = normalize(diff);
      ggo::ray3d_f close_ray(camera_world_normal.pos(), dir, false);

      // Check visibility.
      if (_raycaster.check_visibility(close_ray.pos(), close_ray.pos() + diff, exclude_light_object, exclude_camera_object) == true)
      {
        continue;
      }

      // Compute intensity of light.
      float dot_camera_out = ggo::dot(camera_world_normal.dir(), close_ray.dir());
      float dot_light_out = ggo::dot(light_hit_object->_intersection._world_normal.dir(), close_ray.dir());
      if (dot_camera_out < 0 || dot_light_out > 0)
      {
        continue;
      }

      float dot_light_in = ggo::dot(light_ray.dir(), light_hit_object->_intersection._world_normal.dir());
      GGO_ASSERT_BTW(dot_light_in, -1.0001, 0.0001);

      float intensity = dot_light_in * dot_light_out * dot_camera_out;
      GGO_ASSERT_GE(intensity, -0.0001);

      // Compute path color.
      ggo::rgb_32f path_color(light->get_emissive_color());
      path_color *= light_hit_object->_object->get_color(light_hit_object->_intersection._local_normal.pos());
      path_color *= intensity;

      result += path_color;
    }

    return result;
  }
}