#ifndef __GGO_EASE__
#define __GGO_EASE__

namespace ggo
{
  //////////////////////////////////////////////////////////////
  // t in [0, 1], result in [0, 1]
  inline float ease_in(float t)
  {
    return -t * t + 2 *t;
  }

  //////////////////////////////////////////////////////////////
  inline float ease_in(int index, int count)
  {
    return ease_in(float(index) / float(count));
  }

  //////////////////////////////////////////////////////////////
  inline float ease_in(int index, int count, float start, float end)
  {
    float t = ease_in(index, count);
    
    return start + t * (end - start);
  }

  //////////////////////////////////////////////////////////////
  // t in [0, 1], result in [0, 1]
  inline float ease_inout(float t)
  {
    return (1 - std::cos(t * PI<float>())) / 2;
  }

  //////////////////////////////////////////////////////////////
  inline float ease_inout(int index, int count)
  {
    return ease_inout(float(index) / float(count));
  }

  //////////////////////////////////////////////////////////////
  inline float ease_inout(int index, int count, float start, float end)
  {
    float t = ease_inout(index, count);
    
    return start + t * (end - start);
  }

  //////////////////////////////////////////////////////////////
  inline ggo::pos2f ease_inout(int index, int count, const ggo::pos2f & start, const ggo::pos2f & end)
  {
    float t = ease_inout(index, count);

    return ggo::pos2f(start + t * (end - start));
  }
}

#endif