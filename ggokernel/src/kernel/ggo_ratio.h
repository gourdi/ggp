#ifndef __GGO_RATIO__
#define __GGO_RATIO__

#include <cstdint>
#include <type_traits>

namespace ggo
{
  struct ratio
  {
    constexpr ratio(uint64_t k) : _num(k), _den(1) {}
    constexpr ratio(uint64_t num, uint64_t den) : _num(num), _den(den) {}

    uint64_t _num;
    uint64_t _den;

    void operator+=(ggo::ratio rhs);
    void operator-=(ggo::ratio rhs);
  };
}

// Equality.
namespace ggo
{
  inline constexpr bool operator==(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den == r1._den * r2._num;
  }

  inline constexpr bool operator!=(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._num * r2._den != r1._den * r2._num;
  }
}

// Arithmetics.
namespace ggo
{
  inline constexpr ggo::ratio operator*(ggo::ratio r1, ggo::ratio r2)
  {
    return { r1._num * r2._num, r1._den * r2._den };
  }

  inline constexpr ggo::ratio operator/(ggo::ratio r1, ggo::ratio r2)
  {
    return { r1._num * r2._den, r1._den * r2._num };
  }

  inline constexpr ggo::ratio operator+(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._den == r2._den ? ggo::ratio{ r1._num + r2._num, r1._den } : ggo::ratio{ r1._num * r2._den + r2._num * r1._den, r1._den * r2._den };
  }

  inline constexpr ggo::ratio operator-(ggo::ratio r1, ggo::ratio r2)
  {
    return r1._den == r2._den ? ggo::ratio{ r1._num - r2._num, r1._den } : ggo::ratio{ r1._num * r2._den - r2._num * r1._den, r1._den * r2._den };
  }

  inline void ratio::operator+=(ggo::ratio rhs)
  {
    *this = *this + rhs;
  }

  inline void ratio::operator-=(ggo::ratio rhs)
  {
    *this = *this - rhs;
  }
}

// Comparison.
namespace ggo
{
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
}

// Misc.
namespace ggo
{
  template <typename float_t>
  constexpr float_t to(ggo::ratio r)
  {
    static_assert(std::is_floating_point_v<float_t>);

    return static_cast<float_t>(r._num) / static_cast<float_t>(r._den);
  }
}

#endif

