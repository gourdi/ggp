#ifndef __GGO_STOA_ARTIST__
#define __GGO_STOA_ARTIST__

#include <memory>
#include <vector>
#include <map>
#include <ggo_object3d.h>
#include <ggo_renderer_abc.h>
#include <ggo_raytracer_global.h>

// Reference: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html

class ggo_stoa_artist
{
public:

        ggo_stoa_artist();

  void  render(uint8_t * buffer, int width, int height, float hue,
               const ggo::point3d_float& light_pos1, const ggo::point3d_float& light_pos2, 
               ggo::renderer_abc& renderer) const;

private:

  struct ggo_face_object
  {
    ggo_face_object(const ggo::object3d * object, const ggo::face3d<float, true> * face)
      :
      _object(object), _face(face) {}

    const ggo::object3d *             _object;
    const ggo::face3d<float, true> *  _face;
  };

  struct ggo_raycaster_cell
  {
    ggo::aabox3d_float            _bounding_box;
    std::vector<ggo_face_object>  _face_objects;
    int                           _x;
    int                           _y;
    int                           _z;
  };

  struct ggo_raycaster_cells_aggregate
  {
    ggo::aabox3d_float              _bounding_box;
    std::vector<ggo_raycaster_cell> _cells;
  };

  class ggo_stoa_raycaster : public ggo::raycaster_abc
  {
  public:

    ggo_stoa_raycaster(const std::vector<ggo_raycaster_cell> & cells);

    const ggo::object3d * hit_test(const ggo::ray3d_float & ray,
                                   float & dist,
                                   ggo::ray3d_float & local_normal,
                                   ggo::ray3d_float & world_normal,
                                   const ggo::object3d * exclude_object1 = nullptr,
                                   const ggo::object3d * exclude_object2 = nullptr) const override;

    bool hit_test(const ggo::ray3d_float & ray,
                  float dist_max,
                  const ggo::object3d * exclude_object1 = nullptr,
                  const ggo::object3d * exclude_object2 = nullptr) const override;

  private:

    std::map<std::string, ggo_raycaster_cells_aggregate> _aggregates;
  };

  std::unique_ptr<ggo_stoa_raycaster>         _raycaster;
  std::vector<std::shared_ptr<ggo::object3d>> _objects;
};

#endif
