#ifndef __GGO_CUMBIA_ARTIST__
#define __GGO_CUMBIA_ARTIST__

#include <ggo_artist.h>
#include <ggo_tree.h>
#include <ggo_background3d_color.h>
#include <ggo_object3d.h>
#include <ggo_scene.h>
#include <ggo_point_camera.h>

namespace ggo
{
  class renderer_abc;
  class point_camera;

  class cumbia_artist
  {
  public:

    cumbia_artist();
    ~cumbia_artist();

    void	init(ggo::basis3d_float & camera_basis, float & aperture, int boxes_count);
    void	init(ggo::basis3d_float & camera_basis, float & aperture, float & depth_of_field, float depth_of_field_factor, int boxes_count);

    void	render_bitmap(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, ggo::renderer_abc & renderer) const;

  private:

    std::vector<ggo::tree<ggo::aabox3d_float> *>  init_common(ggo::basis3d_float & camera_basis, float & aperture, int boxes_count);

  private:

    ggo::tree<ggo::aabox3d_float> *	_boxes_tree;
    ggo::scene                      _scene;
    ggo::background3d_color			    _background3d;
  };
}

#endif

