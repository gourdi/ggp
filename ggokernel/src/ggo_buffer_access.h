#ifndef __GGO_BUFFER_ACCESS__
#define __GGO_BUFFER_ACCESS__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  void set1d(data_t * buffer, int x, int width, const data_t & v)
  {
    GGO_ASSERT(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    buffer[stride * x] = v;
  }
}

//////////////////////////////////////////////////////////////
// 1D - getters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t & get1d_ref(data_t * buffer, int x, int width)
  {
    GGO_ASSERT(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    return buffer[stride * x];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t get1d_mirror(const data_t * buffer, int x, int width)
  {
    GGO_ASSERT(buffer);
    x = mirror_index_edge_duplicated(x, width);
    return buffer[stride * x];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t & get1d_mirror_ref(data_t * buffer, int x, int width)
  {
    GGO_ASSERT(buffer);
    x = mirror_index_edge_duplicated(x, width);
    return buffer[stride * x];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  const data_t & get1d_fixed_value(const data_t * buffer, int x, int width, const data_t & fixed_value)
  {
    if (x < 0 || x >= width) { return fixed_value; }

    GGO_ASSERT(buffer);
    return buffer[stride * x];
  }
}

//////////////////////////////////////////////////////////////
// 2D - setters
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  void set2d(data_t * buffer, int x, int y, int width, int height, const data_t & data)
  {
    GGO_ASSERT(buffer);
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
  template <typename data_t, int stride = 1>
  data_t get2d(const data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    GGO_ASSERT_BTW(y, 0, height - 1);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t & get2d_ref(data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    GGO_ASSERT_BTW(x, 0, width - 1);
    GGO_ASSERT_BTW(y, 0, height - 1);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t get2d_loop(const data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    x = loop_index(x, width);
    y = loop_index(y, height);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t & get2d_loop_ref(data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    x = loop_index(x, width);
    y = loop_index(y, height);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t get2d_mirror(const data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t & get2d_mirror_ref(data_t * buffer, int x, int y, int width, int height)
  {
    GGO_ASSERT(buffer);
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  const data_t & get2d_fixed_value(const data_t * buffer, int x, int y, int width, int height, const data_t & fixed_value)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }

    GGO_ASSERT(buffer);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, int stride = 1>
  data_t get2d_zero(const data_t * buffer, int x, int y, int width, int height)
  {
    if (x < 0 || x >= width) { return 0; }
    if (y < 0 || y >= height) { return 0; }

    GGO_ASSERT(buffer);
    return buffer[stride * (y * width + x)];
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, data_t fixed_value, int stride = 1>
  data_t get2d_fixed_value_t(const data_t * buffer, int x, int y, int width, int height)
  {
    if (x < 0 || x >= width) { return fixed_value; }
    if (y < 0 || y >= height) { return fixed_value; }

    GGO_ASSERT(buffer);
    return buffer[stride * (y * width + x)];
  }
}

#endif
