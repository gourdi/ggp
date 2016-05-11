#ifndef __GGO_STOA_ARTIST__
#define __GGO_STOA_ARTIST__

#include <memory>
#include <vector>
#include <map>
#include <ggo_tree.h>
#include <ggo_object3d.h>
#include <ggo_renderer_abc.h>
#include <ggo_raytracer_global.h>

// Reference: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html

class ggo_stoa_artist
{
public:

        ggo_stoa_artist(int steps);

  void  render(uint8_t * buffer, int width, int height, float hue,
               const ggo::point3d_float& light_pos1, const ggo::point3d_float& light_pos2, 
               ggo::renderer_abc& renderer) const;

private:

  struct ggo_face_object
  {
    std::shared_ptr<const ggo::object3d>            _object;
    std::shared_ptr<const ggo::face3d<float, true>> _face;
  };

  class ggo_stoa_raycaster : public ggo::raycaster_abc
  {
  public:

    ggo_stoa_raycaster(const std::vector<ggo_stoa_artist::ggo_face_object> & face_objects);

    const ggo::object3d * hit_test(const ggo::ray3d_float & ray,
                                   float & dist,
                                   ggo::ray3d_float & local_normal,
                                   ggo::ray3d_float & world_normal,
                                   const ggo::object3d * exclude_object1 = nullptr,
                                   const ggo::object3d * exclude_object2 = nullptr) const override;

    bool check_visibility(const ggo::ray3d_float & ray,
                          float dist_max,
                          const ggo::object3d * exclude_object1 = nullptr,
                          const ggo::object3d * exclude_object2 = nullptr) const override;

    static  ggo::aabox3d_float  get_bounding_box(const std::vector<ggo_stoa_artist::ggo_face_object> & face_objects);

  private:

    struct ggo_node
    {
      ggo::aabox3d_float            _bounding_box;
      std::vector<ggo_face_object>  _face_objects;
    };

    ggo::tree<ggo_node> _octree;
  };

  std::unique_ptr<ggo_stoa_raycaster> _raycaster;
};

#endif
