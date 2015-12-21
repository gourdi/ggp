#ifndef __GGO_INDIRECT_LIGHTING_ABC__
#define __GGO_INDIRECT_LIGHTING_ABC__

#include <memory>
#include <ggo_scene.h>

namespace ggo
{
  class indirect_lighting_abc
  {
  public:

    virtual ggo::color  render(int x, int y, const ggo::scene & scene) const = 0;
  };
}

#endif
