namespace ggo
{
  enum class scaling_algo
  {
    nearest_neighbor,
    linear_integration,
    cubic_integration
  };

  //////////////////////////////////////////////////////////////
  // Here what we do is building a cubic curve which value at x=0 is
  // the one is input[i] and which value at x=1 is input[i+1].
  // Furthermore, to have a smooth global curve, we also make sure
  // that that the derivatuve of the cubic at x=0 is the slope
  // between the points at input[i-1] and input[i+1], and that the
  // derivative at x=1 is the slope between the points at input[i]
  // and input[i+2].
  //////////////////////////////////////////////////////////////
  template <typename real_t, typename input>
  real_t integrate_cubic_single(input in, int zero_offset, real_t from, real_t to)
  {
    GGO_ASSERT(from >= zero_offset);
    GGO_ASSERT(to <= zero_offset + 1);
    GGO_ASSERT(from <= to);

    // Compute points coordinates.
    int i_p = zero_offset - 1;
    int i_c = zero_offset;
    int i_n = zero_offset + 1;
    int i_nn = zero_offset + 2;

    // Retrieve the values of the cubic and its derivatives.
    real_t y0 = in(i_c);	// Value at x=0
    real_t y1 = in(i_n);	// Value at x=1.
    real_t d0 = (in(i_n) - in(i_p)) / 2.0; // Derative at x=0.
    real_t d1 = (in(i_nn) - in(i_c)) / 2.0; // Derative at x=1.

    // Now we have a linear system with 4 equations:
    // y0 = d;
    // y1 = a+b+c+d
    // d0 = c
    // d1 = 3*a+2*c+b
    // So we can compute the cubic's coefs.
    real_t a = 2 * y0 - 2 * y1 + d0 + d1;
    real_t b = -3 * y0 + 3 * y1 - 2 * d0 - d1;
    real_t c = d0;
    real_t d = y0;

    auto eval_cubic_integrale = [](real_t a, real_t b, real_t c, real_t d, real_t x)
    {
      real_t xx = x * x;
      return a * xx * xx / 4 + b * xx * x / 3 + c * xx / 2 + d * x;
    };

    // Finally, we can integrate.
    return eval_cubic_integrale(a, b, c, d, to - zero_offset) - eval_cubic_integrale(a, b, c, d, from - zero_offset);
  }

  //////////////////////////////////////////////////////////////
  template <typename real_t, typename input>
  real_t integrate_cubic(input in, real_t from, real_t to)
  {
    GGO_ASSERT(from < to);

    real_t val = 0;
    int from_i = static_cast<int>(from);
    int to_i = static_cast<int>(to + 1.0);
    GGO_ASSERT(from_i < to_i);

    // Integrate the middle parts.
    for (int i = from_i; i < to_i; ++i)
    {
      val += integrate_cubic_single(in, i, i, i + 1);
    }

    // Remove first block.
    val -= integrate_cubic_single(in, from_i, from_i, from);

    // Remove last block.
    val -= integrate_cubic_single(in, to_i - 1, to, to_i);

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
    data_t b = in(from_i) - a * from_i;
    val -= a * (from * from - from_i * from_i) / 2 + b * (from - from_i);

    // Remove last block.
    a = in(to_i) - in(to_i - 1);
    b = in(to_i - 1) - a * (to_i - 1);
    val -= a * (to_i * to_i - to * to) / 2 + b * (to_i - to);

    return val;
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename data_t, typename real_t, typename input, typename output>
  void	scale_1d(input in, int size_in, output out, int size_out)
  {
    static_assert(std::is_floating_point<real_t>::value == true);

    real_t ratio = static_cast<real_t>(size_in - 1) / static_cast<real_t>(size_out);
    real_t inv_ratio = 1 / ratio;

    for (int i = 0; i < size_out; ++i)
    {
      if constexpr(algo == scaling_algo::nearest_neighbor)
      {
        int i_in = i * size_in / size_out + (size_in / size_out) / 2;
        out(i, in(i_in));
      }
      else
      {
        real_t from = static_cast<real_t>(i) * ratio;
        real_t to = static_cast<real_t>(i + 1) * ratio;

        if constexpr(algo == scaling_algo::linear_integration)
        {
          out(i, inv_ratio * integrate_linear<data_t, real_t>(in, size_in, from, to));
        }
        else if constexpr(algo == scaling_algo::cubic_integration)
        {
          out(i, inv_ratio * integrate_cubic<data_t, real_t>(in, from, to));
        }
        else
        {
          static_assert(false);
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <scaling_algo algo, typename real_t, typename size_t>
  void	scale_1d(const real_t * input, size_t size_in, real_t * output, size_t size_out)
  {
    static_assert(std::is_floating_point<real_t>::value == true);
    static_assert(std::is_integral<size_t>::value == true);

    auto in = [&](int i) { return input[i]; };
    auto out = [&](int i, real_t v) { output[i] = v; };

    scale_1d<algo, real_t, real_t>(in, static_cast<int>(size_in), out, static_cast<int>(size_out));
  }

  /*
  //////////////////////////////////////////////////////////////
  template <typename T>
  void	scale_fast_t(const T * input, int width_in, int height_in, T * output, int width_out, int height_out)
  {
    int	x, y, x_in, y_in;

    for (y = 0; y < height_out; ++y)
    {
      for (x = 0; x < width_out; ++x)
      {
        x_in = x * width_in / width_out;
        y_in = y * height_in / height_out;

        output[y * width_out + x] = input[y_in * width_in + x_in];
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void	ggo_scale2d_t(const T * input, int width_in, int height_in, T * output, int width_out, int height_out, ggo_resample_algo algo, T clamp_min, T clamp_max)
  {
    int	x, y;
    T *	tmp;

    if (algo == GGO_RESAMPLE_ALGO_FAST)
    {
      scale_fast_t(input, width_in, height_in, output, width_out, height_out);
      return;
    }

    tmp = new T[width_in*height_out];

    // First perform vertical scaling.
    for (x = 0; x < width_in; ++x)
    {
      scale_1d_t(input + x, height_in, width_in, tmp + x, height_out, width_in, algo, clamp_min, clamp_max);
    }

    // Then perform the horizontal scaling.
    for (y = 0; y < height_out; ++y)
    {
      scale_1d_t(tmp + y * width_in, width_in, 1, output + y * width_out, width_out, 1, algo, clamp_min, clamp_max);
    }

    delete[] tmp;
  }

  //////////////////////////////////////////////////////////////
  void	ggo_scale_2d(const uint8_t * input, int width_in, int height_in, uint8_t * output, int width_out, int height_out, ggo_resample_algo algo)
  {
    ggo_scale2d_t<uint8_t>(input, width_in, height_in, output, width_out, height_out, algo, 0, 255);
  }*/
}