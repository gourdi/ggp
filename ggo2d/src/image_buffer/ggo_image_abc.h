#ifndef __GGO_IMAGE_ABC__
#define __GGO_IMAGE_ABC__

#include <functional>
#include <ggo_color.h>

/////////////////////////////////////////////////////////////////////
// Here, const methods keep content unchanged, while non-const method 
// change the content.

/////////////////////////////////////////////////////////////////////
// Base interface that let color being packed/unpacked.
namespace ggo
{
  template <typename color_type>
  class image_abc
  {
  public:

                        image_abc(int width, int height) : _width(width), _height(height) { }
    virtual            ~image_abc() {}

    virtual void        write(int x, int y, const color_type & value) = 0;
    virtual color_type  read(int x, int y) const = 0;

            int         get_width() const { return _width; }
            int         get_height() const { return _height; }
           
            void        fill(const color_type & color);

            void        convert(const image_abc<color_type> & other);
           
            void        for_each_pixel(const std::function<void(int x, int y)> & func) const;
            void        for_each_pixel(const std::function<void(int y)> & line_func,
                                       const std::function<void(int x, int y)> & pixel_func) const;

  protected:

    int  _width = 0;
    int  _height = 0;
  };

  using gray_image_abc = image_abc<float>;
  using rgb_image_abc  = image_abc<ggo::color>;
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void image_abc<color_type>::fill(const color_type & color)
  {
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        write(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void image_abc<color_type>::convert(const image_abc<color_type> & other)
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
        color_type color = other.read(x, y);
        write(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void image_abc<color_type>::for_each_pixel(const std::function<void(int x, int y)> & func) const
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
  template <typename color_type>
  void image_abc<color_type>::for_each_pixel(const std::function<void(int y)> & line_func,
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
