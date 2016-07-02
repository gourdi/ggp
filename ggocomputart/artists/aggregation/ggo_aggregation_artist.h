#ifndef __GGO_AGGREGATION_ARTIST__
#define __GGO_AGGREGATION_ARTIST__

#include <ggo_artist_abc.h>

namespace ggo
{
  namespace aggregation_artist
  {
    std::vector<ggo::pos3f> compute_points(float attraction_factor,
                                           const ggo::vec3f rotation_vector,
                                           float rotation_factor);
    
    void                            render(float background,
                                           const std::vector<ggo::pos3f> & points,
                                           float angle,
                                           uint8_t * buffer,
                                           int width,
                                           int height);
  }
};

#endif
