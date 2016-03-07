#include "ggo_pixel_rect.h"
#include <ggo_kernel.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  pixel_rect::pixel_rect()
  {
    _left   = 0;
    _right  = 0;
    _bottom = 0;
    _top    = 0;
  }

  //////////////////////////////////////////////////////////////
  pixel_rect::pixel_rect(const ggo::rect_float & rect)
  {
    _left   = ggo::to<int>(rect.left());
    _right  = ggo::to<int>(rect.right());
    _bottom = ggo::to<int>(rect.bottom());
    _top    = ggo::to<int>(rect.top());
  }
    
  //////////////////////////////////////////////////////////////
  pixel_rect pixel_rect::from_left_right_bottom_top(int left, int right, int bottom, int top)
  {
    pixel_rect pixel_rect;
    
    pixel_rect._left   = left;
    pixel_rect._right  = right;
    pixel_rect._bottom = bottom;
    pixel_rect._top    = top;
    
    return pixel_rect;
  }

  //////////////////////////////////////////////////////////////
  bool pixel_rect::is_one_pixel() const
  { 
    return _left == _right && _bottom == _top;
  }

  //////////////////////////////////////////////////////////////
  bool pixel_rect::is_visible(int image_width, int image_height) const
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
  bool pixel_rect::crop(int image_width, int image_height)
  {
    if (is_visible(image_width, image_height) == false)
    {
      return false;
    }
    
    _left    = std::max(_left, 0);
    _right   = std::min(_right, image_width - 1);
    _bottom  = std::max(_bottom, 0);
    _top     = std::min(_top, image_height - 1);
    
    return true;
  }
}
  