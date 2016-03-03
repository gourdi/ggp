#ifndef __GGO_CONVOLUTION1D__
#define __GGO_CONVOLUTION1D__

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Apply 1D symetric filter.
  template <typename input_type,
            typename filter_type,
            typename data_type,
            typename output_type,
            input_type(fetch_func)(const input_type *, int, int, int)>
  void apply_symetric_filter_1d(const input_type * in,
                                output_type * out,
                                int size,
                                int stride_in,
                                int stride_out,
                                const filter_type * filter,
                                int filter_size)
  {
    for (int x = 0; x < size; ++x)
    {
      data_type v = ggo::to<data_type>(in[x * stride_in]) * filter[0];
      for (int i = 1; i < filter_size; ++i)
      {
        int i1 = x + i;
        int i2 = x - i;
        data_type tmp1 = ggo::to<data_type>(fetch_func(in, size, stride_in, i1));
        data_type tmp2 = ggo::to<data_type>(fetch_func(in, size, stride_in, i2));
        v += (tmp1 + tmp2) * filter[i];
      }

      *out = ggo::to<output_type>(v);

      out += stride_out;
    }
  }
}

#endif
