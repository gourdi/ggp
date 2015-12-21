#ifndef __GGO_CAUSTIC_LIGHTING__
#define __GGO_CAUSTIC_LIGHTING__

#include <ggo_indirect_lighting_abc.h>

namespace ggo
{
  class caustic_lighting : public indirect_lighting_abc
  {
  public:

    ggo::color  render(int x, int y, const ggo::scene & scene) const override;
  };
}

#endif
