#include "ggo_best_candidate_sequence.h"
#include <kernel/ggo_kernel.h>
#include <kernel/math/ggo_distance.h>

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
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 1, p.y() + 1), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 1, p.y() + 0), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 1, p.y() - 1), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 0, p.y() + 1), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 0, p.y() + 0), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() + 0, p.y() - 1), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() - 1, p.y() + 1), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() - 1, p.y() + 0), point_cur));
          hypot_cur = std::min(hypot_cur, ggo::hypot(ggo::pos2f(p.x() - 1, p.y() - 1), point_cur));
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
