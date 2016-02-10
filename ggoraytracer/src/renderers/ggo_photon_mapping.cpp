#include "ggo_photon_mapping.h"
#include <ggo_best_candidate_sequence.h>
#include <ggo_raytracer.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  photon_mapping::photon_mapping(const std::vector<std::shared_ptr<const ggo::object3d>> & lights,
                                 const std::vector<ggo::point3d_float> & target_samples,
                                 const ggo::object3d & object,
                                 const ggo::raycaster_abc & raycaster)
  {
    std::vector<std::pair<ggo::point3d_float, ggo::color>> photons;

    for (const auto light : lights)
    {
      for (int i = 0; i < target_samples.size(); ++i)
      {
        // Build a ray from the light to the object.
        float random_variable1 = ggo::best_candidate_table[i].x();
        float random_variable2 = ggo::best_candidate_table[i].y();

        ggo::point3d_float target_sample = target_samples[i];
        ggo::point3d_float light_sample = light->sample_point(target_sample, random_variable1, random_variable2);

        ggo::ray3d_float ray(light_sample, target_sample - light_sample);

        // Check if the built ray hits the object.
        ggo::ray3d_float local_normal, world_normal;
        float dist = -1.f;
        if (object.intersect_ray(ray, dist, local_normal, world_normal) == false)
        {
          continue;
        }

        // Check visibility between the light and the object.
        if (raycaster.check_visibility(ray, dist, light.get(), &object) == false)
        {
          continue;
        }

        // Check if the ray goes into the object or if the is full reflection.
        if (ggo::raytracer::transmit_ray(ray, world_normal, 1.f, object.get_density()) == false)
        {
          continue;
        }

        // Let the ray get outside the object.
        bool ray_out = false;
        for (int depth = 0; depth < 3; ++depth) // Allow 3 internal reflections.
        {
          // Interect the ray with the current object.
          if (object.intersect_ray(ray, dist, local_normal, world_normal) == false)
          {
            // Should never happen, but with rounding error...
            ray_out = false;
            break;
          }

          // Check if the ray gets out the object.
          if (ggo::raytracer::transmit_ray(ray, world_normal, object.get_density(), 1.0f) == true)
          {
            ray_out = true;
            break;
          }
        }

        // OK we have the ray that passed through the transparent object. We now cast it to the scene.
        if (raycaster.hit_test(ray, dist, local_normal, world_normal, &object) != nullptr)
        {
          photons.push_back(std::make_pair(ray.pos() + dist * ray.dir(), light->get_emissive_color()));
        }
      }
    }

    _tree = std::make_unique<ggo::tree3d<ggo::color>>(photons);
  }

  //////////////////////////////////////////////////////////////
  ggo::color photon_mapping::render(int x, int y, const ggo::scene & scene) const
  {
    


    return ggo::color::BLACK;
  }
}
