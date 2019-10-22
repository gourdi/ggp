#ifndef __GGO_STOA_ARTIST__
#define __GGO_STOA_ARTIST__

#include <memory>
#include <vector>
#include <map>
#include <kernel/trees/ggo_tree.h>
#include <raytracer/renderers/ggo_renderer_abc.h>
#include <raytracer/ggo_raytracer_global.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>

// Reference: http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html

namespace ggo
{
  class stoa_artist
  {
  public:

          stoa_artist(int steps);

    void  render(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
                 float hue, const ggo::pos3_f & light_pos1, const ggo::pos3_f & light_pos2,
                 ggo::renderer_abc & renderer) const;

  private:

    std::vector<ggo::face3d<float, true>> _faces;
  };
}

#endif
