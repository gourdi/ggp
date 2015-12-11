#ifndef __GGO_DATA_FETCHER__
#define __GGO_DATA_FETCHER__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class data_fetcher1d_abc
  {
  public:
  
    virtual const T & fetch(const T * data, int size, int stride, int x) const = 0;
  };
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  class fixed_value_data_fetcher1d : public data_fetcher1d_abc<T>
  {
  public:
  
    fixed_value_data_fetcher1d(const T & fixed_value) : _fixed_value(fixed_value) {}
  
    const T & fetch(const T * data, int size, int stride, int x) const override
    {
      return (x < 0) || (x >= size) ? _fixed_value : data[stride * x];
    }
    
  private:
    
    T _fixed_value;
  };
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  class duplicated_edge_mirror_data_fetcher1d : public data_fetcher1d_abc<T>
  {
  public:

    const T & fetch(const T * data, int size, int stride, int x) const override
    {
      int index = mirror_index_edge_duplicated(x, size);
      return data[stride * index];
    }
  };
}

//////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class data_fetcher2d_abc
  {
  public:
  
    virtual const T & fetch(const T * data, int width, int height, int stride, int x, int y) const = 0;
  };
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  class fixed_value_data_fetcher2d : public data_fetcher2d_abc<T>
  {
  public:
  
    fixed_value_data_fetcher2d(const T & fixed_value) : _fixed_value(fixed_value) {}
  
    const T & fetch(const T * data, int width, int height, int stride, int x, int y) const override
    {
      return (x < 0) || (x >= width) || (y < 0) || (y >= height) ? _fixed_value : data[stride * (y * width + x)];
    }
    
  private:
    
    T _fixed_value;
  };
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  class duplicated_edge_mirror_data_fetcher2d : public data_fetcher2d_abc<T>
  {
  public:

    const T & fetch(const T * data, int width, int height, int stride, int x, int y) const override
    {
      x = mirror_index_edge_duplicated(x, width);
      y = mirror_index_edge_duplicated(y, height);
      return data[stride * (y * width + x)];
    }
  };
}

#endif
