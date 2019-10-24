#ifndef __GGO_EASE__
#define __GGO_EASE__

namespace ggo
{
  //////////////////////////////////////////////////////////////
  // t in [0, 1], result in [0, 1]
  template <typename data_t>
  data_t ease_in(data_t t)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");
    return -t * t + 2 * t;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t ease_in_to(int index, int count)
  {
    return ease_in(data_t(index) / data_t(count));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename interpolation_t = float>
  data_t ease_in(int index, int count, const data_t & start, const data_t & end)
  {
    interpolation_t t = ease_in_to<interpolation_t>(index, count);
    
    return start + t * (end - start);
  }

  //////////////////////////////////////////////////////////////
  // t in [0, 1], result in [0, 1]
  template <typename data_t>
  constexpr data_t ease_inout(data_t t)
  {
    static_assert(std::is_floating_point<data_t>::value);
    data_t sq = t * t;
    return -2 * t * sq + 3 * sq; // -2 * t^3 + 3 * t^2
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  constexpr data_t ease_inout_to(int index, int count)
  {
    return ease_inout(data_t(index) / data_t(count - 1));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename interpolation_t = float>
  data_t ease_inout(int index, int count, const data_t & start, const data_t & end)
  {
    interpolation_t t = ease_inout_to<interpolation_t>(index, count);
    
    return start + t * (end - start);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename interpolation_t = float>
  data_t ease_inout(int index, int index_start, int index_end, const data_t & start, const data_t & end)
  {
    return ease_inout(index - index_start, index_end - index_start, start, end);
  }
}

#endif