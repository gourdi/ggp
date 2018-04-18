#ifndef __GGO_STOA_ARTIST__
#define __GGO_STOA_ARTIST__

#include <memory>
#include <vector>
#include <map>
#include <ggo_tree.h>
#include <ggo_renderer_abc.h>
#include <raytracer/ggo_raytracer_global.h>
#include <ggo_background3d_color.h>

// Reference: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html

namespace ggo
{
  class stoa_artist
  {
  public:

          stoa_artist(int steps);

    void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, float hue,
                 const ggo::pos3f & light_pos1, const ggo::pos3f & light_pos2,
                 ggo::renderer_abc & renderer) const;

  private:

    std::vector<ggo::face3d<float, true>> _faces;
  };
}

#endif
