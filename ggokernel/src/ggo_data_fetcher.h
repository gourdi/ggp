#ifndef __GGO_DATA_FETCHER__
#define __GGO_DATA_FETCHER__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  template <typename data_type>
  data_type & fetch_data_standard1d(data_type * data, int size, int stride, int x)
  {
    GGO_ASSERT(x >= 0 && x < size);
    return data[stride * x];
  }

  template <typename data_type>
  data_type fetch_data_standard1d_const(const data_type * data, int size, int stride, int x)
  {
    GGO_ASSERT(x >= 0 && x < size);
    return data[stride * x];
  }
  
  template <typename data_type>
  data_type & fetch_data_duplicated_edge_mirror1d(data_type * data, int size, int stride, int x)
  {
    int index = mirror_index_edge_duplicated(x, size);
    return data[stride * index];
  }

  template <typename data_type>
  data_type fetch_data_duplicated_edge_mirror1d_const(const data_type * data, int size, int stride, int x)
  {
    int index = mirror_index_edge_duplicated(x, size);
    return data[stride * index];
  }
    
  template <typename data_type>
  struct fetch_data_duplicated_edge_mirror1d_const_struct
  {
    data_type operator()(const data_type * data, int size, int stride, int x)
    {
      return fetch_data_duplicated_edge_mirror1d_const(data, size, stride, x);
    }
  };
}

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  template <typename data_type>
  data_type & fetch_data_standard2d(data_type * data, int width, int height, int stride, int x, int y)
  {
    GGO_ASSERT(x >= 0 && x < width);
    GGO_ASSERT(y >= 0 && y < height);
    return data[stride * (y * width + x)];
  }

  template <typename data_type>
  data_type fetch_data_standard2d_const(const data_type * data, int width, int height, int stride, int x, int y)
  {
    GGO_ASSERT(x >= 0 && x < width);
    GGO_ASSERT(y >= 0 && y < height);
    return data[stride * (y * width + x)];
  }

  template <typename data_type, data_type fixed_value = 0>
  data_type fetch_data_fixed2d(const data_type * data, int width, int height, int stride, int x, int y)
  {
    return x < 0 || x >= width || y < 0 || y >= height ? fixed_value : data[stride * (y * width + x)];
  }

  template <typename data_type>
  data_type & fetch_data_duplicated_edge_mirror2d(data_type * data, int width, int height, int stride, int x, int y)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return data[stride * (y * width + x)];
  }

  template <typename data_type>
  data_type fetch_data_duplicated_edge_mirror2d_const(const data_type * data, int width, int height, int stride, int x, int y)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return data[stride * (y * width + x)];
  }
}

#endif
