#ifndef __GGO_KERNEL__
#define __GGO_KERNEL__

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <optional>
#include <bitset>
#include <kernel/ggo_assert.h>

//////////////////////////////////////////////////////////////
// Random.
namespace ggo
{
  inline std::default_random_engine & get_random_generator()
  {
    static std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    return generator;
  };

  template <typename data_t>
  data_t rand(data_t inf, data_t sup)
  {
    GGO_ASSERT_LT(inf, sup);

    if constexpr(std::is_floating_point<data_t>::value)
    {
      return std::uniform_real_distribution<data_t>(inf, sup)(get_random_generator());
    }
    else
    {
      // We must used 'long long' in order to be able to have uin8_t random number for instance.
      return static_cast<data_t>(std::uniform_int_distribution<long long>(inf, sup)(get_random_generator()));
    }
  }

  template <typename data_t>
  data_t rand()
  {
    if constexpr(std::is_same<data_t, bool>::value)
    {
      return rand<int>(0, 1) % 2 != 0;
    }
    else if constexpr(std::is_floating_point<data_t>::value)
    {
      return rand<data_t>(0, 1);
    }
    else
    {
      return rand(std::numeric_limits<data_t>::lowest(), std::numeric_limits<data_t>::max());
    }
  }
}

//////////////////////////////////////////////////////////////
// Functions.
namespace ggo
{
  template <typename from_t, typename to_t>
  to_t map(from_t v, from_t inf_from, from_t sup_from, to_t inf_to, to_t sup_to)
  { 
    return inf_to + (v - inf_from ) * (sup_to - inf_to ) / (sup_from - inf_from);
  }

  template <typename data_t> 
  data_t pow(data_t base, data_t exponent)
  {
    static_assert(std::is_integral<data_t>::value, "expecting integral type");
    GGO_ASSERT(exponent >= 0);

    if (exponent == 0)
    {
      return 1;
    }

    data_t res = base;
    for (int i = 1; i < exponent; ++i)
    {
      res *= base;
    }
    return res;
  }

  template <typename data_t>
  constexpr data_t pos_mod(data_t v, data_t m)
  {
    if constexpr(std::is_floating_point<data_t>::value)
    {
      v = std::fmod(v, m);
      return v < 0 ? v + m : v;
    }
    else
    {
      if constexpr(std::is_unsigned<data_t>::value)
      {
        return v % m;
      }
      else
      {
        v = v % m;
        return v < 0 ? v + m : v;
      }
    }
  }

  template <uint32_t bit_shift, typename data_t>
  constexpr data_t fixed_point_div(data_t v)
  {
    static_assert(std::is_integral<data_t>::value);

    if constexpr(bit_shift == 0)
    {
      return v;
    }
    else
    {
      if constexpr(std::is_unsigned<data_t>::value)
      {
        return (v + (1 << (bit_shift - 1))) >> bit_shift;
      }
      else
      {
        if (v >= 0)
        {
          return (v + (1 << (bit_shift - 1))) >> bit_shift;
        }
        else
        {
          return -((-v + (1 << (bit_shift - 1))) >> bit_shift);
        }
      }
    }
  }

  template <typename data_t>
  constexpr data_t round_div(data_t value, data_t div)
  { 
    static_assert(std::is_integral<data_t>::value );

    if constexpr(std::is_unsigned<data_t>::value == true)
    {
      return (value + div / 2) / div;
    }
    else
    {
      if (div < 0)
      {
        value *= -1;
        div *= -1;
      }
      return value < 0 ? (value - div / 2) / div : (value + div / 2) / div;
    }
  };

  template <typename data_t, typename scalar_t = float>
  data_t linerp(const data_t & a, const data_t & b, scalar_t weight)
  {
    return weight * a + (1 - weight) * b;
  }

  template <typename data_t> constexpr  data_t  clamp(data_t v, data_t inf, data_t sup) { return v > sup ? sup : (v < inf ? inf : v); };
  template <typename data_t> constexpr  data_t  square(data_t value) { return value * value; };
  template <typename data_t> constexpr  data_t  sign(data_t value) { return value > data_t(0) ? data_t(1) : data_t(-1); };
  template <typename data_t> constexpr  data_t  cotan(data_t angle)                     { return 1 / std::tan(angle); }
  constexpr inline					            int		  pad(int value, int pad)				          { return (((value-1)/pad)+1)*pad; };
  template <typename data_t>constexpr   bool	  is_even(data_t value) { return (value & 1) == 0; };
  template <typename data_t>constexpr   bool	  is_odd(data_t value)						        { return (value & 1) == 1; };
  inline					                      int		  log2(int v)								              { int log2 = 1; while (v >>= 1) { ++log2; } return log2; }
}

//////////////////////////////////////////////////////////////
// log2_fract
namespace ggo
{
  template <uint32_t log2_den>
  struct log2_fract final
  {
    log2_fract() = default;
    constexpr log2_fract(uint32_t num) : _num(num) {}

    uint32_t _num = 0;
    static constexpr uint32_t _log2_den = log2_den;
    static constexpr uint32_t _den = 1 << log2_den;

    template <typename real_t>
    real_t to() const
    {
      static_assert(std::is_floating_point<real_t>::value == true);
      return real_t(_num) / real_t(_den);
    }
  };

  constexpr log2_fract<0> one_log2_fract(1); // One.

  template <uint32_t log2_den>
  constexpr uint32_t operator*(const log2_fract<log2_den> & fract, uint32_t v)
  {
    return ggo::fixed_point_div<log2_den>(fract._num * v);
  }

  template <uint32_t log2_den>
  constexpr uint32_t operator*(uint32_t v, const log2_fract<log2_den> & fract)
  {
    return ggo::fixed_point_div<log2_den>(fract._num * v);
  }
}

