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

  template <typename T> T rand_int(T inf, T sup)		  { return std::uniform_int_distribution<T>(inf, sup)(get_random_generator()); }
  template <typename T> T rand_real(T inf, T sup)		  { return std::uniform_real_distribution<T>(inf, sup)(get_random_generator()); }
  
  inline  int		  rand_int(int inf, int sup)				  { return rand_int<int>(inf, sup); }
  inline  float	  rand_float()							          { return rand_real<float>(0, 1); }
  inline  float	  rand_float(float inf, float sup)		{ return rand_real<float>(inf, sup); }
  inline  double	rand_double()							          { return rand_real<double>(0, 1); }
  inline  double	rand_double(double inf, double sup)	{ return rand_real<double>(inf, sup); }
  inline  bool	  rand_bool()								          { return (rand_int(0, 1) % 2) != 0; }
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

  template <typename T>	T	      clamp(T v, T inf, T sup)				      { return v > sup ? sup : (v < inf ? inf : v); };
  template <typename T>	T	      square(T value)							          { return value*value; };
  template <typename T>	T	      sign(T value)							            { return value > T(0) ? T(1) : T(-1); };
  template <typename T>	T	      pos_mod(T v, T m)						          { v = v % m; return v < 0 ? v + m : v; }
  template <typename T> T       cotan(T angle)                        { return 1 / std::tan(angle); }
  inline 					      int		  get_2d_index(int x, int y, int width)	{ return y*width+x; };
  inline					      int		  pad(int value, int pad)				        { return (((value-1)/pad)+1)*pad; };
  inline					      bool	  is_even(int value)						        { return (value&1)==0; };
  inline					      bool	  is_odd(int value)						          { return (value&1)==1; };
  inline					      int		  round_div(int value, int div)	        { return (value+div/2)/div; };
  inline					      double  pos_fmod(double v, double m)	        { v = std::fmod(v, m); return v < 0 ? v + m : v; } 
  inline					      float	  pos_fmod(float v, float m)		        { v = std::fmod(v, m); return v < 0 ? v + m : v; } 
  inline					      int		  log2(int v)								            { int log2 = 1; while (v >>= 1) { ++log2; } return log2; }

  inline int gcd(int v1, int v2)
  {
    int inf = std::min(v1, v2);
    
    int res = 1;
    for (int i = 2; i <= inf; ++i)
    {
      if (((v1 % i) == 0) && ((v2 % i) == 0))
      {
        res = i;
      }
    }
    
    return res;
  }
}

//////////////////////////////////////////////////////////////
// Type casting
namespace ggo
{
  namespace hidden
  {
    template <typename T_OUT, typename T_IN>
    T_OUT clamp_and_round(T_IN v)
    {
      T_IN clamped = clamp(v, static_cast<T_IN>(std::numeric_limits<T_OUT>::min()), static_cast<T_IN>(std::numeric_limits<T_OUT>::max()));
      return static_cast<T_OUT>(clamped < 0 ? clamped - T_IN(0.5) : clamped + T_IN(0.5));
    }
    
    template <typename T_OUT, typename T_IN>
    T_OUT clamp_min_max(T_IN v)
    {
      T_IN clamped = clamp(v, static_cast<T_IN>(std::numeric_limits<T_OUT>::min()), static_cast<T_IN>(std::numeric_limits<T_OUT>::max()));
      return static_cast<T_OUT>(clamped);
    }
    
    template <typename T_OUT, typename T_IN>
    T_OUT clamp_max(T_IN v)
    {
      GGO_ASSERT(v >= 0);
      T_IN clamped = std::min(v, static_cast<T_IN>(std::numeric_limits<T_OUT>::max()));
      return static_cast<T_OUT>(clamped);
    }
  }

  template <typename T_OUT, typename T_IN>  T_OUT to(T_IN v) { return static_cast<T_OUT>(v); }

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

//////////////////////////////////////////////////////////////
// Variadic templates.
namespace ggo
{
  // Min/max.
  template <typename T>
  T min(const T & a, const T & b)
  {
    return std::min(a, b);
  }

  template <typename T>
  T max(const T & a, const T & b)
  {
    return std::max(a, b);
  }

  template <typename T, typename... Args>
  T min(const T & v, Args... args)
  {
    return ggo::min(v, ggo::min(args...));
  }

  template <typename T, typename... Args>
  T max(const T & v, Args... args)
  {
    return ggo::max(v, ggo::max(args...));
  }

