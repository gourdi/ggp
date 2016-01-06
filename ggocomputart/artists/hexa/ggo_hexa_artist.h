#ifndef __GGO_HEXA_ARTIST__
#define __GGO_HEXA_ARTIST__

#include <ggo_scalar_field_2d_abc.h>
#include <ggo_set3.h>
#include <memory>
#include <vector>

namespace ggo
{
  class color;
  class object3d;
  class renderer_abc;
}

class ggo_hexa_artist
{
public:

  struct hexa_info
  {
    float       _x;
    float       _y;
    float       _height;
    std::string _key;
  };

  static  std::pair<ggo::color, ggo::color> generate_colors();
  static  std::vector<ggo::point3d_float>   generate_light_positions();
  static  std::vector<hexa_info>            generate_hexa_infos();

	static  void                              render(uint8_t * buffer, int width, int height,
                                                   const std::vector<hexa_info> & hexa_infos,
                                                   const ggo::color & color_top, const ggo::color & color_side,
                                                   const std::vector<ggo::point3d_float> & lights_pos,
                                                   const ggo::color & fog_color,
                                                   ggo::renderer_abc & renderer);
};

#endif
