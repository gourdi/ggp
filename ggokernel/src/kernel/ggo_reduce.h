#ifndef __GGO_REDUCE__
#define __GGO_REDUCE__

#include <stdint.h>
#include <algorithm>

// Given a binary lambda f and a list of parameters (a, b, c, ..., y, z),
// returns f(a, f(b, f(c, f(..., f(y, z)...))),
// assuming the type of return value of f is the same of the input parameters.
namespace ggo
{
  // Variadic.
  template <typename func, typename data_t>
  constexpr data_t reduce(func f, const data_t & v1, const data_t & v2)
  {
    return f(v1, v2);
  }

  template <typename func, typename data_t, typename... args>
  constexpr data_t reduce(func f, const data_t & v, args... a)
  {
    return f(v, ggo::reduce(f, a...));
  }

  // Buffer.
  namespace details
  {
    template <typename data_t, int count>
    struct reduce_t
    {
      template <typename func>
      constexpr static data_t call(func f, const data_t * ptr)
      {
        static_assert(count > 2);
        return f(*ptr, reduce_t<data_t, count - 1>::call(f, ptr + 1));
      }
    };

    template <typename data_t>
    struct reduce_t<data_t, 2>
    {
      template <typename func>
      constexpr static data_t call(func f, const data_t * ptr)
      {
        return f(ptr[0], ptr[1]);
      }
    };
  }

  template <int size, typename data_t, typename func>
  constexpr data_t reduce(func f, const data_t * ptr)
  {
    if constexpr (size == 1)
    {
      return *ptr;
    }
    else
    {
      return ggo::details::reduce_t<data_t, size>::call(f, ptr);
    }
  }

  template <int size, typename data_t, typename func>
  constexpr data_t reduce(func f, data_t const (&ptr)[size])
  {
    if constexpr (size == 1)
    {
      return *ptr;
    }
    else
    {
      return ggo::details::reduce_t<data_t, size>::call(f, ptr);
    }
  }

  // Cast input parameters to the given output type.
  template <typename output_t, typename func, typename data_t>
  constexpr output_t reduce_to(func f, data_t v1, data_t v2)
  {
    return f(static_cast<output_t>(v1), static_cast<output_t>(v2));
  }

  template <typename output_t, typename func, typename data_t, typename... args>
  constexpr output_t reduce_to(func f, data_t v, args... a)
  {
    return f(static_cast<output_t>(v), ggo::reduce_to<output_t>(f, a...));
  }
}

// Min, max.
namespace ggo
{
  template <typename data_t, typename... args>
  constexpr data_t min(const data_t & v, args... a)
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return std::min(v1, v2); }, v, a...);
  }

  template <typename data_t, typename... args>
  constexpr data_t max(const data_t & v, args... a)
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return std::max(v1, v2); }, v, a...);
  }

  template <int size, typename data_t>
  constexpr data_t min(data_t const (&ptr)[size])
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return std::min(v1, v2); }, ptr);
  }

  template <int size, typename data_t>
  constexpr data_t max(data_t const (&ptr)[size])
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return std::max(v1, v2); }, ptr);
  }
}

// Mul.
namespace ggo
{
  template <typename data_t>
  constexpr data_t mul(const data_t & v)
  {
    return v;
  }

  template <typename data_t, typename... args>
  constexpr data_t mul(const data_t & v, args... a)
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return v1 * v2; }, v, a...);
  }

  template <int size, typename data_t>
  constexpr data_t mul(const data_t * ptr)
  {
    return ggo::reduce<size>([](const data_t & v1, const data_t & v2) constexpr { return v1 * v2; }, ptr);
  }

  template <int size, typename data_t>
  constexpr data_t mul(data_t const (&ptr)[size])
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return v1 * v2; }, ptr);
  }
}

// Sum.
namespace ggo
{
  template <typename data_t, typename... args>
  constexpr data_t sum(const data_t & v, args... a)
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return v1 + v2; }, v, a...);
  }

  template <typename output_t, typename data_t, typename... args>
  constexpr output_t sum_to(data_t v, args... a)
  {
    return ggo::reduce_to<output_t>([](output_t v1, output_t v2) constexpr { return v1 + v2; }, v, a...);
  }

  template <int size, typename data_t>
  constexpr data_t sum(data_t const (&ptr)[size])
  {
    return ggo::reduce([](const data_t & v1, const data_t & v2) constexpr { return v1 + v2; }, ptr);
  }
}

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

#endif

