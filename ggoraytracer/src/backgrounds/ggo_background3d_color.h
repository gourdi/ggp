#ifndef __GGO_BACKGROUND3D_COLOR__
#define __GGO_BACKGROUND3D_COLOR__

#include "ggo_background3d_abc.h"

namespace ggo
{
  class background3d_color : public background3d_abc
  {
  public:

                        background3d_color(const ggo::color & color = ggo::color::BLACK) : _color(color) {}
                        
    virtual	ggo::color  get_color(const ggo::ray3d_float & ray) const { return _color; }
    
    ggo::color &			  color() { return _color; }
    const ggo::color &	color() const { return _color; }
    
  private:

    ggo::color	_color;
  };
}

#endif
