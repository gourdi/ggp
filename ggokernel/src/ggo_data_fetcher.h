#ifndef __GGO_DATA_FETCHER__
#define __GGO_DATA_FETCHER__

#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
// 1D, non-const
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class data_fetcher1d_abc
  {
  public:
  
    virtual const T & fetch(const T * data, int size, int stride, int x) const = 0;
    virtual T &       fetch(T * data, int size, int stride, int x) const = 0;
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

    T & fetch(T * data, int size, int stride, int x) const override
    {
      int index = mirror_index_edge_duplicated(x, size);
      return data[stride * index];
    }
  };
}

//////////////////////////////////////////////////////////////
// 1D, const
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class const_data_fetcher1d_abc
  {
  public:

    virtual T fetch(const T * data, int size, int stride, int x) const = 0;
  };

  //////////////////////////////////////////////////////////////
  template <typename T, T fixed_value>
  class fixed_value_data_fetcher1d : public const_data_fetcher1d_abc<T>
  {
  public:

    T fetch(const T * data, int size, int stride, int x) const override
    {
      return (x < 0) || (x >= size) ? fixed_value : data[stride * x];
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename T>
  class duplicated_edge_mirror_const_data_fetcher1d : public const_data_fetcher1d_abc<T>
  {
  public:

    T fetch(const T * data, int size, int stride, int x) const override
    {
      int index = mirror_index_edge_duplicated(x, size);
      return data[stride * index];
    }
  };
}

//////////////////////////////////////////////////////////////
// 2D, non-const
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class data_fetcher2d_abc
  {
  public:

    virtual const T & fetch(const T * data, int width, int height, int stride, int x, int y) const = 0;
    virtual T &       fetch(T * data, int width, int height, int stride, int x, int y) const = 0;
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

    T & fetch(T * data, int width, int height, int stride, int x, int y) const override
    {
      x = mirror_index_edge_duplicated(x, width);
      y = mirror_index_edge_duplicated(y, height);
      return data[stride * (y * width + x)];
    }
  };
}

//////////////////////////////////////////////////////////////
// 2D, const
namespace ggo
{
  template <typename T>
  T & fetch_data_standard(T * data, int width, int height, int stride, int x, int y)
  {
    return data[stride * (y * width + x)];
  }

  template <typename T>
  T fetch_data_standard_const(const T * data, int width, int height, int stride, int x, int y)
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



  //////////////////////////////////////////////////////////////
  template <typename T>
  class const_data_fetcher2d_abc
  {
  public:

    virtual T fetch(const T * data, int width, int height, int stride, int x, int y) const = 0;
  };

  //////////////////////////////////////////////////////////////
  template <typename T, T fixed_value>
  class fixed_value_data_fetcher2d : public const_data_fetcher2d_abc<T>
  {
  public:

    const T & fetch(const T * data, int width, int height, int stride, int x, int y) const override
    {
      return (x < 0) || (x >= width) || (y < 0) || (y >= height) ? fixed_value : data[stride * (y * width + x)];
    }
  };

  //////////////////////////////////////////////////////////////
  template <typename T>
  class duplicated_edge_mirror_const_data_fetcher2d : public const_data_fetcher2d_abc<T>
  {
  public:

    T fetch(const T * data, int width, int height, int stride, int x, int y) const override
    {
      x = mirror_index_edge_duplicated(x, width);
      y = mirror_index_edge_duplicated(y, height);
      return data[stride * (y * width + x)];
    }
  };
}

#endif
