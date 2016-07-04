#ifndef __GGO_ENTABENI__
#define __GGO_ENTABENI__

#include <ggo_array.h>
#include <ggo_curve.h>
#include <ggo_color.h>
#include <ggo_image_abc.h>

class ggo_entabeni
{
public:

  static  ggo::array<float, 2>                create_grid(bool loop_x, bool loop_y);
  static  ggo::cubic_curve<float, ggo::color> create_color_map();

  static  void                                render_bitmap(ggo::rgb_image_abc & image,
                                                            const ggo::array<float, 2> & grid,
                                                            const ggo::cubic_curve<float, ggo::color> & color_map,
                                                            float z,
                                                            float angle);
};

#endif
