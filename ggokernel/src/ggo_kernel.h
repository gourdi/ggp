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
  data_t rand(data_t inf, data_t sup, typename std::enable_if<std::is_floating_point<data_t>::value>::type* = 0)
  {
    return std::uniform_real_distribution<data_t>(inf, sup)(get_random_generator());
  }

  template <typename data_t>
  data_t rand(data_t inf, data_t sup, typename std::enable_if<std::is_integral<data_t>::value>::type* = 0)
  {
    // We must used 'long long' in order to be able to have uin8_t random number for instance.
    return static_cast<data_t>(std::uniform_int_distribution<long long>(inf, sup)(get_random_generator()));
  }

  template <typename data_t>
  data_t rand(typename std::enable_if<std::is_floating_point<data_t>::value>::type* = 0)
  {
    return rand<data_t>(0, 1);
  }

  template <typename data_t>
  data_t rand(typename std::enable_if<std::is_integral<data_t>::value>::type* = 0)
  {
    return rand(std::numeric_limits<data_t>::lowest(), std::numeric_limits<data_t>::max());
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
  data_t pos_mod(data_t v, data_t m, typename std::enable_if<std::is_floating_point<data_t>::value>::type* = 0)
  {
    v = std::fmod(v, m);
    return v < 0 ? v + m : v;
  }

  template <typename data_t>
  data_t pos_mod(data_t v, data_t m, typename std::enable_if<std::is_integral<data_t>::value>::type* = 0)
  {
    v = v % m;
    return v < 0 ? v + m : v;
  }

  template <int bit_shift, typename data_t>
  data_t fixed_point_div(data_t v)
  {
    static_assert(bit_shift > 1, "invalid bit shift");
    static_assert(std::is_integral<data_t>::value && std::is_unsigned<data_t>::value, "expected unsigned integral type");

    return (v + (1 << (bit_shift - 1))) >> bit_shift;
  }

  template <typename data_t>
  data_t round_div(data_t value, data_t div)
  { 
    static_assert(std::is_integral<data_t>::value, "expecting integral type");

    if (std::is_unsigned<data_t>::value == true)
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

  template <typename T>	T	      clamp(T v, T inf, T sup)				      { return v > sup ? sup : (v < inf ? inf : v); };
  template <typename T>	T	      square(T value)							          { return value * value; };
  template <typename T>	T	      sign(T value)							            { return value > T(0) ? T(1) : T(-1); };
  template <typename T> T       cotan(T angle)                        { return 1 / std::tan(angle); }
  inline					      int		  pad(int value, int pad)				        { return (((value-1)/pad)+1)*pad; };
  inline					      bool	  is_even(int value)						        { return (value&1)==0; };
  inline					      bool	  is_odd(int value)						          { return (value&1)==1; };
  inline					      int		  log2(int v)								            { int log2 = 1; while (v >>= 1) { ++log2; } return log2; }
}

//////////////////////////////////////////////////////////////
// Type casting
namespace ggo
{
  namespace hidden
  {
    template <typename out_t, typename in_t>
    out_t clamp_and_round(in_t v)
    {
      in_t clamped = clamp(v, static_cast<in_t>(std::numeric_limits<out_t>::min()), static_cast<in_t>(std::numeric_limits<out_t>::max()));
      return static_cast<out_t>(clamped < 0 ? clamped - in_t(0.5) : clamped + in_t(0.5));
    }
    
    template <typename out_t, typename in_t>
    out_t clamp_min_max(in_t v)
    {
      in_t clamped = clamp(v, static_cast<in_t>(std::numeric_limits<out_t>::min()), static_cast<in_t>(std::numeric_limits<out_t>::max()));
      return static_cast<out_t>(clamped);
    }
    
    template <typename out_t, typename in_t>
    out_t clamp_max(in_t v)
    {
      GGO_ASSERT(v >= 0);
      in_t clamped = std::min(v, static_cast<in_t>(std::numeric_limits<out_t>::max()));
      return static_cast<out_t>(clamped);
    }
  }

  template <typename out_t, typename in_t>  out_t to(in_t v) { return static_cast<out_t>(v); }

  // From float
  template <> inline int8_t    to(float v) { return hidden::clamp_and_round<int8_t, float>(v); }
  template <> inline uint8_t   to(float v) { return hidden::clamp_and_round<uint8_t, float>(v); }
  template <> inline int16_t   to(float v) { return hidden::clamp_and_round<int16_t, float>(v); }
  template <> inline uint16_t  to(float v) { return hidden::clamp_and_round<uint16_t, float>(v); }
  template <> inline int32_t   to(float v) { return hidden::clamp_and_round<int32_t, float>(v); }
  template <> inline uint32_t  to(float v) { return hidden::clamp_and_round<uint32_t, float>(v); }

  // From double
  template <> inline int8_t    to(double v) { return hidden::clamp_and_round<int8_t, double>(v); }
  template <> inline uint8_t   to(double v) { return hidden::clamp_and_round<uint8_t, double>(v); }
  template <> inline int16_t   to(double v) { return hidden::clamp_and_round<int16_t, double>(v); }
  template <> inline uint16_t  to(double v) { return hidden::clamp_and_round<uint16_t, double>(v); }
  template <> inline int32_t   to(double v) { return hidden::clamp_and_round<int32_t, double>(v); }
  template <> inline uint32_t  to(double v) { return hidden::clamp_and_round<uint32_t, double>(v); }
  
  // From int8_t
  template <> inline uint8_t   to(int8_t v) { return v < 0 ? 0 : v; }
  template <> inline uint16_t  to(int8_t v) { return v < 0 ? 0 : v; }
  template <> inline uint32_t  to(int8_t v) { return v < 0 ? 0 : v; }

  // From int16_t
  template <> inline int8_t    to(int16_t v) { return hidden::clamp_min_max<int8_t, int16_t>(v); }
  
  template <> inline uint8_t   to(int16_t v) { return hidden::clamp_min_max<uint8_t, int16_t>(v); }
  template <> inline uint16_t  to(int16_t v) { return v < 0 ? 0 : v; }
  template <> inline uint32_t  to(int16_t v) { return v < 0 ? 0 : v; }
  
  // From int32_t
  template <> inline int8_t    to(int32_t v) { return hidden::clamp_min_max<int8_t, int32_t>(v); }
  template <> inline int16_t   to(int32_t v) { return hidden::clamp_min_max<int16_t, int32_t>(v); }
  
  template <> inline uint8_t   to(int32_t v) { return hidden::clamp_min_max<uint8_t, int32_t>(v); }
  template <> inline uint16_t  to(int32_t v) { return hidden::clamp_min_max<uint16_t, int32_t>(v); }
  template <> inline uint32_t  to(int32_t v) { return v < 0 ? 0 : v; }
  
  // From uint8_t
  template <> inline int8_t    to(uint8_t v) { return hidden::clamp_max<int8_t, uint8_t>(v); }
  
  // From uint16_t
  template <> inline int8_t    to(uint16_t v) { return hidden::clamp_max<int8_t, uint16_t>(v); }
  template <> inline int16_t   to(uint16_t v) { return hidden::clamp_max<int16_t, uint16_t>(v); }
  
  template <> inline uint8_t   to(uint16_t v) { return hidden::clamp_max<uint8_t, uint16_t>(v); }
  
  // From uint32_t
  template <> inline int8_t    to(uint32_t v) { return hidden::clamp_max<int8_t, uint32_t>(v); }
  template <> inline int16_t   to(uint32_t v) { return hidden::clamp_max<int16_t, uint32_t>(v); }
  template <> inline int32_t   to(uint32_t v) { return hidden::clamp_max<int32_t, uint32_t>(v); }
  
  template <> inline uint8_t   to(uint32_t v) { return hidden::clamp_max<uint8_t, uint32_t>(v); }
  template <> inline uint16_t  to(uint32_t v) { return hidden::clamp_max<uint32_t, uint32_t>(v); }
}

//////////////////////////////////////////////////////////////
// Edges functions.
namespace ggo
{
  inline int loop_index(int i, int w)                   { return pos_mod(i, w); }
  inline int mirror_index_edge_duplicated(int i, int w) { i = pos_mod(i, 2 * w); return i < w ? i : 2 * w - i - 1; }
  inline int mirror_index_single_edge(int i, int w)     { i = pos_mod(i, 2 * w - 2) ; return i < w ? i : 2 * w - i- 2; }
}

//////////////////////////////////////////////////////////////
// Containers.
namespace ggo
{
  template <typename container, typename predicate>
  void remove_if(container & c, predicate p)
  {
    auto new_end = std::remove_if(c.begin(), c.end(), p);

    c.erase(new_end, c.end()); // Because std::remove_if does not remove, it just moves actually.
  }

  template <typename container, typename predicate>
  void remove_first_if(container & c, predicate p)
  {
    for (auto it = c.begin(); it != c.end(); ++it)
    {
      if (p(*it) == true)
      {
        c.erase(it);
        return;
      }
    }
  }

  template <typename container>
  void shuffle(container & c)
  {
    std::shuffle(c.begin(), c.end(), get_random_generator());
  }
  
  template <typename container, typename predicate>
  void sort(container & c, predicate p)
  {
    std::sort(c.begin(), c.end(), p);
  }

  template <typename container, typename predicate>
  bool find_if(const container & c, predicate p)
  {
    return std::find_if(c.begin(), c.end(), p) != c.end();
  }

  template <typename container, typename data_t>
  bool find(const container & c, const data_t & v)
  {
    return std::find(c.begin(), c.end(), v) != c.end();
  }

  template <typename container, typename data_t>
  void push_once(container & c, const data_t & v)
  {
    if (std::find(c.begin(), c.end(), v) == c.end())
    {
      c.push_back(v);
    }
  }

  template <typename container, typename data_t>
  void fill(container & c, const data_t & v)
  {
    std::fill(c.begin(), c.end(), v);
  }

  template <typename container, typename func_t, typename data_t>
  data_t accumulate(container & c, func_t f, data_t init)
  {
    return std::accumulate(c.begin(), c.end(), init, f);
  }
}

#include <ggo_template_variadic.h>
#include <ggo_template_pointer.h>

//////////////////////////////////////////////////////////////
// Exceptions.
namespace ggo
{
  class dimension_mismatch_exception : public std::exception {};
  class inplace_exception : public std::exception {};
}

//////////////////////////////////////////////////////////////
// Constants.
namespace ggo
{
  template<typename T> constexpr T pi() { return static_cast<T>(3.1415926535897932385); }
  template<typename T> constexpr T inv_sqrt2() { return static_cast<T>(0.707106781186547524); }
  template<typename T> constexpr T sqrt2() { return static_cast<T>(1.414213562373095049); }
}

#endif

