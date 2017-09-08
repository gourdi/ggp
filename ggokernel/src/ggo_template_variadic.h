#ifndef __GGO_TEMPLATE_VARIADIC__
#define __GGO_TEMPLATE_VARIADIC__

namespace ggo
{
  // Given a binary lambda f ans a set of variadic parameters (a, b, c, ..., y, z),
  // returns f(a, f(b, f(c, f(..., f(y, z)...))),
  // assuming the type of return value of f is the same of the input parameters.
  template <typename func, typename data_t>
  constexpr data_t accumulate(func f, const data_t & v1, const data_t & v2)
  {
    return f(v1, v2);
  }

  template <typename func, typename data_t, typename... args>
  constexpr data_t accumulate(func f, const data_t & v, args... a)
  {
    return f(v, ggo::accumulate(f, a...));
  }

  // Same as accumulate, but cast input parameters to the given output type.
  template <typename output_t, typename func, typename data_t>
  constexpr output_t accumulate_to(func f, data_t v1, data_t v2)
  {
    return f(static_cast<output_t>(v1), static_cast<output_t>(v2));
  }

  template <typename output_t, typename func, typename data_t, typename... args>
  constexpr output_t accumulate_to(func f, data_t v, args... a)
  {
    return f(static_cast<output_t>(v), ggo::accumulate_to<output_t>(f, a...));
  }

  // Min, max.
  template <typename data_t, typename... args>
  constexpr data_t min(const data_t & v, args... a)
  {
    return ggo::accumulate([](const data_t & v1, const data_t & v2) { return std::min(v1, v2); }, v, a...);
  }

  template <typename data_t, typename... args>
  constexpr data_t max(const data_t & v, args... a)
  {
    return ggo::accumulate([](const data_t & v1, const data_t & v2) { return std::max(v1, v2); }, v, a...);
  }

  // Sum, average.
  template <typename data_t, typename... args>
  constexpr data_t sum(const data_t & v, args... a)
  {
    return ggo::accumulate([](const data_t & v1, const data_t & v2) { return v1 + v2; }, v, a...);
  }

  template <typename output_t, typename data_t, typename... args>
  constexpr output_t sum_to(data_t v, args... a)
  {
    return ggo::accumulate_to<output_t>([](output_t v1, output_t v2) { return v1 + v2; }, v, a...);
  }

  template <typename data_t, typename... args>
  constexpr data_t average(data_t v, args... a)
  {
    if constexpr(std::is_integral<data_t>::value == true)
    {
      if constexpr(std::is_unsigned<data_t>::value == true)
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

  // Add coefs to a buffer.
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

  // Set a buffer with coefs.
  template <typename data_t>
  void set(data_t * ptr, const data_t & v)
  {
    ptr[0] = v;
  }

  template <typename data_t, typename... args>
  void set(data_t * ptr, const data_t & v, args... a)
  {
    ptr[0] = v;
    ggo::set<data_t>(ptr + 1, a...);
  }
}

#endif
