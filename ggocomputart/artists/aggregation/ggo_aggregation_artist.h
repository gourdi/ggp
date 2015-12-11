#ifndef __GGO_AGGREGATION_ARTIST__
#define __GGO_AGGREGATION_ARTIST__

#include <ggo_artist_abc.h>
#include <ggo_set3.h>

namespace ggo
{
  namespace aggregation_artist
  {
    std::vector<ggo::point3d_float> compute_points(float attraction_factor,
                                                   const ggo::vector3d_float rotation_vector,
                                                   float rotation_factor);
    
    void                            render(float background,
                                           const std::vector<ggo::point3d_float> & points,
                                           float angle,
                                           uint8_t * buffer,
                                           int width,
                                           int height);
  }
};

#endif
