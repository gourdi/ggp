#ifndef __GGO_PIXEL_RECT__
#define __GGO_PIXEL_RECT__

#include <ggo_shapes2d.h>
#include <functional>

namespace ggo
{
  class pixel_rect
  {
  public:
    
          pixel_rect() = default;

          template <typename data_t>
          pixel_rect(const ggo::rect<data_t> & rect);

    int   left() const   { return _left; }
    int   right() const  { return _right; }
    int   bottom() const { return _bottom; }
    int   top() const    { return _top; }
          
    int & left()   { return _left; }
    int & right()  { return _right; }
    int & bottom() { return _bottom; }
    int & top()    { return _top; }
          
    int   width() const  { return _right - _left   + 1; }
    int   height() const { return _top   - _bottom + 1; }
          
    bool  is_one_pixel() const;
          
    bool  is_visible(int image_width, int image_height) const;
          
    bool  crop(int image_width, int image_height);
     
    template <typename pixel_func>
    void  for_each_pixel(const pixel_func & f) const
    {
      for (int y = _bottom; y <= _top; ++y)
      {
        for (int x = _left; x <= _right; ++x)
        {
          f(x, y);
        }
      }
    }

    template <typename data_t>
    ggo::rect<data_t> get_rect() const;
    
  public:
    
    static pixel_rect  from_left_right_bottom_top(int left, int right, int bottom, int top);
    
  private:
    
    int _left = 0;
    int _bottom = 0;
    int _right = 0; // Inclusive.
    int _top = 0;   // Inclusive.
  };
}

//////////////////////////////////////////////////////////////
// Global function.
namespace ggo
{
  template <typename process_block_func>
  void process_blocks(const ggo::pixel_rect & pixel_rect, int block_width, int block_height, process_block_func process_block);
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  inline pixel_rect pixel_rect::from_left_right_bottom_top(int left, int right, int bottom, int top)
  {
    pixel_rect pixel_rect;

    pixel_rect._left = left;
    pixel_rect._right = right;
    pixel_rect._bottom = bottom;
    pixel_rect._top = top;

    return pixel_rect;
  }

  //////////////////////////////////////////////////////////////
  inline bool pixel_rect::is_one_pixel() const
  {
    return _left == _right && _bottom == _top;
  }

  //////////////////////////////////////////////////////////////
  inline bool pixel_rect::is_visible(int image_width, int image_height) const
  {
    if (_right < 0 || _left >= image_width)
    {
      return false;
    }

    if (_top < 0 || _bottom >= image_height)
    {
      return false;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  inline bool pixel_rect::crop(int image_width, int image_height)
  {
    if (is_visible(image_width, image_height) == false)
    {
      return false;
    }

    _left = std::max(_left, 0);
    _right = std::min(_right, image_width - 1);
    _bottom = std::max(_bottom, 0);
    _top = std::min(_top, image_height - 1);

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  pixel_rect::pixel_rect(const ggo::rect<data_t> & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    _left = ggo::to<int>(rect.left());
    _right = ggo::to<int>(rect.right());
    _bottom = ggo::to<int>(rect.bottom());
    _top = ggo::to<int>(rect.top());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::rect<data_t> pixel_rect::get_rect() const
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    data_t left = static_cast<data_t>(_left) - data_t(0.5);
    data_t right = static_cast<data_t>(_right) + data_t(0.5);
    data_t bottom = static_cast<data_t>(_bottom) - data_t(0.5);
    data_t top =  static_cast<data_t>(_top) + data_t(0.5);

    return ggo::rect<data_t>::from_left_right_bottom_top(left, right, bottom, top);
  }

  //////////////////////////////////////////////////////////////
  template <typename process_block_func>
  void process_blocks(const ggo::pixel_rect & pixel_rect, int block_width, int block_height, process_block_func process_block)
  {
    int x = pixel_rect.left();
    int y = pixel_rect.bottom();

    int x_end = std::min(x + block_width - 1, pixel_rect.right());
    int y_end = std::min(y + block_height - 1, pixel_rect.top());

    while (true)
    {
      process_block(pixel_rect::from_left_right_bottom_top(x, x_end, y, y_end));

      // Move to the next block.
      if (x_end < pixel_rect.right())
      {
        x += block_width;
        x_end = std::min(x + block_width - 1, pixel_rect.right());
      }
      else
      {
        GGO_ASSERT(x_end == pixel_rect.right());

        if (y_end == pixel_rect.top())
        {
          // Done.
          break;
        }
        else
        {
          // Go to next line.
          x = pixel_rect.left();
          x_end = std::min(x + block_width - 1, pixel_rect.right());

          y += block_height;
          y_end = std::min(y + block_height - 1, pixel_rect.top());
        }
      }
    }
  }
}

#endif
