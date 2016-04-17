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
    using color_point = ggo::kdtree<3, ggo::color>::data_point;

    std::vector<color_point> photons;

    for (const auto light : lights)
    {
      std::vector<color_point> current_light_photons;

      for (int i = 0; i < target_samples.size(); ++i)
      {
        // Build a ray from the light to the object.
        float random_variable1 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].x();
        float random_variable2 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].y();

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
        if (raycaster.check_visibility(ray, dist, light.get(), &object) == true)
        {
          continue;
        }

        // Check if the ray goes into the object or if the is full reflection.
        if (ggo::raytracer::transmit_ray(ray, world_normal, 1.f, object.get_density()) == false)
        {
          // TODO reflected photons should be stored too.
          continue;
        }

        // Let the ray get outside the object.
        bool transmission = false;
        for (int depth = 0; depth < 3; ++depth) // Allow 3 internal reflections.
        {
          // Intersect the ray with the current object.
          if (object.intersect_ray(ray, dist, local_normal, world_normal) == false)
          {
            // Should never happen, but with rounding error...
            transmission = false;
            break;
          }

          // Check if the ray gets out the object.
          if (ggo::raytracer::transmit_ray(ray, world_normal, object.get_density(), 1.0f) == true)
          {
            transmission = true;
            break;
          }
        }
        if (transmission == false)
        {
          continue;
        }

        // OK we have the ray that passed through the transparent object. We now cast it to the scene.
        auto hit_object = raycaster.hit_test(ray, dist, local_normal, world_normal, &object);
        if (hit_object != nullptr)
        {
          ggo::color photon_color = -ggo::dot(world_normal.dir(), ray.dir()) * (light->get_emissive_color() * hit_object->get_color(world_normal.pos()));
          ggo::point3d_float photon_pos = ray.pos() + dist * ray.dir();
          current_light_photons.push_back({ { photon_pos.x(), photon_pos.y(), photon_pos.z() }, photon_color });
        }
      }

      // Normalize photons and store them.
      float normalization = 1.f / current_light_photons.size();
      for (auto & photon : current_light_photons)
      {
        photon._data *= normalization;
        photons.push_back(photon);
      }
    }

    _tree = std::make_unique<ggo::kdtree<3, ggo::color>>(photons);
  }

  //////////////////////////////////////////////////////////////
  ggo::color photon_mapping::process(const ggo::ray3d_float & ray,
                                     const ggo::ray3d_float & world_normal,
                                     const ggo::object3d & hit_object,
                                     const ggo::color & hit_color,
                                     float random_variable1,
                                     float random_variable2) const
  {
    const float radius = 0.1f;

    ggo::color output_color(ggo::color::BLACK);

    auto photons = _tree->find_points({ world_normal.pos().x(), world_normal.pos().y(), world_normal.pos().z() }, radius);
        
    for (const auto & photon : photons)
    {
      output_color += photon._data;
    }

    // Normalization: divide by sphere surface.
    //output_color /= 4 * ggo::PI<float>() * ggo::square(radius);
    output_color /= ggo::PI<float>() * ggo::square(radius);

    return output_color;
  }
}
