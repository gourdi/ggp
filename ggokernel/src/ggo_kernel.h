#ifndef __GGO_KERNEL__
#define __GGO_KERNEL__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <exception>
#include <numeric>
#include <optional>

//////////////////////////////////////////////////////////////
// Debug.

#ifdef GGO_DEBUG
#ifdef GGO_WIN
  void GGO_ASSERT(bool b);
#else
  #include <assert.h>
  #define	GGO_ASSERT(zzz) assert(zzz)
#endif
  #define	GGO_TRACE(...) printf(__VA_ARGS__)
  #define	GGO_ASSERT_PTR(ptr) if (!ptr) { std::cerr << std::endl << #ptr << " is null" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_EQ(v1, v2) if ((v1) != (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] != " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_NE(v1, v2) if ((v1) == (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] == " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_LT(v1, v2) if ((v1) >= (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] >= " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_LE(v1, v2) if ((v1) >  (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] > "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_GT(v1, v2) if ((v1) <= (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] <= " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_GE(v1, v2) if ((v1) <  (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] < "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define GGO_ASSERT_FLOAT_EQ(v1, v2) if (std::abs(v1 - v2) > 0.001f) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] != "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
  #define	GGO_ASSERT_BTW(v1, v2, v3) if ((v1) < (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] < "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); } \
                                     if ((v1) > (v3)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] > "  << #v3 << " [=" << (v3) << "] !" << std::endl; GGO_FAIL(); }
#else
  #define	GGO_TRACE(...)
	#define	GGO_ASSERT(zzz)
  #define	GGO_ASSERT_PTR(ptr)
  #define	GGO_ASSERT_EQ(v1, v2)
  #define	GGO_ASSERT_NE(v1, v2)
  #define	GGO_ASSERT_LT(v1, v2)
  #define	GGO_ASSERT_LE(v1, v2)
  #define	GGO_ASSERT_GT(v1, v2)
  #define	GGO_ASSERT_GE(v1, v2)
  #define GGO_ASSERT_FLOAT_EQ(v1, v2)
  #define	GGO_ASSERT_BTW(v1, v2, v3)
#endif
#define	GGO_FAIL() GGO_ASSERT(0)

namespace ggo
{
  template <uint32_t log2_den>
  struct log2_fract final
  {
    log2_fract() = default;
    log2_fract(uint32_t num) : _num(num) {}

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
}

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
    if constexpr(std::is_floating_point<data_t>::value)
    {
      return rand<data_t>(0, 1);
    }
    else
    {
      return rand(std::numeric_limits<data_t>::lowest(), std::numeric_limits<data_t>::max());
    }
  }
  
  inline bool rand_bool() { return (rand<int>(0, 1) % 2) != 0; }
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
// Edges functions.
namespace ggo
{
  constexpr inline int loop_index(int i, int w)                   { return pos_mod(i, w); }
  constexpr inline int mirror_index_edge_duplicated(int i, int w) { i = pos_mod(i, 2 * w); return i < w ? i : 2 * w - i - 1; }
  constexpr inline int mirror_index_single_edge(int i, int w)     { i = pos_mod(i, 2 * w - 2) ; return i < w ? i : 2 * w - i- 2; }
}

#include <ggo_template_variadic.h>
#include <ggo_template_pointer.h>
#include <ggo_container_helpers.h>

//////////////////////////////////////////////////////////////
// Exceptions.
namespace ggo
{
  class dimension_mismatch_exception : public std::exception {};
  class inplace_exception : public std::exception {};
  class invalid_parameter_exception : public std::exception {};
}

//////////////////////////////////////////////////////////////
// Constants.
namespace ggo
{
  template<typename T> constexpr T pi() { return static_cast<T>(3.1415926535897932385); }
  template<typename T> constexpr T inv_sqrt2() { return static_cast<T>(0.707106781186547524); }
  template<typename T> constexpr T sqrt2() { return static_cast<T>(1.414213562373095049); }

  enum class direction
  {
    left,
    right,
    up,
    down
  };
}

//////////////////////////////////////////////////////////////
// String to value
namespace ggo
{
  template <typename data_t>
  std::optional<data_t> str_to(const std::string & str)
  {
    return {};
  }

  template <> inline
    std::optional<bool> str_to(const std::string & str)
  {
    if (str == "true")
    {
      return true;
    }
    if (str == "false")
    {
      return false;
    }
    return {};
  }

  template <> inline
    std::optional<int> str_to(const std::string & str)
  {
    std::optional<int> val;
    try
    {
      val = stoi(str);
    }
    catch (...)
    {

    }
    return val;
  }
}

//////////////////////////////////////////////////////////////
// Literals
inline constexpr uint8_t operator "" _u8(unsigned long long v) noexcept
{
  return static_cast<uint8_t>(v);
}

#endif

