#ifndef __GGO_SUBSTEPS_PROCESSOR__
#define __GGO_SUBSTEPS_PROCESSOR__

#include <kernel/ggo_ratio.h>

namespace ggo
{
  struct substeps_processing
  {
    substeps_processing(ggo::ratio substeps_per_frame) : _substeps_per_frame(substeps_per_frame) {}

    template <typename func_t>
    void call(func_t && func)
    {
      for (_substeps_current += _substeps_per_frame; _substeps_current >= 1; _substeps_current -= 1, ++_substeps_count)
      {
        func();
      }
    }

    const ggo::ratio  _substeps_per_frame;
    ggo::ratio        _substeps_current = 0;
    int               _substeps_count = 0;
  };
}

#endif
