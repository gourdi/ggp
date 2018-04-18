#ifndef __GGO_OCTREE_RAYCASTER__
#define __GGO_OCTREE_RAYCASTER__

#include <memory>
#include <kernel/trees/ggo_octree.h>
#include <raytracer/raycasters/ggo_brute_force_raycaster.h>

namespace ggo
{
  class scene;

  class octree_raycaster : public raycaster_abc
  {
  public:

    octree_raycaster(const scene & scene, int depth = 3);

    std::optional<hit_data> hit_test(const ggo::ray3d_float & ray,
                                     const ggo::object3d_abc * exclude_object1,
                                     const ggo::object3d_abc * exclude_object2) const override;

    bool check_visibility(const ggo::ray3d_float & ray,
                          float dist_max,
                          const ggo::object3d_abc * exclude_object1,
                          const ggo::object3d_abc * exclude_object2) const override;

    using octree_t = ggo::octree<const ggo::object3d_abc *, float>;

  private:

    const ggo::object3d_abc * hit_test(const ggo::object3d_abc * hit_object,
                                       const ggo::octree<const ggo::object3d_abc *, float> & node,
                                       const ggo::ray3d_float & ray,
                                       float & dist,
                                       ggo::ray3d_float & local_normal,
                                       ggo::ray3d_float & world_normal,
                                       const ggo::object3d_abc * exclude_object1,
                                       const ggo::object3d_abc * exclude_object2) const;

    bool check_visibility(const ggo::octree<const ggo::object3d_abc *, float> & node, 
                          const ggo::ray3d_float & ray,
                          float dist_max,
                          const ggo::object3d_abc * exclude_object1,
                          const ggo::object3d_abc * exclude_object2) const;

  private:

    std::unique_ptr<const octree_t> _visible_objects_tree;
    std::unique_ptr<const octree_t> _casting_shadows_objects_tree;
    std::unique_ptr<brute_force_raycaster> _brute_force_raycaster;
  };
}

#endif
