#ifndef __GGO_CONVERT__
#define __GGO_CONVERT__

namespace ggo
{
  template <typename data_t>
  struct passthrough_conversion
  {
    using input_t = data_t;
    using output_t = data_t;

    static const data_t & convert(const data_t & v) { return v; }
  };
}

#endif
