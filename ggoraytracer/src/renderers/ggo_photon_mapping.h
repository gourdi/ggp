#ifndef __GGO_PHOTON_MAPPING__
#define __GGO_PHOTON_MAPPING__

#include <ggo_indirect_lighting_abc.h>
#include <ggo_kdtree.h>
#include <ggo_raycaster_abc.h>
#include <ggo_camera_abc.h>
#include <memory>

namespace ggo
{
  class photon_mapping : public indirect_lighting_abc
  {
  public:

                // Caustic constructor.
                photon_mapping(const std::vector<std::shared_ptr<const ggo::object3d>> & lights,
                               const std::vector<ggo::pos3f> & target_samples,
                               const ggo::object3d & object,
                               const ggo::raycaster_abc & raycaster);

    ggo::color  process(const ggo::ray3d_float & ray,
                        const ggo::ray3d_float & world_normal,
                        const ggo::object3d & hit_object,
                        const ggo::color & hit_color,
                        float random_variable1,
                        float random_variable2) const override;

  private:

    std::unique_ptr<ggo::kdtree<ggo::color, 3>>  _tree;
  };
}

#endif
