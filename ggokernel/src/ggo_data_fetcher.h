#ifndef __GGO_DATA_FETCHER__
#define __GGO_DATA_FETCHER__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  template <typename T>
  T & fetch_data_standard1d(T * data, int size, int stride, int x)
  {
    return data[stride * x];
  }

  template <typename T>
  T fetch_data_standard1d_const(const T * data, int size, int stride, int x)
  {
    return data[stride * x];
  }

  template <typename T>
  T & fetch_data_duplicated_edge_mirror1d(T * data, int size, int stride, int x)
  {
    int index = mirror_index_edge_duplicated(x, size);
    return data[stride * index];
  }

  template <typename T>
  T fetch_data_duplicated_edge_mirror1d_const(const T * data, int size, int stride, int x)
  {
    int index = mirror_index_edge_duplicated(x, size);
    return data[stride * index];
  }
}

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  template <typename T>
  T & fetch_data_standard2d(T * data, int width, int height, int stride, int x, int y)
  {
    return data[stride * (y * width + x)];
  }

  template <typename T>
  T fetch_data_standard2d_const(const T * data, int width, int height, int stride, int x, int y)
  {
    return data[stride * (y * width + x)];
  }

  template <typename T>
  T & fetch_data_duplicated_edge_mirror2d(T * data, int width, int height, int stride, int x, int y)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return data[stride * (y * width + x)];
  }

  template <typename T>
  T fetch_data_duplicated_edge_mirror2d_const(const T * data, int width, int height, int stride, int x, int y)
  {
    x = mirror_index_edge_duplicated(x, width);
    y = mirror_index_edge_duplicated(y, height);
    return data[stride * (y * width + x)];
  }
}

#endif
