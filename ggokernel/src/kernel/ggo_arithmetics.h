#ifndef __GGO_ARITHMETICS__
#define __GGO_ARITHMETICS__

// In-place operations.
namespace ggo
{
  namespace details
  {
    template <typename data_t, int count, int index>
    struct inplace_t
    {
      static void mul(data_t * coefs, const data_t * coefs2)
      {
        inplace_t<data_t, count, index - 1>::mul(coefs, coefs2);
        coefs[index] *= coefs2[index];
      }

      static void div(data_t * coefs, const data_t * coefs2)
      {
        inplace_t<data_t, count, index - 1>::div(coefs, coefs2);
        coefs[index] /= coefs2[index];
      }

      static void add(data_t * coefs, const data_t * coefs2)
      {
        inplace_t<data_t, count, index - 1>::add(coefs, coefs2);
        coefs[index] += coefs2[index];
      }

      static void sub(data_t * coefs, const data_t * coefs2)
      {
        inplace_t<data_t, count, index - 1>::sub(coefs, coefs2);
        coefs[index] -= coefs2[index];
      }

      static void mul(data_t * coefs, data_t k)
      {
        inplace_t<data_t, count, index - 1>::mul(coefs, k);
        coefs[index] *= k;
      }

      static void div(data_t * coefs, data_t k)
      {
        inplace_t<data_t, count, index - 1>::div(coefs, k);
        coefs[index] /= k;
      }

      static void set(data_t * coefs, const data_t * coefs2)
      {
        inplace_t<data_t, count, index - 1>::set(coefs, coefs2);
        coefs[index] = coefs2[index];
      }
    };

    template <typename data_t, int count>
    struct inplace_t<data_t, count, 0>
    {
      static void mul(data_t * coefs, const data_t * coefs2)
      {
        coefs[0] *= coefs2[0];
      }

      static void div(data_t * coefs, const data_t * coefs2)
      {
        coefs[0] /= coefs2[0];
      }

      static void add(data_t * coefs, const data_t * coefs2)
      {
        coefs[0] += coefs2[0];
      }

      static void sub(data_t * coefs, const data_t * coefs2)
      {
        coefs[0] -= coefs2[0];
      }

      static void mul(data_t * coefs, data_t k)
      {
        coefs[0] *= k;
      }

      static void div(data_t * coefs, data_t k)
      {
        coefs[0] /= k;
      }

      static void set(data_t * coefs, const data_t * coefs2)
      {
        coefs[0] = coefs2[0];
      }
    };
  }

  template <int count, typename data_t>
  void mul(data_t * coefs, const data_t * coefs2)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::mul(coefs, coefs2);
  }

  template <int size, typename data_t>
  void mul(data_t (&coefs)[size], data_t const (&coefs2)[size])
  {
    ggo::details::inplace_t<data_t, size, size - 1>::mul(coefs, coefs2);
  }

  template <int count, typename data_t>
  void div(data_t * coefs, const data_t * coefs2)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::div(coefs, coefs2);
  }

  template <int size, typename data_t>
  void div(data_t (&coefs)[size], data_t const (&coefs2)[size])
  {
    ggo::details::inplace_t<data_t, size, size - 1>::div(coefs, coefs2);
  }

  template <int count, typename data_t>
  void add(data_t * coefs, const data_t * coefs2)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::add(coefs, coefs2);
  }

  template <int size, typename data_t>
  void add(data_t (&coefs)[size], data_t const (&coefs2)[size])
  {
    ggo::details::inplace_t<data_t, size, size - 1>::add(coefs, coefs2);
  }

  template <int count, typename data_t>
  void sub(data_t * coefs, const data_t * coefs2)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::sub(coefs, coefs2);
  }

  template <int size, typename data_t>
  void sub(data_t(&coefs)[size], data_t const (&coefs2)[size])
  {
    ggo::details::inplace_t<data_t, size, size - 1>::sub(coefs, coefs2);
  }

  template <int count, typename data_t>
  void mul(data_t * coefs, data_t k)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::mul(coefs, k);
  }

  template <int count, typename data_t>
  void div(data_t * coefs, data_t k)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::div(coefs, k);
  }

  template <int count, typename data_t>
  void set(data_t * coefs, const data_t * coefs2)
  {
    ggo::details::inplace_t<data_t, count, count - 1>::set(coefs, coefs2);
  }
}

// Dot product.
namespace ggo
{
  template <typename data_t, int count>
  struct dot_t
  {
    constexpr static data_t dot(const data_t * ptr1, const data_t * ptr2)
    {
      return ptr1[0] * ptr2[0] + ggo::dot_t<data_t, count - 1>::dot(ptr1 + 1, ptr2 + 1);
    }
  };

  template <typename data_t>
  struct dot_t<data_t, 1>
  {
    constexpr static data_t dot(const data_t * ptr1, const data_t * ptr2)
    {
      return ptr1[0] * ptr2[0];
    }
  };

  template <int size, typename data_t>
  constexpr data_t dot(const data_t * ptr1, const data_t * ptr2)
  {
    return ggo::dot_t<data_t, size>::dot(ptr1, ptr2);
  }

  template <int size, typename data_t>
  constexpr data_t dot(data_t const (&ptr1)[size], data_t const (&ptr2)[size])
  {
    return ggo::dot_t<data_t, size>::dot(ptr1, ptr2);
  }
}

#endif