  // Sum/average.
  template <typename data_t>
  data_t sum(data_t v)
  {
    return v;
  }

  template <typename data_t, typename... args>
  data_t sum(const data_t & v, args... a)
  {
    return v + sum(a...);
  }

  template <typename output_t, typename data_t>
  output_t sum_to(const data_t & v)
  {
    return static_cast<output_t>(v);
  }

  template <typename output_t, typename data_t, typename... args>
  output_t sum_to(data_t v, args... a)
  {
    return static_cast<output_t>(v) + sum_to<output_t>(a...);
  }

  template <typename data_t, typename... args>
  data_t average(data_t v, args... a)
  {
    if (std::is_integral<data_t>::value == true)
    {
      if (std::is_unsigned<data_t>::value == true)
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
  uint8_t average(uint8_t v, args... a)
  {
    return static_cast<uint8_t>((sum_to<uint32_t>(v, a...) + (1 + sizeof...(a)) / 2) / (1 + sizeof...(a)));
  }

  // Add.
  template <typename data_t>
  void add(data_t * ptr, data_t v)
  {
    ptr[0] += v;
  }

  template <typename data_t, typename... args>
  void add(data_t * ptr, data_t v, args... a)
  {
    ptr[0] += v;
    add(ptr + 1, a...);
  }

  template <typename data_t, int count>
  struct mul_t
  {
    static void mul(data_t * ptr, data_t k)
    {
      ptr[0] *= k;
      mul_t<data_t, count - 1>::mul(ptr + 1, k);
    }
  };

  template <typename data_t>
  struct mul_t<data_t, 1>
  {
    static void mul(data_t * ptr, data_t k)
    {
      ptr[0] *= k;
    }
  };

  template <int count, typename data_t>
  void mul(data_t * ptr, data_t k)
  {
    ggo::mul_t<data_t, count>::mul(ptr, k);
  }

  // Binary operations.
  template <typename data_t, typename func, int count>
  struct binary_operation_t
  {
    static void process(data_t * dst, const data_t * src1, const data_t * src2, func f)
    {
      dst[0] = f(src1[0], src2[0]);
      ggo::binary_operation_t<data_t, func, count - 1>::process(dst + 1, src1 + 1, src2 + 1, f);
    }
  };

  template <typename data_t, typename func>
  struct binary_operation_t<data_t, func, 1>
  {
    static void process(data_t * dst, const data_t * src1, const data_t * src2, func f)
    {
      dst[0] = f(src1[0], src2[0]);
    }
  };

  template <int count, typename data_t, typename func>
  void binary_operation(data_t * dst, const data_t * src1, const data_t * src2, func f)
  {
    binary_operation_t<data_t, func, count>::process(dst, src1, src2, f);
  }

  // Copy a buffer.
  template <typename data_t, int count>
  struct copy_t
  {
    static void copy(data_t * dst, const data_t * src)
    {
      copy_t<data_t, count - 1>::copy(dst, src);
      dst[count - 1] = src[count - 1];
    }
  };

  template <typename data_t>
  struct copy_t<data_t, 0>
  {
    static void copy(data_t * dst, const data_t * src)
    {
    }
  };

  template <int count, typename data_t>
  void copy(data_t * dst, const data_t * src)
  {
    copy_t<data_t, count>::copy(dst, src);
  }

  // Compare a buffer.
  template <typename data_t, int count>
  struct compare_t
  {
    static bool compare(const data_t * ptr1, const data_t * ptr2)
    {
      if (ptr1[0] != ptr2[0])
      {
        return false;
      }

      return compare_t<data_t, count - 1>::compare(ptr1 + 1, ptr2 + 1);
    }

    static bool compare(const data_t * ptr1, const data_t * ptr2, data_t tolerance)
    {
      if (std::abs(ptr1[0] - ptr2[0]) > tolerance)
      {
        return false;
      }

      return compare_t<data_t, count - 1>::compare(ptr1 + 1, ptr2 + 1, tolerance);
    }
  };

  template <typename data_t>
  struct compare_t<data_t, 1>
  {
    static bool compare(const data_t * ptr1, const data_t * ptr2)
    {
      return ptr1[0] == ptr2[0];
    }

    static bool compare(const data_t * ptr1, const data_t * ptr2, data_t tolerance)
    {
      return std::abs(ptr1[0] - ptr2[0]) <= tolerance;
    }
  };

  template <int count, typename data_t>
  bool compare(const data_t * ptr1, const data_t * ptr2)
  {
    return ggo::compare_t<data_t, count>::compare(ptr1, ptr2);
  }

  template <int count, typename data_t>
  bool compare(const data_t * ptr1, const data_t * ptr2, data_t tolerance)
  {
    return ggo::compare_t<data_t, count>::compare(ptr1, ptr2, tolerance);
  }

  // Set a buffer.
  template <typename data_t>
  void set(data_t * ptr, const data_t & v)
  {
    ptr[0] = v;
  }

  template <typename data_t, typename... args>
  void set(data_t * ptr, const data_t & v, args... a)
  {
    ptr[0] = v;
    ggo::set(ptr + 1, a...);
  }

  template <typename data_t, typename func, int count>
  struct set_t
  {
    static void set(data_t * dst, const data_t * src, func f)
    {
      dst[0] = f(src[0]);
      ggo::set_t<data_t, func, count - 1>::set(dst + 1, src + 1, f);
    }

    static void set(data_t * dst, const data_t * src1, const data_t * src2, func f)
    {
      dst[0] = f(src1[0], src2[0]);
      ggo::set_t<data_t, func, count - 1>::set(dst + 1, src1 + 1, src2 + 1, f);
    }
  };

  template <typename data_t, typename func>
  struct set_t<data_t, func, 1>
  {
    static void set(data_t * dst, const data_t * src, func f)
    {
      dst[0] = f(src[0]);
    }

    static void set(data_t * dst, const data_t * src1, const data_t * src2, func f)
    {
      dst[0] = f(src1[0], src2[0]);
    }
  };

  template <int count, typename data_t, typename func>
  void set(data_t * dst, const data_t * src, func f)
  {
    ggo::set_t<data_t, func, count>::set(dst, src, f);
  }

  template <int count, typename data_t, typename func>
  void set(data_t * dst, const data_t * src1, const data_t * src2, func f)
  {
    ggo::set_t<data_t, func, count>::set(dst, src1, src2, f);
  }

  // Dot product.
  template <typename data_t, int count>
  struct dot_t
  {
    static data_t dot(const data_t* ptr1, const data_t* ptr2)
    {
      return ptr1[0] * ptr2[0] + ggo::dot_t<data_t, count - 1>::dot(ptr1 + 1, ptr2 + 1);
    }
  };

  template <typename data_t>
  struct dot_t<data_t, 1>
  {
    static data_t dot(const data_t* ptr1, const data_t* ptr2)
    {
      return ptr1[0] * ptr2[0];
    }
  };
  
  template <int count, typename data_t>
  data_t dot(const data_t* ptr1, const data_t* ptr2)
  {
    return ggo::dot_t<data_t, count>::dot(ptr1, ptr2);
  }

  // Transform.
  template <typename data_t, typename func, int count>
  struct transform_t
  {
    static void transform(data_t * ptr, func f)
    {
      ptr[0] = f(ptr[0]);
      ggo::transform_t<data_t, func, count - 1>::transform(ptr + 1, f);
    }
  };

  template <typename data_t, typename func>
  struct transform_t<data_t, func, 1>
  {
    static void transform(data_t * ptr, func f)
    {
      ptr[0] = f(ptr[0]);
    }
  };

  template <int count, typename data_t, typename func>
  void transform(data_t * ptr, func f)
  {
    ggo::transform_t<data_t, func, count>::transform(ptr, f);
  }

  // Dump.
  template <typename data_t, int count>
  struct dump_t
  {
    static void dump(const data_t * ptr, std::ostream & os)
    {
      os << ptr[0] << "; ";
      ggo::dump_t<data_t, count - 1>::dump(ptr + 1, os);
    }
  };

  template <typename data_t>
  struct dump_t<data_t, 1>
  {
    static void dump(const data_t * ptr, std::ostream & os)
    {
      os << ptr[0];
    }
  };

  template <int count, typename data_t>
  void dump(const data_t * ptr, std::ostream & os)
  {
    os << "(";
    ggo::dump_t<data_t, count>::dump(ptr, os);
    os << ")";
  }
}

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
  template<typename T> constexpr T PI() { return static_cast<T>(3.1415926535897932385); }
  template<typename T> constexpr T INV_SQRT2() { return static_cast<T>(0.707106781186547524); }
  template<typename T> constexpr T SQRT2() { return static_cast<T>(1.414213562373095049); }
}

#endif

