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

    octree_raycaster(const std::vector<const ggo::object3d_abc *> & objects, int depth = 3);
    octree_raycaster(const std::vector<std::shared_ptr<const ggo::object3d_abc>> & objects, int depth = 3);

    using octree_t = ggo::octree<const ggo::object3d_abc *, float>;

    void  process_ray(const ggo::ray3d_float & ray,
                      std::function<bool(const ggo::object3d_abc *)> func,
                      const ggo::object3d_abc * exclude_object1 = nullptr,
                      const ggo::object3d_abc * exclude_object2 = nullptr) const override;

    void  process_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                          std::function<bool(const ggo::object3d_abc *)> func,
                          const ggo::object3d_abc * exclude_object1 = nullptr,
                          const ggo::object3d_abc * exclude_object2 = nullptr) const override;

  private:

    bool  process_ray(const ggo::octree<const ggo::object3d_abc *, float> & node,
                      const ggo::ray3d_float & ray,
                      const std::function<bool(const ggo::object3d_abc *)> & func,
                      const ggo::object3d_abc * exclude_object1,
                      const ggo::object3d_abc * exclude_object2) const;

    bool  process_segment(const ggo::octree<const ggo::object3d_abc *, float> & node, 
                          const ggo::pos3f & pos, const ggo::vec3f & dir, float length,
                          const std::function<bool(const ggo::object3d_abc *)> & func,
                          const ggo::object3d_abc * exclude_object1,
                          const ggo::object3d_abc * exclude_object2) const;

  //  const ggo::object3d_abc * hit_test(const ggo::object3d_abc * hit_object,
  //                                     const ggo::octree<const ggo::object3d_abc *, float> & node,
  //                                     const ggo::ray3d_float & ray,
  //                                     float & dist,
  //                                     ggo::ray3d_float & local_normal,
  //                                     ggo::ray3d_float & world_normal,
  //                                     const ggo::object3d_abc * exclude_object1,
  //                                     const ggo::object3d_abc * exclude_object2) const;

  //  bool check_visibility(const ggo::octree<const ggo::object3d_abc *, float> & node, 
  //                        const ggo::ray3d_float & ray,
  //                        float dist_max,
  //                        const ggo::object3d_abc * exclude_object1,
  //                        const ggo::object3d_abc * exclude_object2) const;

  private:

    std::unique_ptr<const octree_t> _objects_tree;
    std::unique_ptr<brute_force_raycaster> _brute_force_raycaster;
  };
}

#endif