//////////////////////////////////////////////////////////////
// Type casting
namespace ggo
{
  enum class cast_mode
  {
    regular,
    round,
    clamp_round
  };

  template <typename out_t, typename in_t>
  constexpr out_t round_to(in_t v)
  {
    static_assert(std::is_floating_point<in_t>::value);
    static_assert(std::is_integral<out_t>::value);

    if constexpr(std::is_unsigned<out_t>::value)
    {
      return static_cast<out_t>(v + in_t(0.5));
    }
    else
    {
      return v > 0 ? static_cast<out_t>(v + in_t(0.5)) : static_cast<out_t>(v - in_t(0.5));
    }
  }

  template <typename out_t, typename in_t>
  constexpr out_t clamp_and_round_to(in_t v)
  {
    static_assert(std::is_floating_point<in_t>::value);
    static_assert(std::is_integral<out_t>::value);

    constexpr in_t inf = static_cast<in_t>(std::numeric_limits<out_t>::min());
    constexpr in_t sup = static_cast<in_t>(std::numeric_limits<out_t>::max());

    return round_to<out_t>(clamp(v, inf, sup));
  }

  template <typename out_t, cast_mode mode, typename in_t>
  constexpr out_t cast_to(in_t v)
  {
    if constexpr(mode == cast_mode::regular)
      return static_cast<out_t>(v);
    else if constexpr(mode == cast_mode::round)
      return round_to<out_t>(v);
    else if constexpr(mode == cast_mode::clamp_round)
      return clamp_and_round_to<out_t>(v);
    return 0;
  }
}

//////////////////////////////////////////////////////////////
// Average.
namespace ggo
{
  template <typename data_t, typename... args>
  constexpr data_t average(data_t v, args... a)
  {
    if constexpr (std::is_integral<data_t>::value == true)
    {
      if constexpr (std::is_unsigned<data_t>::value == true)
      {
        return (sum(v, a...) + (1 + sizeof...(a)) / 2) / (1 + sizeof...(a));
      }
      else
      {
        auto s = sum(v, a...);
        if (s > 0)
        {
          return (s + (1 + sizeof...(a)) / 2) / (1 + sizeof...(a));
        }
        else
        {
          data_t num = s - (1 + sizeof...(a)) / 2;
          data_t den = 1 + sizeof...(a); // Force conversion to signed type.
          return num / den;
        }
      }
    }
    else
    {
      return sum(v, a...) / (1 + sizeof...(a));
    }
  }

  template <typename... args>
  constexpr uint8_t average(uint8_t v, args... a)
  {
    return static_cast<uint8_t>((sum_to<uint32_t>(v, a...) + (1 + sizeof...(a)) / 2) / (1 + sizeof...(a)));
  }
}

//////////////////////////////////////////////////////////////
// MinMax.
namespace ggo
{
  template <typename data_t>
  constexpr std::pair<data_t, data_t> minmax(data_t v1, data_t v2)
  {
    return v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1);
  }

  template <typename data_t, typename... args>
  constexpr std::pair<data_t, data_t> minmax(data_t v1, data_t v2, args... a)
  {
    auto result = minmax(v2, a...);
    if (v1 < result.first)
    {
      return std::make_pair(v1, result.second);
    }
    else if (v2 > result.second)
    {
      return std::make_pair(result.first, v2);
    }
    else
    {
      return result;
    }
  }
}
  
#include <kernel/ggo_container_helpers.h>

//////////////////////////////////////////////////////////////
// Constants.
namespace ggo
{
  template<typename T> constexpr T pi() { return static_cast<T>(3.1415926535897932385); }
  template<typename T> constexpr T inv_sqrt2() { return static_cast<T>(0.707106781186547524); }
  template<typename T> constexpr T sqrt2() { return static_cast<T>(1.414213562373095049); }

  constexpr float   pi_f = pi<float>();
  constexpr double  pi_d = pi<double>();

  enum class lines_order
  {
    down,
    up
  };

  inline std::ostream & operator<<(std::ostream & os, lines_order lo)
  {
    switch (lo)
    {
    case lines_order::down:
      os << "lines_order::down"; break;
    case lines_order::up:
      os << "lines_order::up"; break;
    }
    return os;
  }
}

//////////////////////////////////////////////////////////////
// Range.
namespace ggo
{
  template <typename data_t>
  struct range
  {
    range(data_t inf, data_t sup) : _inf(inf), _sup(sup) {}
    data_t _inf;
    data_t _sup;
  };

  using range_f = range<float>;

  template <typename data_t>
  std::optional<range<data_t>> get_range_intersection(range<data_t> r1, range<data_t> r2)
  {
    if (r1._sup < r2._inf || r2._sup < r1._inf)
    {
      return {};
    }
    return range<data_t>(std::max(r1._inf, r2._inf), std::min(r1._sup, r2._sup));
  }
}

//////////////////////////////////////////////////////////////
// Bit mask.
namespace ggo
{
  template <typename enum_t>
  struct bit_mask
  {
    using type = std::underlying_type_t<enum_t>;

    static_assert(std::is_enum_v<enum_t>);
    static_assert(std::is_integral_v<type>);
    static_assert(std::numeric_limits<type>::radix == 2);

    type _flags;

    bit_mask(std::initializer_list<enum_t> flags)
    {
      _flags = 0;

      for (const auto & flag : flags)
      {
        GGO_ASSERT_GT(static_cast<type>(flag), 0);
        GGO_ASSERT_EQ(std::bitset<std::numeric_limits<type>::digits>(static_cast<type>(flag)).count(), 1);
        _flags |= static_cast<type>(flag);
      }
    }

    bool has(enum_t e)
    {
      return (_flags & static_cast<type>(e)) != 0;
    }
  };
}

#endif

