#ifndef __GGO_BACKGROUND3D_COLOR__
#define __GGO_BACKGROUND3D_COLOR__

#include "ggo_background3d_abc.h"

namespace ggo
{
  class background3d_color : public background3d_abc
  {
  public:

                          background3d_color(const ggo::rgb_32f & color = ggo::black<ggo::rgb_32f>()) : _color(color) {}
                        
    virtual	ggo::rgb_32f  get_color(const ggo::ray3d_f & ray) const { return _color; }
    
    ggo::rgb_32f &			  color() { return _color; }
    const ggo::rgb_32f &	color() const { return _color; }
    
  private:

    ggo::rgb_32f	_color;
  };
}

#endif
