#ifndef __GGO_KERNEL_NON_REG__
#define __GGO_KERNEL_NON_REG__

#include <vector>
#include <ggo_nonreg.h>
#include <ggo_shapes2d.h>

/////////////////////////////////////////////////////////////////////
inline bool find_point(const std::vector<ggo::pos2f> & v, const ggo::pos2f & p)
{
  for (const auto & pp : v)
  {
    if (std::fabs(pp.x() - p.x()) < 0.001 && std::fabs(pp.y() - p.y()) < 0.001f)
    {
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
inline bool find_point(const std::vector<ggo::pos2f> & v, float x, float y)
{
  return find_point(v, ggo::pos2f(x, y));
}

/////////////////////////////////////////////////////////////////////
inline bool compare_segment(const ggo::segment_float & s1, const ggo::segment_float & s2)
{
  if (std::fabs(s1.p1().x() - s2.p1().x()) < 0.001 &&
      std::fabs(s1.p1().y() - s2.p1().y()) < 0.001 &&
      std::fabs(s1.p2().x() - s2.p2().x()) < 0.001 &&
      std::fabs(s1.p2().y() - s2.p2().y()) < 0.001)
  {
    return true;
  }

  if (std::fabs(s1.p1().x() - s2.p2().x()) < 0.001 &&
      std::fabs(s1.p1().y() - s2.p2().y()) < 0.001 &&
      std::fabs(s1.p2().x() - s2.p1().x()) < 0.001 &&
      std::fabs(s1.p2().y() - s2.p1().y()) < 0.001)
  {
    return true;
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
inline bool find_segment(const std::vector<ggo::segment_float> & v, const ggo::segment_float & s)
{
  for (const auto & ss : v)
  {
    if (compare_segment(ss, s) == true)
    {
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
#define GGO_CHECK_RECT_INTERSECTION(shape, left, right, bottom, top, res) \
  { ggo::rect_data<float> rect_data {{static_cast<float>(left), static_cast<float>(bottom)}, \
                                      static_cast<float>(right - left), \
                                      static_cast<float>(top - bottom)}; \
  GGO_CHECK(shape.get_rect_intersection(rect_data) == res); }

#endif
