#ifndef __GGO_RATIO__
#define __GGO_RATIO__

#include <cstdint>
#include <type_traits>

namespace ggo
{
  struct ratio
  {
    uint64_t _num;
    uint64_t _den;

    constexpr bool operator==(ggo::ratio rhs) const { return _num * rhs._den == _den * rhs._num; }
    constexpr bool operator!=(ggo::ratio rhs) const { return _num * rhs._den != _den * rhs._num; }

    void operator+=(ggo::ratio rhs);
  };

  inline constexpr ratio operator*(ggo::ratio r1, ggo::ratio r2)
  {
    return { r1._num * r2._num, r1._den * r2._den };
  }

  inline constexpr ratio operator*(ggo::ratio r, uint64_t k)
  {
    return { k * r._num, r._den };
  }

  inline constexpr ratio operator*(uint64_t k, ggo::ratio r)
  {
    return { k * r._num, r._den };
  }

  inline constexpr ratio operator/(ggo::ratio r1, ggo::ratio r2)
  {
    return { r1._num * r2._den, r1._den * r2._num };
  }

  inline constexpr ratio operator/(uint64_t k, ggo::ratio r)
  {
    return { k * r._den, r._num };
  }

  inline constexpr ratio operator/(ggo::ratio r, uint64_t k)
  {
    return { r._num, k * r._den };
  }

  inline constexpr ratio operator+(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._den == r2._den ? ratio{ r1._num + r2._num, r1._den } : ratio{ r1._num * r2._den + r2._num * r1._den, r1._den * r2._den };
  }

  inline constexpr bool operator<(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den < r2._num * r1._den;
  }

  inline constexpr bool operator<(uint64_t k, ggo::ratio r)
  {
    return k * r._den < r._num;
  }

  inline constexpr bool operator<(ggo::ratio r, uint64_t k)
  {
    return r._num < k * r._den;
  }

  inline constexpr bool operator<=(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den <= r2._num * r1._den;
  }

  inline constexpr bool operator<=(uint64_t k, ggo::ratio r)
  {
    return k * r._den < r._num;
  }

  inline constexpr bool operator<=(ggo::ratio r, uint64_t k)
  {
    return r._num < k * r._den;
  }

  inline constexpr bool operator>(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den > r2._num * r1._den;
  }

  inline constexpr bool operator>(uint64_t k, ggo::ratio r)
  {
    return k * r._den > r._num;
  }

  inline constexpr bool operator>(ggo::ratio r, uint64_t k)
  {
    return r._num > k * r._den;
  }

  inline constexpr bool operator>=(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den >= r2._num * r1._den;
  }

  inline constexpr bool operator>=(uint64_t k, ggo::ratio r)
  {
    return k * r._den >= r._num;
  }

  inline constexpr bool operator>=(ggo::ratio r, uint64_t k)
  {
    return r._num >= k * r._den;
  }

  inline void ratio::operator+=(ggo::ratio rhs)
  {
    *this = *this + rhs;
  }

  template <typename float_t>
  constexpr float_t to(ratio r)
  {
    static_assert(std::is_floating_point_v<float_t>);

    return static_cast<float_t>(r._num) / static_cast<float_t>(r._den);
  }
}

#endif

