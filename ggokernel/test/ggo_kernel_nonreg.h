#ifndef __GGO_KERNEL_NON_REG__
#define __GGO_KERNEL_NON_REG__

#include <vector>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>

/////////////////////////////////////////////////////////////////////
inline bool find_point(const std::vector<ggo::pos2_f> & v, const ggo::pos2_f & p)
{
  for (const auto & pp : v)
  {
    if (ggo::compare(pp, p) == true)
    {
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
inline bool find_point(const std::vector<ggo::pos2_f> & v, float x, float y)
{
  return find_point(v, ggo::pos2_f(x, y));
}

/////////////////////////////////////////////////////////////////////
inline bool compare_segment(const ggo::segment_f & s1, const ggo::segment_f & s2)
{
  if (ggo::compare(s1.p1(), s2.p1()) == true && ggo::compare(s1.p2(), s2.p2()) == true)
  {
    return true;
  }

  if (ggo::compare(s1.p1(), s2.p2()) == true && ggo::compare(s1.p2(), s2.p1()) == true)
  {
    return true;
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
inline bool find_segment(const std::vector<ggo::segment_f> & v, const ggo::segment_f & s)
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
