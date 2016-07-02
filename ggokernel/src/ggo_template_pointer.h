#ifndef __GGO_TEMPLATE_POINTER__
#define __GGO_TEMPLATE_POINTER__

namespace ggo
{
  // Accumulate.
  template <typename data_t, typename func, int remaining>
  struct accumulate_t
  {
    static data_t accumulate(const data_t * ptr, func f)
    {
      return f(ggo::accumulate_t<data_t, func, remaining - 1>::accumulate(ptr, f), ptr[remaining - 1]);
    }
  };

  template <typename data_t, typename func>
  struct accumulate_t<data_t, func, 1>
  {
    static data_t accumulate(const data_t * ptr, func f)
    {
      return ptr[0];
    }
  };

  template <int count, typename data_t, typename func>
  data_t accumulate(const data_t * ptr, func f)
  {
    return ggo::accumulate_t<data_t, func, count>::accumulate(ptr, f);
  }

  // Multiply all.
  template <int count, typename data_t>
  data_t multiply_all(const data_t * ptr)
  {
    return ggo::accumulate<count>(ptr, [](data_t v1, data_t v2) { return v1 * v2; });
  }

  // For each: a single pointer.
  template <typename data_t, typename func, int count>
  struct for_each_t
  {
    static void for_each(data_t * ptr, func f)
    {
      f(ptr[0]);
      ggo::for_each_t<data_t, func, count - 1>::for_each(ptr + 1, f);
    }
  };

  template <typename data_t, typename func>
  struct for_each_t<data_t, func, 1>
  {
    static void for_each(data_t * ptr, func f)
    {
      f(ptr[0]);
    }
  };

  template <int count, typename data_t, typename func>
  void for_each(data_t * ptr, func f)
  {
    ggo::for_each_t<data_t, func, count>::for_each(ptr, f);
  }

  // Mul.
  template <int count, typename data_t>
  void mul(data_t * ptr, data_t k)
  {
    ggo::for_each<count>(ptr, [&](data_t & v) { v *= k; });
  }

  // Unary operation: a destination pointer and a source pointer.
  template <typename data_t, typename func, int index, int count>
  struct unary_operation_t
  {
    static void process(data_t * dst, const data_t * src, func f)
    {
      f(dst[index], src[index]);
      ggo::unary_operation_t<data_t, func, index + 1, count>::process(dst, src, f);
    }
  };

  template <typename data_t, typename func, int count>
  struct unary_operation_t<data_t, func, count, count>
  {
    static void process(data_t * dst, const data_t * src, func f) { }
  };

  template <int count, typename data_t, typename func>
  void unary_operation(data_t * dst, const data_t * src, func f)
  {
    unary_operation_t<data_t, func, 0, count>::process(dst, src, f);
  }

  // Copy a buffer.
  template <int count, typename data_t>
  void copy(data_t * dst, const data_t * src)
  {
    ggo::unary_operation<count>(dst, src, [](data_t & dst, const data_t & src) { dst = src; });
  }

  // Binary operation: a destination pointer and 2 source pointers.
  template <typename data_t, typename func, int index, int count>
  struct binary_operation_t
  {
    static void process(data_t * dst, const data_t * src1, const data_t * src2, func f)
    {
      f(dst[index], src1[index], src2[index]);
      ggo::binary_operation_t<data_t, func, index + 1, count>::process(dst, src1, src2, f);
    }
  };

  template <typename data_t, typename func, int count>
  struct binary_operation_t<data_t, func, count, count>
  {
    static void process(data_t * dst, const data_t * src1, const data_t * src2, func f) { }
  };

  template <int count, typename data_t, typename func>
  void binary_operation(data_t * dst, const data_t * src1, const data_t * src2, func f)
  {
    binary_operation_t<data_t, func, 0, count>::process(dst, src1, src2, f);
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

#endif
