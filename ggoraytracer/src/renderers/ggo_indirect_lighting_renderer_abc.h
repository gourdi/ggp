#ifndef __GGO_INDIRECT_LIGHTINH_RENDERER_ABC__
#define __GGO_INDIRECT_LIGHTINH_RENDERER_ABC__

#include <memory>
#include <ggo_scene.h>

namespace ggo
{
  class indirect_lighting_renderer_abc
  {
  public:

    virtual ggo::color  render(int x, int y, const ggo::scene & scene) const = 0;
  };
}

#endif
