#ifndef __GGO_CIRCLES_ARTIST__
#define __GGO_CIRCLES_ARTIST__

#include <ggo_artist_abc.h>

class ggo_circles_artist : public ggo_artist_abc
{
public:

  struct ggo_colored_disc
  {
    ggo::disc_float _disc;
    ggo::color      _color;
  };

                                              ggo_circles_artist(int render_width, int render_height);
  
  std::vector<std::vector<ggo_colored_disc>>  generate_discs() const;
  
  static  void  paint_disc(uint8_t * buffer, int width, int height, const ggo::pos2f & pos, float radius, const ggo::color & color);
  static  void  paint_disc(uint8_t * buffer, int width, int height, const ggo_colored_disc & colored_disc);
};

#endif