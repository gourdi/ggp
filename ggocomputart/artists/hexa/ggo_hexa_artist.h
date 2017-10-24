#ifndef __GGO_HEXA_ARTIST__
#define __GGO_HEXA_ARTIST__

#include <ggo_scalar_field_2d_abc.h>
#include <ggo_vec.h>
#include <ggo_color.h>
#include <ggo_pixel_buffer.h>
#include <memory>
#include <vector>

namespace ggo
{
  class object3d;
  class renderer_abc;

  class hexa_artist
  {
  public:

    struct hexa_info
    {
      float _x;
      float _y;
      float _height;
    };

    static  std::pair<ggo::color_32f, ggo::color_32f> generate_colors();
    static  std::vector<ggo::pos3f>                   generate_light_positions();
    static  std::vector<hexa_info>                    generate_hexa_infos();

    static  void                                      render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                                                             const std::vector<hexa_info> & hexa_infos,
                                                             const ggo::color_32f & color_top, const ggo::color_32f & color_side,
                                                             const std::vector<ggo::pos3f> & lights_pos,
                                                             const ggo::color_32f & fog_color,
                                                             ggo::renderer_abc & renderer);
  };
}

#endif
