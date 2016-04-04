#ifndef __GGO_DATA_FETCHER__
#define __GGO_DATA_FETCHER__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type>
  void set1d_standard(data_type * buffer, int x, int width, const data_type & v, int stride = 1)
  {
    GGO_ASSERT_BTW(x, 0, width - 1);
    buffer[stride * x] = v;
  }
}

//////////////////////////////////////////////////////////////
// 1D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get1d_standard(const data_type * buffer, int x, int width, int stride = 1)
  {
    GGO_ASSERT_BTW(x, 0, width - 1);
    return buffer[stride * x];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get1d_duplicated_edge_mirror(const data_type * buffer, int x, int width, int stride = 1)
  {
    x = mirror_index_edge_duplicated(x, width);
    return buffer[stride * x];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get1d_fixed_value(const data_type * buffer, int x, int width, const data_type & fixed_value, int stride = 1)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    return buffer[stride * x];
  }
}

//////////////////////////////////////////////////////////////
// 2D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type>
  void set2d_standard(data_type * buffer, int x, int y, int width, int height, const data_type & data, int stride = 1)
  {
    GGO_ASSERT_BTW(x, 0, width - 1);
    GGO_ASSERT_BTW(y, 0, height - 1);
    buffer[stride * (y * width + x)] = data;
  }
}

//////////////////////////////////////////////////////////////
// 2D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get2d_standard(const data_type * buffer, int x, int y, int width, int height, int stride = 1)
  {
    GGO_ASSERT_BTW(x, 0, width - 1);
    GGO_ASSERT_BTW(y, 0, height - 1);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get2d_duplicated_edge_mirror(const data_type * buffer, int x, int y, int width, int height, int stride = 1)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type>
  const data_type & get2d_fixed_value(const data_type * buffer, int x, int y, int width, int height, const data_type & fixed_value, int stride = 1)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }
    return buffer[stride * (y * width + x)];
  }
}

#endif
