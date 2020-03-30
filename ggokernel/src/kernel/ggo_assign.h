#ifndef __GGO_ASSIGN__
#define __GGO_ASSIGN__

// Variadic.
namespace ggo
{
  template <typename data_t, typename... args>
  constexpr void assign(data_t * coefs, data_t k, args... a)
  {
    *coefs = k;

    if constexpr (sizeof...(a) > 0)
    {
      ggo::assign(coefs + 1, a...);
    }
  }

  // Variant to force the number of coefs to assign.
  template <int count, typename data_t, typename... args>
  constexpr void assign(data_t * coefs, data_t k, args... a)
  {
    *coefs = k;

    if constexpr (count > 1)
    {
      ggo::assign<count - 1>(coefs + 1, a...);
    }
  }
}

// Buffers
namespace ggo
{
  namespace details
  {
    template <int size, typename data_t>
    constexpr void assign_t(data_t * dst, const data_t * src)
    {
      *dst = *src;
      if constexpr (size > 1)
      {
        ggo::details::assign_t<size - 1>(dst + 1, src + 1);
      }
    }
  }

  template <int size, typename data_t>
  constexpr void assign(data_t * dst, const data_t * src)
  {
    ggo::details::assign_t<size, data_t>(dst, src);
  }

  template <int size, typename data_t>
  constexpr void assign(data_t (&dst)[size], data_t const (&src)[size])
  {
    ggo::details::assign_t<size, data_t>(dst, src);
  }
}

#endif
