#ifndef __GGO_COMPARE__
#define __GGO_COMPARE__

#include <cmath>

namespace ggo
{
  namespace details
  {
    template <typename data_t, int count, int index>
    struct compare_t
    {
      static constexpr bool call(const data_t * ptr1, const data_t * ptr2)
      {
        return compare_t<data_t, count, index - 1>::call(ptr1, ptr2) && (ptr1[index] == ptr2[index]);
      }

      static constexpr bool call(const data_t * ptr1, const data_t * ptr2, data_t epsilon)
      {
        return compare_t<data_t, count, index - 1>::call(ptr1, ptr2) && (std::abs(ptr1[index] - ptr2[index]) < epsilon);
      }
    };

    template <typename data_t, int count>
    struct compare_t<data_t, count, 0>
    {
      static constexpr bool call(const data_t * ptr1, const data_t * ptr2)
      {
        return ptr1[0] == ptr2[0];
      }

      static constexpr bool call(const data_t * ptr1, const data_t * ptr2, data_t epsilon)
      {
        static_assert(std::is_floating_point_v<data_t>);
        return std::abs(ptr1[0] - ptr2[0]) < epsilon;
      }
    };
  }

  template <int size, typename data_t>
  constexpr bool compare(const data_t * ptr1, const data_t * ptr2)
  {
    return ggo::details::compare_t<data_t, size, size - 1>::call(ptr1, ptr2);
  }

  template <int size, typename data_t>
  constexpr bool compare(const data_t * ptr1, const data_t * ptr2, data_t epsilon)
  {
    return ggo::details::compare_t<data_t, size, size - 1>::call(ptr1, ptr2, epsilon);
  }

  template <int size, typename data_t>
  constexpr bool compare(data_t const (&buffer1)[size], data_t const (&buffer2)[size])
  {
    return ggo::details::compare_t<data_t, size, size - 1>::call(buffer1, buffer2);
  }

  template <int size, typename data_t>
  constexpr bool compare(data_t const (&buffer1)[size], data_t const (&buffer2)[size], data_t epsilon)
  {
    return ggo::details::compare_t<data_t, size, size - 1>::call(buffer1, buffer2, epsilon);
  }
}

#endif
