#include "ggo_best_candidate_sequence.h"
#include <ggo_kernel.h>
#include <ggo_distance2d.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::vector<ggo::point2d_float> best_candidate_sequence_2d(int count)
  {
    std::vector<ggo::point2d_float> result;

    while (result.size() < static_cast<size_t>(count))
    {
      float hypot = 0;
      ggo::point2d_float point;
      
      for (int i = 0; i < 64; ++i)
      {
        ggo::point2d_float point_cur(ggo::rand_float(), ggo::rand_float());
        
        float hypot_cur = std::numeric_limits<float>::max();
        
        for (const auto & p : result)
        {
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 1, p.y() + 1, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 1, p.y() + 0, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 1, p.y() - 1, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 0, p.y() + 1, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 0, p.y() + 0, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() + 0, p.y() - 1, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() - 1, p.y() + 1, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() - 1, p.y() + 0, point_cur.x(), point_cur.y()));
          hypot_cur = std::min(hypot_cur, ggo::hypot(p.x() - 1, p.y() - 1, point_cur.x(), point_cur.y()));
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
