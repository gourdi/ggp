#include "ggo_photon_mapping.h"
#include <ggo_best_candidate_sequence.h>
#include <ggo_raytracer.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  photon_mapping::photon_mapping(const std::vector<std::shared_ptr<const ggo::object3d>> & lights,
                                 const std::vector<ggo::pos3f> & target_samples,
                                 const ggo::object3d & object,
                                 const ggo::raycaster_abc & raycaster)
  {
    using color_point = ggo::kdtree<ggo::color_32f, 3>::data_point;

    std::vector<color_point> photons;

    for (const auto light : lights)
    {
      std::vector<color_point> current_light_photons;

      for (int i = 0; i < target_samples.size(); ++i)
      {
        // Build a ray from the light to the object.
        float random_variable1 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].get<0>();
        float random_variable2 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].get<1>();

        ggo::pos3f target_sample = target_samples[i];
        ggo::pos3f light_sample = light->sample_point(target_sample, random_variable1, random_variable2);

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
          ggo::color_32f photon_color = -ggo::dot(world_normal.dir(), ray.dir()) * (light->get_emissive_color() * hit_object->get_color(world_normal.pos()));
          ggo::pos3f photon_pos = ray.pos() + dist * ray.dir();
          current_light_photons.push_back({ { photon_pos.get<0>(), photon_pos.get<1>(), photon_pos.get<2>() }, photon_color });
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

    _tree.reset(new ggo::kdtree<ggo::color_32f, 3>(photons));
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f photon_mapping::process(const ggo::ray3d_float & ray,
                                         const ggo::ray3d_float & world_normal,
                                         const ggo::object3d & hit_object,
                                         const ggo::color_32f & hit_color,
                                         float random_variable1,
                                         float random_variable2) const
  {
    const float radius = 0.1f;

    ggo::color_32f output_color(ggo::color_32f::BLACK);

    auto photons = _tree->find_points({ world_normal.pos().get<0>(), world_normal.pos().get<1>(), world_normal.pos().get<2>() }, radius);
        
    for (const auto & photon : photons)
    {
      output_color += photon._data;
    }

    // Normalization: divide by sphere surface.
    //output_color /= 4 * ggo::PI<float>() * ggo::square(radius);
    output_color /= ggo::pi<float>() * ggo::square(radius);

    return output_color;
  }
}
