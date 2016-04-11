#ifndef __GGO_PIXEL_RECT__
#define __GGO_PIXEL_RECT__

#include <ggo_shapes2d.h>
#include <functional>

namespace ggo
{
  class pixel_rect
  {
  public:
    
          pixel_rect();
          pixel_rect(const ggo::rect_float & rect);

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

    ggo::rect_float get_rect_float() const;
    
  public:
    
    static pixel_rect  from_left_right_bottom_top(int left, int right, int bottom, int top);
    
  private:
    
    int _left;
    int _bottom;
    int _right; // Inclusive.
    int _top;   // Inclusive.
  };
}

#endif
