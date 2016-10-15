#include "ggo_best_candidate_sequence.h"
#include <ggo_kernel.h>
#include <ggo_distance.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::vector<ggo::pos2f> best_candidate_sequence_2d(int count)
  {
    std::vector<ggo::pos2f> result;

    while (result.size() < static_cast<size_t>(count))
    {
      float hypot = 0;
      ggo::pos2f point;
      
      for (int i = 0; i < 64; ++i)
      {
        ggo::pos2f point_cur(ggo::rand<float>(), ggo::rand<float>());
        
        float hypot_cur = std::numeric_limits<float>::max();
        
        for (const auto & p : result)
        {
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 1, p.get<1>() + 1, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 1, p.get<1>() + 0, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 1, p.get<1>() - 1, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 0, p.get<1>() + 1, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 0, p.get<1>() + 0, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() + 0, p.get<1>() - 1, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() - 1, p.get<1>() + 1, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() - 1, p.get<1>() + 0, point_cur.get<0>(), point_cur.get<1>()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.get<0>() - 1, p.get<1>() - 1, point_cur.get<0>(), point_cur.get<1>()));
        }
        
        if (hypot_cur > hypot)
        {
          point = point_cur;
          hypot = hypot_cur;
        }
      }
      
      result.push_back(point);
    }
    
    return result;
  }
}
