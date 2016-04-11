#ifndef __GGO_PACKER_ABC__
#define __GGO_PACKER_ABC__

#include <functional>
#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// Here, const methods keep content unchanged, while non-const method 
// change the content.
namespace ggo
{
  template <typename T>
  class image_data_abc
  {
  public:

                  image_data_abc(int width, int height) : _width(width), _height(height) {}
    virtual      ~image_data_abc() {}

            void  pack(int x, int y, const T & value);
            T     unpack(int x, int y) const;
    
            int   get_width() const { return _width; }
            int   get_height() const { return _height; }
            
            void  fill(const T & color);
            void  copy(const image_data_abc<T> & other);
            
            void  for_each_pixel(const std::function<void(int x, int y)> & func) const;
            void  for_each_pixel(const std::function<void(int y)> & line_func,
                                 const std::function<void(int x, int y)> & pixel_func) const;

  private:

    virtual void  do_pack(int x, int y, const T & value) = 0;
    virtual T     do_unpack(int x, int y) const = 0;

  private:
    
    int _width = 0;
    int _height = 0;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void image_data_abc<T>::pack(int x, int y, const T & value)
  {
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    do_pack(x, y, value);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T image_data_abc<T>::unpack(int x, int y) const
  {
    GGO_ASSERT(_width > 0 && _height > 0);
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);
    
    return do_unpack(x, y);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void image_data_abc<T>::fill(const T & color)
  {
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        pack(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void image_data_abc<T>::copy(const image_data_abc<T> & other)
  {
    if (get_width() != other.get_width() ||
        get_height() != other.get_height())
    {
      throw dimension_mismatch_exception();
    }
    
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        T color = other.unpack(x, y);
        pack(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void image_data_abc<T>::for_each_pixel(const std::function<void(int x, int y)> & func) const
  {
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        func(x, y);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void image_data_abc<T>::for_each_pixel(const std::function<void(int y)> & line_func,
                                         const std::function<void(int x, int y)> & pixel_func) const
  {
    for (int y = 0; y < _height; ++y)
    {
      line_func(y);
      
      for (int x = 0; x < _width; ++x)
      {
        pixel_func(x, y);
      }
    }
  }
}

#endif
