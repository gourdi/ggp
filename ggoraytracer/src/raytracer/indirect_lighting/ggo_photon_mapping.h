#ifndef __GGO_PHOTON_MAPPING__
#define __GGO_PHOTON_MAPPING__

#include <kernel/trees/ggo_kdtree.h>
#include <raytracer/indirect_lighting/ggo_indirect_lighting_abc.h>
#include <raytracer/raycasters/ggo_raycaster_abc.h>
#include <raytracer/cameras/ggo_camera_abc.h>
#include <memory>

namespace ggo
{
  class photon_mapping : public indirect_lighting_abc
  {
  public:

                    // Caustic constructor.
                  photon_mapping(const std::vector<const ggo::object3d_abc *> & lights,
                                 const std::vector<ggo::pos3_f> & target_samples,
                                 const ggo::object3d_abc & object,
                                 const ggo::raycaster_abc & raycaster);

    ggo::rgb_32f  process(const ggo::ray3d_f & ray,
                          const ggo::ray3d_f & world_normal,
                          const ggo::object3d_abc & hit_object,
                          const ggo::rgb_32f & hit_color,
                          float random_variable1,
                          float random_variable2) const override;

  private:

    std::unique_ptr<ggo::kdtree<ggo::rgb_32f, ggo::vec3_f>>  _tree;
  };
}

#endif
