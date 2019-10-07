#ifndef __GGO_SCALE1D__
#define __GGO_SCALE1D__

#include <kernel/ggo_kernel.h>

namespace ggo
{
  enum class scaling_algo
  {
    nearest_neighbor,
    linear_integration,
    cubic_integration
  };

  //////////////////////////////////////////////////////////////
  inline int nearest_neighbor_index(int i, int size_in, int size_out)
  {
    return i * size_in / size_out + (size_in / size_out) / 2;
  }

  //////////////////////////////////////////////////////////////
  // Here what we do is building a cubic curve which value at x=0 is
  // the one is input[i] and which value at x=1 is input[i+1].
  // Furthermore, to have a smooth global curve, we also make sure
  // that that the derivatuve of the cubic at x=0 is the slope
  // between the points at input[i-1] and input[i+1], and that the
  // derivative at x=1 is the slope between the points at input[i]
  // and input[i+2].
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename real_t, typename input>
  data_t integrate_cubic_single(input in, int zero_offset, real_t from, real_t to)
  {
    static_assert(std::is_floating_point<real_t>::value);
    GGO_ASSERT(from >= zero_offset);
    GGO_ASSERT(to <= zero_offset + 1);
    GGO_ASSERT(from <= to);

    // Compute points coordinates.
    int i_p = zero_offset - 1;
    int i_c = zero_offset;
    int i_n = zero_offset + 1;
    int i_nn = zero_offset + 2;

    // Retrieve the values of the cubic and its derivatives.
    data_t y0 = in(i_c);	// Value at x=0.
    data_t y1 = in(i_n);	// Value at x=1.
    data_t d0 = (in(i_n) - in(i_p)) / real_t(2); // Derative at x=0.
    data_t d1 = (in(i_nn) - in(i_c)) / real_t(2); // Derative at x=1.

    // Now we have a linear system with 4 equations:
    // y0 = d;
    // y1 = a+b+c+d
    // d0 = c
    // d1 = 3*a+2*c+b
    // So we can compute the cubic's coefs.
    data_t a = real_t(2) * y0 - real_t(2) * y1 + d0 + d1;
    data_t b = real_t(-3) * y0 + real_t(3) * y1 - real_t(2) * d0 - d1;
    data_t c = d0;
    data_t d = y0;

    auto eval_cubic_integrale = [](data_t a, data_t b, data_t c, data_t d, real_t x)
    {
      real_t xx = x * x;
      return a * xx * xx / real_t(4) + b * xx * x / real_t(3) + c * xx / real_t(2) + d * x;
    };

    // Finally, we can integrate.
    return eval_cubic_integrale(a, b, c, d, to - zero_offset) - eval_cubic_integrale(a, b, c, d, from - zero_offset);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename real_t, typename input>
  data_t integrate_cubic(input in, int size_in, real_t from, real_t to)
  {
    GGO_ASSERT(from < to);

    data_t val(0);
    int from_i = std::max(0, static_cast<int>(from));
    int to_i = std::min(size_in - 1, static_cast<int>(to + 1.0));
    GGO_ASSERT(from_i < to_i);

    // Integrate the middle parts.
    for (int i = from_i; i < to_i; ++i)
    {
      val += integrate_cubic_single<data_t>(in, i, real_t(i), real_t(i + 1));
    }

    // Remove first block.
    val -= integrate_cubic_single<data_t>(in, from_i, real_t(from_i), from);

    // Remove last block.
    val -= integrate_cubic_single<data_t>(in, to_i - 1, to, real_t(to_i));

    return val;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename real_t, typename input>
  data_t integrate_linear(input in, int size_in, real_t from, real_t to)
  {
    GGO_ASSERT(from < to);

    data_t val = 0;
    int from_i = std::max(0, static_cast<int>(from));
    int to_i = std::min(size_in - 1, static_cast<int>(to + 1.0));
    GGO_ASSERT(from_i < to_i);

    // Integrate the middle parts.
    for (int i = from_i; i < to_i; ++i)
    {
      val += in(i) + in(i + 1);
    }
    val /= 2;

    // Remove first block.
    data_t a = in(from_i + 1) - in(from_i);
    data_t b = in(from_i) - a * static_cast<real_t>(from_i);
    val -= a * (from * from - from_i * from_i) / static_cast<real_t>(2) + b * (from - from_i);

    // Remove last block.
    a = in(to_i) - in(to_i - 1);
    b = in(to_i - 1) - a * static_cast<real_t>(to_i - 1);
    val -= a * (to_i * to_i - to * to) / static_cast<real_t>(2) + b * (to_i - to);

    return val;
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename data_t, typename real_t, typename input, typename output>
  void scale_1d(input in, int size_in, output out, int size_out)
  {
    static_assert(std::is_floating_point<real_t>::value == true);

    real_t ratio = static_cast<real_t>(size_in - 1) / static_cast<real_t>(size_out);
    real_t inv_ratio = 1 / ratio;

    for (int i = 0; i < size_out; ++i)
    {
      if constexpr(algo == scaling_algo::nearest_neighbor)
      {
        int i_in = nearest_neighbor_index(i, size_in, size_out);
        out(i, in(i_in));
      }
      else
      {
        static_assert(std::is_floating_point<real_t>::value == true);

        real_t from = static_cast<real_t>(i) * ratio;
        real_t to = static_cast<real_t>(i + 1) * ratio;

        if constexpr(algo == scaling_algo::linear_integration)
        {
          out(i, inv_ratio * integrate_linear<data_t, real_t>(in, size_in, from, to));
        }
        else if constexpr(algo == scaling_algo::cubic_integration)
        {
          out(i, inv_ratio * integrate_cubic<data_t, real_t>(in, size_in, from, to));
        }
        else
        {
          static_assert(false);
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename data_t, typename int_t>
  void	scale_1d(const data_t * input, int_t size_in, data_t * output, int_t size_out)
  {
    static_assert(std::is_integral<int_t>::value == true);

    auto in = [&](int i)
    {
      if constexpr(algo == scaling_algo::cubic_integration)
      {
        i = ggo::clamp<int>(i, 0, int(size_in) - 1);
      }

      return input[i];
    };
    
    auto out = [&](int i, data_t v)
    {
      output[i] = v;
    };

    scale_1d<algo, data_t, data_t>(in, static_cast<int>(size_in), out, static_cast<int>(size_out));
  }
}

#endif
