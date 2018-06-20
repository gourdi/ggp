#ifndef __GGO_PIXEL_INT__
#define __GGO_PIXEL_INT__

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_vec.h>
#include <kernel/ggo_size.h>

namespace ggo
{
  class rect_int final
  {
  public:
    
              rect_int() = default;

    int       left() const   { return _left; }
    int       right() const  { return _right; }
    int       bottom() const { return _bottom; }
    int       top() const    { return _top; }
              
    int &     left()   { return _left; }
    int &     right()  { return _right; }
    int &     bottom() { return _bottom; }
    int &     top()    { return _top; }
              
    int       width() const  { return _right - _left   + 1; }
    int       height() const { return _top   - _bottom + 1; }

    ggo::size size() const { return { width(), height() }; }
          
    bool      is_one() const;

    bool      clip(int image_width, int image_height);
    bool      clip(const rect_int & other);
 
    template <typename pixel_func>
    void      for_each_pixel(const pixel_func & f) const
    {
      for (int y = _bottom; y <= _top; ++y)
      {
        for (int x = _left; x <= _right; ++x)
        {
          f(x, y);
        }
      }
    }

    bool is_inside(const ggo::pos2i & p) const;

    void vertical_flip(int height);

    bool operator==(const ggo::rect_int & r) const { return _left == r._left && _bottom == r._bottom && _right == r._right && _top == r._top; }
    bool operator!=(const ggo::rect_int & r) const { return !this->operator==(r); }
    
  public:
    
    static rect_int  from_left_right_bottom_top(int left, int right, int bottom, int top);
    static rect_int  from_width_height(int width, int height);
    static rect_int  from_size(const ggo::size & size);

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
  void process_blocks(const ggo::rect_int & rect, int block_width, int block_height, process_block_func process_block);

  inline std::ostream & operator<<(std::ostream & os, const ggo::rect_int & r)
  {
    os << "left: " << r.left() << ", right: " << r.right() << ", bottom: " << r.bottom() << ", top: " << r.top();
    return os;
  }
}

//////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  //////////////////////////////////////////////////////////////
  inline rect_int rect_int::from_left_right_bottom_top(int left, int right, int bottom, int top)
  {
    GGO_ASSERT_LE(left, right);
    GGO_ASSERT_LE(bottom, top);

    rect_int rect;

    rect._left = left;
    rect._right = right;
    rect._bottom = bottom;
    rect._top = top;

    return rect;
  }

  //////////////////////////////////////////////////////////////
  inline rect_int rect_int::from_width_height(int width, int height)
  {
    GGO_ASSERT_GT(width, 0);
    GGO_ASSERT_GT(height, 0);

    rect_int rect;

    rect._left = 0;
    rect._right = width - 1;
    rect._bottom = 0;
    rect._top = height - 1;

    return rect;
  }

  //////////////////////////////////////////////////////////////
  inline rect_int rect_int::from_size(const ggo::size & size)
  {
    return from_width_height(size.width(), size.height());
  }

  //////////////////////////////////////////////////////////////
  inline bool rect_int::is_one() const
  {
    return _left == _right && _bottom == _top;
  }

  //////////////////////////////////////////////////////////////
  inline bool rect_int::clip(int image_width, int image_height)
  {
    return clip(from_width_height(image_width, image_height));
  }

  //////////////////////////////////////////////////////////////
  inline bool rect_int::clip(const rect_int & other)
  {
    if (_right < other._left || _left > other._right)
    {
      return false;
    }

    if (_top < other._bottom || _bottom > other._top)
    {
      return false;
    }

    _left = std::max(_left, other._left);
    _right = std::min(_right, other._right);
    _bottom = std::max(_bottom, other._bottom);
    _top = std::min(_top, other._top);

    return true;
  }

  //////////////////////////////////////////////////////////////
  inline void rect_int::vertical_flip(int height)
  {
    int top    = height - 1 - _bottom;
    int bottom = height - 1 - _top;

    _bottom = bottom;
    _top = top;
  }

  //////////////////////////////////////////////////////////////
  inline bool rect_int::is_inside(const ggo::pos2i & p) const
  {
    return p.x() >= _left && p.x() <= _right && p.y() >= _bottom && p.y() <= _top;
  }

  //////////////////////////////////////////////////////////////
  template <typename process_block_func>
  void process_blocks(const ggo::rect_int & rect, int block_width, int block_height, process_block_func process_block)
  {
    int x = rect.left();
    int y = rect.bottom();

    int x_end = std::min(x + block_width - 1, rect.right());
    int y_end = std::min(y + block_height - 1, rect.top());

    while (true)
    {
      process_block(rect_int::from_left_right_bottom_top(x, x_end, y, y_end));

      // Move to the next block.
      if (x_end < rect.right())
      {
        x += block_width;
        x_end = std::min(x + block_width - 1, rect.right());
      }
      else
      {
        GGO_ASSERT(x_end == rect.right());

        if (y_end == rect.top())
        {
          // Done.
          break;
        }
        else
        {
          // Go to next line.
          x = rect.left();
          x_end = std::min(x + block_width - 1, rect.right());

          y += block_height;
          y_end = std::min(y + block_height - 1, rect.top());
        }
      }
    }
  }
}

#endif
