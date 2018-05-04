#include "ggo_photon_mapping.h"
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.h>
#include <raytracer/ggo_raytracer.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  photon_mapping::photon_mapping(const std::vector<const ggo::object3d_abc *> & lights,
                                 const std::vector<ggo::pos3f> & target_samples,
                                 const ggo::object3d_abc & object,
                                 const ggo::raycaster_abc & raycaster)
  {
    using color_point = ggo::kdtree<ggo::color_32f, 3>::data_point;

    
    std::vector<color_point> photons;

    for (const auto * light : lights)
    {
      std::vector<color_point> current_light_photons;

      for (int i = 0; i < target_samples.size(); ++i)
      {
        // Build a ray from the light to the object.
        float random_variable1 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].x();
        float random_variable2 = ggo::best_candidate_table[i % GGO_BEST_CANDITATE_TABLE_SIZE].y();

        ggo::pos3f target_sample = target_samples[i];
        ggo::pos3f light_sample = light->sample_point(target_sample, random_variable1, random_variable2);

        ggo::ray3d_float ray(light_sample, target_sample - light_sample);

        // Check if the built ray hits the object.
        auto intersection = object.intersect_ray(ray);
        if (intersection.has_value() == false)
        {
          continue;
        }

        // Check visibility between the light and the object.
        if (raycaster.check_visibility(ray.pos(), ray.pos() + intersection->_dist * ray.dir(), light, &object) == true)
        {
          continue;
        }

        // Transmit the ray through the object.
        int depth = 3;
        auto transmission = object.compute_transmission(ray, intersection->_world_normal, depth);
        if (transmission._type != transmission_type::partial_transmission)
        {
          continue;
        }

        // OK we have the ray that passed through the transparent object. We now cast it to the scene.
        auto hit = raycaster.hit_test(transmission._ray, &object);
        if (hit.has_value() == true)
        {
          const float intensity = ggo::dot(hit->_intersection._world_normal.dir(), transmission._ray.dir());
          GGO_ASSERT_LE(intensity, 0.001f);
          ggo::color_32f photon_color = -intensity * (light->get_emissive_color() * hit->_object->get_color(hit->_intersection._world_normal.pos()));
          ggo::pos3f photon_pos = transmission._ray.pos() + hit->_intersection._dist * transmission._ray.dir();
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

    _tree.reset(new ggo::kdtree<ggo::color_32f, 3>(photons));
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f photon_mapping::process(const ggo::ray3d_float & ray,
                                         const ggo::ray3d_float & world_normal,
                                         const ggo::object3d_abc & hit_object,
                                         const ggo::color_32f & hit_color,
                                         float random_variable1,
                                         float random_variable2) const
  {
    const float radius = 0.1f;

    ggo::color_32f output_color(ggo::black<ggo::color_32f>());

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
