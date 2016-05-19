#ifndef __GGO_IMAGE_ABC__
#define __GGO_IMAGE_ABC__

#include <functional>
#include <ggo_color.h>
#include <ggo_blender_abc.h>

/////////////////////////////////////////////////////////////////////
// Here, const methods keep content unchanged, while non-const method 
// change the content.

/////////////////////////////////////////////////////////////////////
// Base interface that let color being packed/unpacked.
namespace ggo
{
  template <typename color_t>
  class image_abc
  {
  public:

                    image_abc(int width, int height) : _width(width), _height(height) { }
    virtual        ~image_abc() {}

    virtual void    set(int x, int y, const color_t & value) = 0;
    virtual color_t get(int x, int y) const = 0;

            int     get_width() const { return _width; }
            int     get_height() const { return _height; }
           
            void    fill(const color_t & color);

            template <typename color_t2>
            void    from(const image_abc<color_t2> & other);

            void    set(int x, int y, color_t color, float opacity, const ggo::blender_abc<color_t> & blender = ggo::alpha_blender<color_t>());
           
            void    for_each_pixel(const std::function<void(int x, int y)> & func) const;
            void    for_each_pixel(const std::function<void(int y)> & line_func,
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
  template <typename color_t>
  void image_abc<color_t>::fill(const color_t & color)
  {
    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        set(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  template <typename color_t2>
  void image_abc<color_t>::from(const image_abc<color_t2> & other)
  {
    if (get_width() != other.get_width() || get_height() != other.get_height())
    {
      throw dimension_mismatch_exception();
    }

    for (int y = 0; y < _height; ++y)
    {
      for (int x = 0; x < _width; ++x)
      {
        color_t color = ggo::to<color_t>(other.get(x, y));
        set(x, y, color);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  void image_abc<color_t>::set(int x, int y, color_t color, float opacity, const ggo::blender_abc<color_t> & blender)
  {
    GGO_ASSERT(x >= 0 && x < _width);
    GGO_ASSERT(y >= 0 && y < _height);

    set(x, y, blender.blend(get(x, y), opacity, color));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  void image_abc<color_t>::for_each_pixel(const std::function<void(int x, int y)> & func) const
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
  template <typename color_t>
  void image_abc<color_t>::for_each_pixel(const std::function<void(int y)> & line_func,
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
